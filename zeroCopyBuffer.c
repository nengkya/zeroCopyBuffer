#include "stdio.h"
#include "string.h" /*strcmp();*/
#include <stdlib.h> /*exit(1) from anywhere on your code will terminate program execution immediately*/


void copy(const char * ) {

	printf("copy\n");

}

void zeroCopy() {

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

	if mode = 1, copy <source file> <destination file> <mode>
	else 	 zeroCopy <source file> <destination file> <mode>
	*/
	if (strcmp(argumentValue[3], "1")) copy(); else zeroCopy();


}
