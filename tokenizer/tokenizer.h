#ifndef TOKEN_H
#define TOKEN_H

typedef struct TokenizerT_ {
	const char * tokenString;
	unsigned long cursorPosition;
	unsigned long tokenLength;
}TokenizerT;


char* charToString(const char* a, char c);
char* toString(const char* a, const char* b);
char* numToken(TokenizerT* tk);
char* operatorToken(TokenizerT* tk);
char* wordToken(TokenizerT* tk);
TokenizerT *TKCreate(char* ts);
void TKDestroy(TokenizerT* tk);
char* TKGetNextToken(TokenizerT * tk);

#endif
