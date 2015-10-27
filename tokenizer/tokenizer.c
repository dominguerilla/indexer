//JIMMY LE          jl1415
//CARLOS DOMINGUEZ  cmd363
/*
 * tokenizer.c
 */

#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int const TOKEN_TYPE_OFFSET = 10;
static int ecounter = 0;
static int andCounter = 0;
static int orCounter = 0;
static int rightcounter = 0;
static int leftcounter = 0;


/*
 * charToString will return a string that is the concatenation of the string a and the character c.
 */

char* charToString(const char* a, char c)
{
    size_t len = strlen(a) + 1 + 1; // 1 for character and 1 for null terminator.

    char *retStr;

    if (a == NULL || (retStr = malloc(len)) == NULL) { // Checks for enough memory to be used.
        return NULL;
    }

    sprintf(retStr, "%s%c", a, c); 
    return retStr;
}

/*
 * toString will return a string that is the concatenation of a and b. 
 */

char* toString(const char* a, const char* b)
{
    size_t len = strlen(a) + strlen(b) + 1;
    char *retStr = malloc(len);

    sprintf(retStr, "%s%s", a, b);
    return retStr;
}

/*
 * numToken evaluates each numerical input from string, and checks for each specification to see
 * if the input belongs numerically to a certain identification such as octal, float, decimal, and hex.
 */
char* numToken(TokenizerT* tk) 
{
    const char *string = tk->tokenString;

    if (isspace(string[tk->cursorPosition])) {
        return NULL;
    }

    int isFirstChar = 1;
    int isSecondChar = 1;
    int period = 0;
    int isE = 0;


    char numeric = 'r'; // d - decimal, o - octal, x - hex, f - float, z - zero, r - Error.  Defaults to Zero.
    char *number = malloc(tk->tokenLength + 1);


    int i = 0;
    while (tk->cursorPosition < tk->tokenLength && !isspace(string[tk->cursorPosition])) {
        char c = string[tk->cursorPosition];

        if(isdigit(c) && isFirstChar) { //First char is a digit & its the first
            number[i] = c;
            if (c != '0'){
                numeric = 'd';    //If isdigit and isFirstchar not being 0 then its a decimal.
            }else {
                numeric = 'z';
            }      
            if(c == '0'){       //If it is zero, then it is hex.
                numeric = 'x';
                char d = string[tk->cursorPosition+1];
                char e = string[tk->cursorPosition+2];


                if (d == ' '){           //Else if it is a white space, then it is zero.
                    numeric = 'd';
                }else if (d != 'x' && d != 'X'){    //Else if there is no x after 0, then it is octal.
                    numeric = 'o';
                }else if (d == 'x' && d == 'X' && e != '\\'){
                    numeric = 'x';
                }else if ((d != 'x' && d != 'X' && d == '8') || (d != 'x' && d != 'X' && d == '9') || (d != 'x' && d != 'X' && e == '\\')){
                    numeric = 'r';
                }
            }
            isFirstChar = 0;
            i++;
            tk->cursorPosition++;

        } else if((!isFirstChar && isalnum(c)) || (!isFirstChar && c == '.') || (!isFirstChar && c == '-') ){ //not first and its alphanumeric


            if ((c == 'x' || c == 'X') && isSecondChar && numeric == 'x'){
                isSecondChar = 0;
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            }


            if (numeric == 'x' && ((c >= 65 && c <=70) || (c >= 97 && c <= 102) || isdigit(c))){
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            } else if(numeric == 'x' && isalpha(c)) {
                break;
            }


            if (numeric != 'x' && !period && c == '.'){
                numeric = 'f';
                period = 1;
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            }


            if (numeric == 'f' && isE && isalpha(c)){
                break;
            }


            if ((numeric == 'f' && period && isdigit(c)) || (numeric == 'f' && period && (c == 'e' || c == 'E'))){


                if(c == 'e' || c == 'E') {
                    char u = string[tk->cursorPosition+1];
                    if (isalpha(u)){
                        break;
                    }
                    isE = 1;
                }
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            }


            if (numeric == 'f' && c == '-' && isE){
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            } else if (numeric != 'f' && c == '-'){
                break;
            }


            if (numeric == 'o' && c <= 55 && isdigit(c)){
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            } else if (numeric == 'o' && isdigit(c)){
                //numeric = 'd';
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            } 

            if (numeric == 'd' && isdigit(c) ){
                number[i] = c;
                i++;
                tk->cursorPosition++;
                continue;
            }

            break;

        } else {
            numeric = 'r';
            return NULL; // Not a numeric.
        }
    }


    number[i] = '\0';


    char *retStr = NULL;	//Checks and Return correct numeric value with correct specification.
    switch(numeric) {
        /*case 'z':
            retStr = malloc(strlen(number) + strlen("zero \"" + 2));
            strcpy(retStr, "decimal constant \"");
            strcat(retStr, number);
            break;*/
        case 'o':
            retStr = malloc(strlen(number) + strlen("octal constant \"" + 2));
            strcpy(retStr, "octal constant \"");
            strcat(retStr, number);
            break;
        case 'd':
            retStr = malloc(strlen(number) + strlen("decimal constant \"" + 2));
            strcpy(retStr, "decimal constant \"");
            strcat(retStr, number);
            break;
        case 'x':
            retStr = malloc(strlen(number) + strlen("hex constant \"" + 2));
            strcpy(retStr, "hex constant \"");
            strcat(retStr, number);
            break;
        case 'f':
            retStr = malloc(strlen(number) + strlen("float constant \"" + 2));
            strcpy(retStr, "float constant \"");
            strcat(retStr, number);
            break;
        case 'r':
            retStr = malloc(strlen(number) + strlen("Error \"" + 2));
            strcpy(retStr, "Error \"");
            strcat(retStr, number);
            break;
    }

    strcat(retStr, "\"");

    return retStr;
}

