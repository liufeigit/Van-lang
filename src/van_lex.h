#ifndef VAN_LEX_H
#define VAN_LEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "van_enum.h"

typedef struct Token{
	TokenType type;
	TokenSubType subType;

	union {
		long long l;
		double d;
	} value; // int/float 
	char *data; // indentify/string 

	struct Token *next;
	struct Token *pre;

	unsigned int row;
	unsigned int column;

} Token;



int lexStr(char *str,Token **token);
void lexDebug(Token *token);

void freeToken(Token *token);




// van_error 相关
typedef struct ErrorInfor{
	ErrorType type;

	unsigned int row;
	unsigned int column;

	char *data; // indentify

} ErrorInfor;

void error_throwError(ErrorType type,Token *token);



#ifdef __cplusplus
}
#endif

#endif

