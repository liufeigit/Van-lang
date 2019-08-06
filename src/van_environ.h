#ifndef VAN_ENVIRON_H
#define VAN_ENVIRON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "van_data.h"
#include "van_type.h"
#include "van_parse.h"
#include "van_parseUtil.h"

typedef struct {
	VanMap *scopeDict; // var/function/class
	
	VanMap *curScope;
	VanWrap *curBindObj; // self cls/ins
	VanInstance *curIns; // self

	VanList *callStack; // function call stack
	VanList *instanceStack;
	
	VanWrap *returnValue; // return/break/continue
	int isReturn;
	int isBreak;
	int isContinue;

} VanEnviron;

// environ
/***
	environ_setScopeValue
	environ_getScopeValue
	environ_eval
****/
VanEnviron *environ_create();

void environ_addNameSapce(VanEnviron *env,char *str);
void environ_importNameSapce(VanEnviron *env,char *str);

void environ_init(VanEnviron *env);

void environ_initStatic(VanEnviron *env);
void environ_initDynamic(VanEnviron *env);
void environ_initBuiltin(VanEnviron *env);

void environ_brigeNativeFunction(VanEnviron *env,VanFunction *func);
void environ_brigeNativeClass(VanEnviron *env,VanClass *cls);

void environ_eval(VanEnviron *env,ASTNode *astNode);
void environ_evalStr(VanEnviron *env,char *str);

void environ_free(VanEnviron *env);


#ifdef __cplusplus
}
#endif

#endif