/*
 * operatorToken checks for special characters and operators, which if there does exist in the input, it will return an error message
 * with correct hex represenation.
 */
char* operatorToken(TokenizerT* tk) {


    const char *string = tk->tokenString;


    if (isspace(string[tk->cursorPosition])) {
        return NULL;
    }


    char token[4];
    strncpy(token, string+tk->cursorPosition, 3);
    token[3] = '\0';


    char *retStr = NULL;


    if (retStr != NULL) {
        tk->cursorPosition += 3; // Move the cursor by 3.
        return retStr;
    }

    if (retStr != NULL) {
        tk->cursorPosition += 2; // Move the cursor by 2.
        return retStr;
    }

    switch(token[0]) {	//Checks and Returns Error Message with Escape Operators or Certain Special Characters with Hex Identification.
        case '(':
            retStr = "left parenthesis \"(\""; //This was the orignal return statement
            break;
        case ')':
            retStr = "right parenthesis \")\"";
            break;
        case '[':
            retStr = "left brace \"[\"";
            break;
        case ']':
            retStr = "right brace \"]\"";
            break;
        case '{':
            retStr = "Error [0x7b]";
            break;
        case '}':
            retStr = "Error [0x2d]";
            break;
        case '.':
            retStr = "Error [0x2e]";
            break;
        case '&':
        	if (token[1] == '&'){
       			retStr = "logical and \"&&\"";
       			andCounter++;
       			break;
       		}else{
            	retStr = "bitwise and \"&\"";
            	break;
        	}	
        case '-':
        	if (token[1] == '='){
       			retStr = "subequals \"-=\"";
       			ecounter++;
       			break;
        	}else{
            	retStr = "subtract \"-\"";
            	break;
            }
        case '!':
        	if (token[1] == '=')
        	{
        		retStr = "not equal \"!=\"";
        		ecounter++;
        		break;
        	}else{
        		retStr = "Error [0x21]";
            	break;
            }
        case '~':
            retStr = "Error [0x7e]";
            break;
        case '*':
        	if (token[1] == '=')
        	{
       			retStr = "multiequals \"*=\"";
       			ecounter++;
       			break;
        	}else if (ecounter > 0){
        		andCounter = 0;
        		break;
        	}else{
            	retStr = "multiply \"*\"";
            	break;
        	}
        case '/':
            if (token[1] == '='){
            	retStr = "divideequals \"/=\"";
            	ecounter++;
            	break;
        	} else{
            	retStr = "divide \"/\"";
            	break;
        	}
        case '%':
        	if (token[1] == '='){
        		retStr = "modulusequals \"%=\"";
        		ecounter++;
       			break;
       		} else {
            	retStr = "modulus \"%";
            	break;
        	}
        case '+':
            if (token[1] == '='){
        		retStr = "plusequals \"+=\"";
        		ecounter++;
       			break;
       		}else{
            	retStr = "add \"+\"";
            	break;
        	}
        case '<':
        	if (leftcounter > 0){
        		leftcounter = 0;
        		break;
            }else if (token[1] == '='){
       			retStr = "less or equal \"<=\"";
       			ecounter++;
       			break;
       		}else if (token[1] == '<'){	       
       			retStr = "shift left \"<<\"";
       			leftcounter++;
       			break;
       		}else {
            	retStr = "less than \"<\"";
            	break;
        	}
        case '>':
        	if (rightcounter > 0){
        		rightcounter = 0;
        		break;
        	}else if (token[1] == '='){
       			retStr = "greater or equal \">=\"";
       			ecounter++;
       			break;
       		}else if (token[1] == '>'){
       			retStr = "shift right \">>\"";
       			rightcounter++;
       			break;
        	}else{
            	retStr = "greater than \">\"";
            	break;
            }
        case '^':
            retStr = "bitwise exclusive or \"^\"";
            break;
        case '|':
        	if (orCounter > 0){
        		orCounter = 0;
        		break;
        	}else if (token[1] == '|'){
       			retStr = "logical or \"||\"";
       			orCounter++;
       			break;
        	}else{
            	retStr = "bitwise or \"|\"";
            	break;
        	}
        case ',':
            retStr = "comma \",\"";
            break;
       	case '=':
       		if (ecounter > 0){
       			ecounter = 0;
       			break;
       		}else if (token[1] == '='){
            	retStr = "equals \"==\"";
            	ecounter++;
            	break;
            }else{
       			retStr = "equal \"=\"";
       			break;
       		}
        case '\\':
            if ((token[1] == 't') || (token[1] == 'v') || (token[1] == 'f') || (token[1] == 'r') || (token[1] == 'n')){
                break;
            }


    } 

    if (retStr != NULL) {
        tk->cursorPosition++; // Increment the cursor.
    }

    return retStr;
}

