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

int lexStr(char *str,Token **headToken,Token **tailToken);

void freeToken(Token *token);
void debugToken(Token *token);


#ifdef __cplusplus
}
#endif

#endif




