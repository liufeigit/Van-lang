#ifndef VAN_LEXUTIL_H
#define VAN_LEXUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "van_lex.h"

Token *filterEmpty(Token *token);

// ()[]{}
Token *__leftRight(Token *token,TokenType type,TokenSubType subType);

/***
	while/for/class/function
****/
Token *__boundExprState(Token *token);
Token *__boundAssignState(Token *token);
Token *__boundIfState(Token *token);
Token *__boundWhileState(Token *token);
Token *__boundForState(Token *token);
Token *__boundFunctionState(Token *token);
Token *__boundContinueState(Token *token);
Token *__boundBreakState(Token *token);
Token *__boundReturnState(Token *token);
Token *__boundClassState(Token *token);
Token *__boundUpState(Token *token);
Token *__boundGlobalState(Token *token);
Token *__boundFieldState(Token *token);
Token *__boundYieldState(Token *token);
Token *__boundImportState(Token *token);
Token *__boundPrintState(Token *token); // 测试

Token *__boundBrace(Token *token); // {}
Token *__boundParenth(Token *token); // ()
Token *__boundBracket(Token *token); // []
Token *__boundSemicolon(Token *token); // ;
Token *__boundIn(Token *token,Token *tail); // in;
Token *__boundAssign(Token *token,Token *tail); // =
Token *__boundComma(Token *token,Token *tail); // ,
Token *__boundColon(Token *token,Token *tail); // :

Token *__boundMapExpr(Token *token);
Token *__boundListExpr(Token *token);
Token *__boundFunctionExpr(Token *token);
Token *__boundClassExpr(Token *token);
Token *__boundMonoExpr(Token *token);

// 0 (,...)[,...]{,...} 1 ,...
int __calCommaCount(Token *start,Token *end,int flag); // ,
int __calMapLength(Token *start,Token *end);
int __calListLength(Token *start,Token *end);
int __calArgumentsLength(Token *start,Token *end);
int __calIfConditionLength(Token *start,Token *end);
int __calIfBlockLength(Token *start,Token *end);

int __isExprState(Token *token);
int __isAssignState(Token *token);
int __isIfState(Token *token);
int __isWhileState(Token *token);
int __isForState(Token *token);
int __isFunctionState(Token *token);
int __isContinueState(Token *token);
int __isBreakState(Token *token);
int __isReturnState(Token *token);
int __isClassState(Token *token);
int __isUpState(Token *token);
int __isGlobalState(Token *token);
int __isFieldState(Token *token);
int __isYieldState(Token *token);
int __isStaticState(Token *token);
int __isImportState(Token *token);
int __isPrintState(Token *token); // 测试

// 单值
int __isIdentifiExpr(Token *token);
int __isNumberExpr(Token *token);
int __isNullExpr(Token *token);
int __isTrueExpr(Token *token);
int __isFalseExpr(Token *token);
int __isSelfExpr(Token *token);
int __isSuperExpr(Token *token);
int __isStringExpr(Token *token);

// 复合
int __isMapExpr(Token *token);
int __isListExpr(Token *token);
int __isFunctionExpr(Token *token);
int __isClassExpr(Token *token);

// 递归
int __isPriorExpr(Token *token);
int __isNegativeExpr(Token *token);
int __isNewExpr(Token *token);
int __isReverseExpr(Token *token);
int __isNotExpr(Token *token);


#ifdef __cplusplus
}
#endif

#endif

