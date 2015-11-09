/*JIMMY LE 				jl1415
 *CARLOS DOMINGUEZ		cmd363
 *	index.c
 */

#include "index.h"
#include "tokenizer.h"

int main(int argc, char **argv)
{
	if (argc != 3)
	{ 
		printf("Usage: index <inverted-index file name> <directory or file name>\n");
		return 1;
	}

	int i;
	int writeCheck;
	int offset = 0;

	char *wPath = argv[1];
	char *path = argv[2];


	for (i = 1; i <= 47; i++) {
		delims[offset++] = i;
	}
	for (i = 58; i <= 64; i++) {
		delims[offset++] = i;
	}
	for (i = 91; i <= 96; i++) {
		delims[offset++] = i;
	}
	for (i = 123; i <= 127; i++) {
		delims[offset++] = i;
	}
	delims[offset] = '\0';


	if (isDirectory(path) == 1)
	{ 
		evalDir(path);
	} else{ 
		evalFile(path);
	}
	
	writeCheck = writeFile(wPath);
	clear();

	if (writeCheck == 1) {
		return 0; 
	} else {
		printf("ERROR: Unable to write to File\n");
		return 1;
	}
}