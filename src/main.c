#include "main.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	switch(argc){
		case 1:
			fprintf(stderr, "Usage: ./boundless <file>\n");
			exit(10);

			break;
		case 2:
			for(int i = 0; i < argc; i++){
				fprintf(stdout, "%s ", argv[i]);
			}
			fprintf(stdout, "\n");

			break;
	}
	
	return 0;
}
