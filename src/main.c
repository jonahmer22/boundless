#include "main.h"

#include <stdlib.h>
#include <stdio.h>

char *readFile(const char *path, size_t *outLen){
	FILE *file = fopen(path, "rb");	// rb means read as binary so I get all bytes
	if(!file)
		return NULL;

	// get the file size in bytes
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// allocate the char buffer size
	char *buffer = malloc(sizeof(char) * size + 1);	// add one for EOF
	if(!buffer){
		fclose(file);
	
		return NULL;
	}
	
	// read the file into the buffer
	if(fread(buffer, 1, size, file) != (size_t)size){
		free(buffer);
		fclose(file);
		
		return NULL;
	}

	buffer[size] = '\0';	// add back EOF
	if(outLen)	// if we want the size back
		*outLen = (size_t)size;

	return buffer;
}

int main(int argc, char **argv){
	if(argc < 2 || argc >= 3){
		printf("Usage: ./boundless <path/to/file>\n");

		return 1;
	}

	size_t length = 0;
	char *buffer = NULL;
	buffer = readFile(argv[argc - 1], &length);

	// just for testing prints the file and it's length
	printf("%s\n%s contains %ld Bytes\n", buffer, argv[argc - 1], length);

	free(buffer);

	return 0;
}