/*
 * wordToken checks for words in string.  Pointer checks each individual character and prints out the entire word until white space.
 * Also contains the extra credit method (1) to identify DISTINCT C TOKENS
 */

char* wordToken(TokenizerT* tk) {


    const char *string = tk->tokenString;
    char *token = NULL;
    int isFirstChar = 1;

    if (isspace(string[tk->cursorPosition])) {
        return NULL;
    }
    while (tk->cursorPosition < tk->tokenLength && !isspace(string[tk->cursorPosition])) {

        char c = string[tk->cursorPosition];
        
	/*
	char d = string[tk->cursorPosition+1];
        char e = string[tk->cursorPosition+2];
        char f = string[tk->cursorPosition+3];
        char g = string[tk->cursorPosition+4];
        char h = string[tk->cursorPosition+5];
        char i = string[tk->cursorPosition+6];
        char j = string[tk->cursorPosition+7];
        char k = string[tk->cursorPosition+8];
        char l = string[tk->cursorPosition+9];
        */
        if(isalpha(c) || (isalnum(c) && !isFirstChar)){

           /** if ( ((c == '\n')) || (d == 't') || (d == 'v') || (d == 'f') || (d == 'r') ){ // Words are alphabetic followed by alphanumeric.
                return NULL;
            }**/

            if ( ((c == '\n')) || (c == '\t') || (c == '\v') || (c == '\f') || (c == '\r') ){ // Words are alphabetic followed by alphanumeric.
                return NULL;
            }
            
            /*EXTRA CREDIT FUNCTION (1): Looks for distinct C Keywords, and prints them out with a message
            if  (isFirstChar && c == 'a' && d == 'u' && e == 't' && f == 'o' && (g == ' ' || g =='\0')){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if (isFirstChar && c == 'b' && d == 'r' && e == 'e' && f == 'a' && g == 'k' && (h == ' ' || h == '\0')){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'c' && d == 'a' && e == 's' && f == 'e' && (g == ' ' || g == '\0')) || (isFirstChar && c == 'c' && d == 'h' && e == 'a' && f == 'r' && (g == ' ' || g == '\0')) || (isFirstChar && c == 'c' && d == 'o' && e == 'n' && f == 's' && g == 't' && (h ==' ' || h == '\0')) || (isFirstChar && c == 'c' && d == 'o' && e == 'n' && f == 't' && g == 'i' && h == 'n' && i == 'u' && j =='e' && (k == ' ' || k == '\0'))){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'd' && d == 'e' && e == 'f' && f == 'a' && g == 'u' && h == 'l' && i == 't' && (j == ' ' || j == '\0')) || (isFirstChar && c == 'd' && d == 'o' && e == 'u' && f == 'b' && g == 'l' && h == 'e' && (i == ' ' || i == '\0')) || (isFirstChar && c == 'd' && d == 'o' && (e == ' ' || e == '\0'))){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'e' && d == 'l' && e == 's' && f == 'e' && (g == ' ' || g == '\0')) || (isFirstChar && c == 'e' && d == 'n' && e == 'u' && f == 'm' && (g == ' ' || g == '\0')) || (isFirstChar && c == 'e' && d == 'x' && e == 't' && f == 'e' && g == 'r' && h == 'n' && (i == ' ' || i == '\0'))) {
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'f' && d == 'l' && e == 'o' && f == 'a' && g == 't' && (f == ' ' || f == '\0')) || (isFirstChar && c == 'f' && d == 'o' && e == 'r' && (f == ' ' || f == '\0'))){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'g' && d == 'o' && e == 't' && f == 'o' && (g == ' ' && g== '\0'))){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'i' && d == 'f' && (e == ' ' || e == '\0')) || (isFirstChar && c == 'i' && d == 'n' && e == 't' && (f == ' ' || f == '\0'))){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'l' && d == 'o' && e == 'n' && f == 'g' && (g == ' ' || g == '\0'))){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'r' && d == 'e' && e == 'g' && f == 'i' && g == 's' && i == 't' && j == 'e' && k == 'r' && (l == ' ' || l == '\0')) || (isFirstChar && c == 'r' && d == 'e' && e == 't' && f == 'u' && g == 'r' && h == 'n' && (i == ' ' || i == '\0'))) {
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 's' && d == 'h' && e == 'o' && f == 'r' && g == 't' && (h == ' ' || h == '\0')) || (isFirstChar && c == 's' && d == 'i' && e == 'g' && f == 'n' && g == 'e' && h == 'd' && (i == ' ' || i == '\0')) || (isFirstChar && c == 's' && d == 'i' && e == 'z' && f == 'e' && g == 'o' && i == 'f' && (j == ' ' || j == '\0')) || (isFirstChar && c == 's' && d == 't' && e == 'a' && f == 't' && g == 'i' && i == 'c' && (j == ' ' || j == '\0')) || (isFirstChar && c == 's' && d == 't' && e == 'r' && f == 'u' && g == 'c' && i == 't' && (j == ' ' || j == '\0')) || (isFirstChar && c == 's' && d == 'w' && e == 'i' && f == 't' && g == 'c' && i == 'h' && (j == ' ' || j == '\0'))) {
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if (isFirstChar && c == 't' && d == 'y' && e == 'p' && f == 'e' && g == 'd' && h == 'e' && i == 'f' && (j == ' ' || j == '\0')) {
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'u' && d == 'n' && e == 'i' && f == 'o' && g == 'n' && (h == ' ' || h == '\0')) || (isFirstChar && c == 'u' && d == 'n' && e == 's' && f == 'i' && g == 'g' && h == 'n' && i == 'e' && j == 'd' && (k == ' ' || k == '\0'))) {
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if ((isFirstChar && c == 'v' && d == 'o' && e == 'i' && f == 'd' && (g == ' ' || g == '\0')) || (isFirstChar && c == 'v' && d == 'o' && e == 'l' && f == 'a' && g == 't' && h == 'i' && i == 'l' && j == 'e' && (k == ' ' || k == '\0'))) {
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            }else if (isFirstChar && c == 'w' && d == 'h' && e == 'i' && f == 'l' && g == 'e' && (h == ' ' || h == '\0')){
                isFirstChar = 0;
                token = malloc(strlen("DISTINCT C TOKEN \"") + 1);
                strncpy(token, "DISTINCT C TOKEN \"", strlen("DISTINCT C TOKEN \"") + 1);
            */
	    /*Main method for differentiating between Words*/
            if (isFirstChar) {
                isFirstChar = 0;
                token = malloc(strlen("word \"") + 1);
                strncpy(token, "word \"", strlen("word \"") + 1);
            }


            // Append valid character to the token we're building then update the token string.
            char *tmp = charToString(token, c);
            free(token);
            token = malloc(strlen(tmp) + 1);
            strcpy(token, tmp);
            free(tmp);

            tk->cursorPosition++;
        } else {
            break; // Token is malformed.
        }
    }


    // Append closing quotes to the token output
    if (token != NULL) {
        char* tmp = charToString(token, '\"');
        free(token);
        token = malloc(strlen(tmp) + 1);
        strcpy(token, tmp);
        free(tmp);
    }

    return token;
}

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {

	if (ts == NULL) { //  Checks for Null input
        return NULL;
    }

	TokenizerT * retToken;

    if ((retToken = malloc(sizeof(TokenizerT))) == NULL) { //Validates for memory usage, and bad memory.
        return NULL;
    }

    retToken->tokenString = ts;
    retToken->tokenLength = strlen(ts);

    retToken->cursorPosition = 0;
    return retToken;

}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {

	free(tk);
	return;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {

	char *token = wordToken(tk);

    if (token == NULL) {
        token = operatorToken(tk);
    }


    if (token == NULL) {
        token = numToken(tk);
    }

    return token;
}
