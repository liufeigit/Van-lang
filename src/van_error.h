#ifndef VAN_ERROR_H
#define VAN_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "van_enum.h"
#include "van_lex.h"

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




