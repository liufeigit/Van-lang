#ifndef VAN_PARSE_H
#define VAN_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "van_data.h"
#include "van_lex.h"

typedef struct ASTNode{
	ASTType type;
	ASTOp op;

	union {
		int outFlag;
		long long l;
		double d;
	} value; // int/float 
	char *data; // indentify/string 
	VanWrap *wrap;

	struct ASTNode *parent;
	int childLength;
	struct ASTNode **childArr;

	struct ASTNode *next;

	unsigned int row;
	unsigned int column;

} ASTNode;


int parseToken(Token *token,ASTNode **node);



#ifdef __cplusplus
}
#endif

#endif







