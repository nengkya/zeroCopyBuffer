#include "stdio.h"
#include "string.h" /*strcmp();*/
#include <stdlib.h> /*exit(1) from anywhere on your code will terminate program execution immediately*/
#include "fcntl.h" /*O_RDONLY*/
#include "sys/stat.h" /*fstat();*/
#include "sys/sendfile.h" /*sendfile();*/

void copy(const char * sourceFilePath, const char * destinationFilePath) {

	FILE * sourceFilePointer, * destinationFilePointer;

	/*
	since size_t unsigned, size_t may have twice (or more) the positive range of a signed int.
	so if the subscript type were signed int, it might be possible to allocate (using size_t) an array
	with more elements than can be indexed (using int).
	*/
	size_t lengthOfFileBuffer;

	int bufferSize = 8 * 1024;
	char fileBuffer[bufferSize];

	sourceFilePointer 	   = fopen(sourceFilePath, "rb");
	destinationFilePointer = fopen(destinationFilePath, "wb");

	while (!feof(sourceFilePointer)) {

		/*
		the return value of fread is the number of items successfully read from the file
		if fread returns 0, it means no items were read, which could be due to an error or reaching the end of the file
		size_t fread(void * fileBuffer, size_t sizeOfEachElement, size_t countOfElementsToBeRead, FILE * stream);
		*/
		lengthOfFileBuffer = fread(fileBuffer, 1, bufferSize, sourceFilePointer);

		/*
		size_t fwrite(const void * restrict buffer, size_t sizeofEachElement,
			size_t countOfElementToBeWrite, FILE * restrict stream);*/
		fwrite(fileBuffer, 1, lengthOfFileBuffer, destinationFilePointer);

	}

	printf("copy\n");

}


void zeroCopy(const char * sourceFilePath, const char * destinationFilePath) {

	int fileDescriptorSource, fileDescriptorDestination, returnCountOfBytesTransferred;

	struct stat statusBuffer;

	off_t offset; /*used for file sizes*/

	/*
	open() function establish connection between a file and a file descriptor
	it shall create an open file description that refers to a file
	and a file descriptor that refers to that open file description
	the file descriptor is used by other I/O functions to refer to that file
	
	int open (const char * path, int flags);
	*/
	fileDescriptorSource = open(sourceFilePath, O_RDONLY);

	/*
	struct stat {
		dev_t	  st_dev	  identifies the device containing the file.
							  the st_dev value is not necessarily consistent across reboots or system crashes, however
		ino_t	  st_ino;
		mode_t	  st_mode;	  specifies file type information and the file permission bits
		ino_t	  st_ino	  the file serial number,
							  which distinguishes this file from all other files on the same device
		nlink_t	  st_nlink;	  the number of hard links to the file.
							  count track of how many directories have entries for this file.
							  if the count is ever decremented to zero,
							  then the file itself is discarded as soon as no process still holds it open.
							  symbolic links are not counted in the total
		uid_t	  st_uid;	  user ID of the file’s owner
		gid_t	  st_gid;	  group ID of the file
		dev_t	  st_rdev;	  device ID (if file is character or block special)
		off_t	  st_size;	  specifies the size of a regular file in bytes.
							  for files that are really devices, this field isn’t usually meaningful.
							  for symbolic links this specifies the length of the file name the link refers to
		time_t	  st_atime;	  the last access time for the file
		time_t	  st_mtime;	  the time of the last modification to the contents of the file
		time_t	  st_ctime;	  the last time the file's inode was changed (e.g. permissions changed, file renamed, etc..)
		blksize_t st_blksize; a file system-specific preferred I/O block size for this object.
							  in some file system types, this may vary from file to file. 
		blkcnt_t  st_blocks;  number of blocks allocated for this object.
		mode_t	  st_attr;
	}; 
	int fstat(int fd, struct stat * buf); file status
	*/
	fstat(fileDescriptorSource, &statusBuffer);

	fileDescriptorDestination = open(destinationFilePath, O_RDONLY | O_CREAT, statusBuffer.st_size);
	
	/*
	sendfile(); copies data between one file descriptor and another.
	because this copying is done within the kernel,
	sendfile(); is more efficient than the combination of read(); and write();,
	which would require transferring data to and from user space.
	ssize_t is signed size_t
	ssize_t sendfile(int out_fd, int in_fd, off_t * _Nullable offset, size_t count);
	*/
	returnCountOfBytesTransferred = sendfile(fileDescriptorDestination, fileDescriptorSource,
		&offset, statusBuffer.st_size);

	if (returnCountOfBytesTransferred != statusBuffer.st_size) {

		fprintf(stderr, "incompatible transfer from sendfile(); %d of %d bytes\n",
			returnCountOfBytesTransferred, (int)statusBuffer.st_size);

		exit(1);

	}

	printf("zero-copy\n");

}


int main(int argumentCounter, char ** argumentValue) {

	if (argumentCounter != 4) {
		printf("usage : %s <source file> <destination file> <mode>\n", argumentValue[0]);
		exit(1);
	}

	/*
	return value from strcmp()
	 0 if strings are equal	
	>0 if the first non-matching character in str1 is greater (in ASCII) than that of str2
	<0 if the first non-matching character in str1 is lower (in ASCII)   than that of str2

	if mode = 1, zero-copy <source file> <destination file> <mode>
	else 			  copy <source file> <destination file> <mode>
	*/
	if (strcmp(argumentValue[3], "1"))
		copy(argumentValue[1], argumentValue[2]);
	else
		zeroCopy(argumentValue[1], argumentValue[2]);

}
