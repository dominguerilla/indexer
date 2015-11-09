#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */


int main(int argc, char **argv) {

	//Checks if the user enters the right amount of inputs
	if(argc != 2){
		printf("Invalid Number of Input\n"); //If the amount of inputs is not 2, then prints out an error message and returns 1
		return 1;
	}

	TokenizerT *tk = TKCreate(argv[1]);

	if((strcmp(argv[1], "0") == 0) || (strcmp(argv[1], " 0 ") == 0)){
		printf("decimal constant \"0\" \n"); //If the amount of inputs is not 2, then prints out an error message and returns 1
		return 0;
	}

	while (tk->cursorPosition < tk->tokenLength){
		char *token = TKGetNextToken(tk);

		if (token == NULL){
			tk->cursorPosition++;
			continue;
		}

		printf("%s\n", token);
	}

	TKDestroy(tk);

  return 0;
}

