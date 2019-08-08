#ifndef VAN_ERRORENUM_H
#define VAN_ERRORENUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// van_error 相关
typedef enum {
	ErrorType_LexError=1, // 词法错误

	ErrorType_ExprSyntaxError, // 语法错误
	ErrorType_AssignSyntaxError,
	ErrorType_ContinueSyntaxError,
	ErrorType_BreakSyntaxError,
	ErrorType_ReturnSyntaxError,
	ErrorType_UpSyntaxError,
	ErrorType_GlobalSyntaxError,
	ErrorType_FieldSyntaxError,
	ErrorType_YieldSyntaxError,
	ErrorType_StaticSyntaxError,
	ErrorType_ImportSyntaxError,
	ErrorType_PrintSyntaxError,

	ErrorType_IfSyntaxError,
	ErrorType_WhileSyntaxError,
	ErrorType_ForSyntaxError,
	ErrorType_FunctionSyntaxError,
	ErrorType_ClassSyntaxError,

	ErrorType_SubscriptError, // 语义错误
	ErrorType_PropertyError,
	ErrorType_CallError,
	ErrorType_NewError,
	ErrorType_OperateError,

} ErrorType;


#ifdef __cplusplus
}
#endif

#endif




