#include "stdio.h"
#include "string.h" /*strcmp();*/
#include <stdlib.h> /*exit(1) from anywhere on your code will terminate program execution immediately*/


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

	int fileDescriptorSource;

	/*
	open() function establish connection between a file and a file descriptor
	it shall create an open file description that refers to a file
	and a file descriptor that refers to that open file description
	the file descriptor is used by other I/O functions to refer to that file
	*/
	fileDescriptorSource = open();


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
