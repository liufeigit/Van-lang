// clang 

#include "van_environ.h"

static void environ_evalExpr(VanEnviron *env,ASTNode *astNode);
static void environ_evalAssign(VanEnviron *env,ASTNode *astNode);
static void environ_evalIf(VanEnviron *env,ASTNode *astNode);
static void environ_evalWhile(VanEnviron *env,ASTNode *astNode);
static void environ_evalFor(VanEnviron *env,ASTNode *astNode);
static void __environ_evalForIn(VanEnviron *env,ASTNode *astNode);
static void __environ_evalForThree(VanEnviron *env,ASTNode *astNode);
static void environ_evalFunction(VanEnviron *env,ASTNode *astNode);
static void environ_evalClass(VanEnviron *env,ASTNode *astNode);

static void environ_evalBlock(VanEnviron *env,ASTNode *astNode);
static void environ_evalComma(VanEnviron *env,ASTNode *astNode);

static void environ_calCurScope(VanEnviron *env,char *name);
static void environ_calWriteScope(VanEnviron *env,char *name);
static void environ_calReadScope(VanEnviron *env,char *name);
static void environ_calCurBindObj(VanEnviron *env);

static int environ_dealJump(VanEnviron *env,ASTNode *astNode);
static void environ_evalContinue(VanEnviron *env,ASTNode *astNode);
static void environ_evalBreak(VanEnviron *env,ASTNode *astNode);
static void environ_evalReturn(VanEnviron *env,ASTNode *astNode);
static void environ_evalYield(VanEnviron *env,ASTNode *astNode);
static void environ_evalUp(VanEnviron *env,ASTNode *astNode);
static void environ_evalGlobal(VanEnviron *env,ASTNode *astNode);
static void environ_evalField(VanEnviron *env,ASTNode *astNode);
static void environ_evalStatic(VanEnviron *env,ASTNode *astNode);
static void environ_evalImport(VanEnviron *env,ASTNode *astNode);
static void environ_evalPrint(VanEnviron *env,ASTNode *astNode); // 测试

static void environ_evalMap(VanEnviron *env,ASTNode *astNode);
static void environ_evalList(VanEnviron *env,ASTNode *astNode);
static void environ_evalFunctionExpr(VanEnviron *env,ASTNode *astNode);
static void environ_evalClassExpr(VanEnviron *env,ASTNode *astNode);

static void environ_evalCall(VanEnviron *env,ASTNode *astNode);
static void environ_evalNew(VanEnviron *env,ASTNode *astNode);
static void environ_evalProperty(VanEnviron *env,ASTNode *astNode);
static void environ_evalSubscript(VanEnviron *env,ASTNode *astNode);

static void environ_evalIndentify(VanEnviron *env,ASTNode *astNode);
static void __environ_evalOutIndentify(VanEnviron *env,ASTNode *astNode);
static void environ_evalNumber(VanEnviron *env,ASTNode *astNode);
static void environ_evalNull(VanEnviron *env,ASTNode *astNode);
static void environ_evalTrue(VanEnviron *env,ASTNode *astNode);
static void environ_evalFalse(VanEnviron *env,ASTNode *astNode);
static void environ_evalSelf(VanEnviron *env,ASTNode *astNode);
static void environ_evalSuper(VanEnviron *env,ASTNode *astNode);
static void environ_evalString(VanEnviron *env,ASTNode *astNode);


// binary 相关
static void environ_evalBinary(VanEnviron *env,ASTNode *astNode);
static void environ_evalUnCome(VanEnviron *env,ASTNode *astNode);

static void environ_evalPrior(VanEnviron *env,ASTNode *astNode);
static void environ_evalNot(VanEnviron *env,ASTNode *astNode);
static void environ_evalReverse(VanEnviron *env,ASTNode *astNode);
static void environ_evalNegative(VanEnviron *env,ASTNode *astNode);

static void __environ_evalFunction(VanEnviron *env,ASTNode *astNode);
static void __environ_evalClass(VanEnviron *env,ASTNode *astNode);

VanEnviron *environ_create(){
	VanEnviron *env=NULL;
	VanMap *scopeDict=NULL;
	VanList *callStack=NULL;

	env=(VanEnviron *)calloc(1, sizeof(VanEnviron ));
	scopeDict=map_create();
	callStack=list_create(0);

	env->scopeDict=scopeDict;
	env->curScope=scopeDict;
	env->callStack=callStack;

	environ_init(env);

	return env;
}

void environ_evalStr(VanEnviron *env,char *str){


}

// 内置如List/Map的初始化
void environ_init(VanEnviron *env){

	environ_initBuiltin(env);
	environ_initDynamic(env);

}

void environ_eval(VanEnviron *env,ASTNode *astNode){
	ASTNode *parent=NULL;

	while(astNode){
		parent=astNode->parent;

		if(environ_dealJump(env,astNode)){
			break;
		}

		if(astNode->type==ASTType_Expr){
			environ_evalExpr(env,astNode);
		}
		else if(astNode->type==ASTType_Assign){
			environ_evalAssign(env,astNode);
		}
		else if(astNode->type==ASTType_If){
			environ_evalIf(env,astNode);
		}
		else if(astNode->type==ASTType_While){
			environ_evalWhile(env,astNode);
		}
		else if(astNode->type==ASTType_For){
			environ_evalFor(env,astNode);
		}
		else if(astNode->type==ASTType_Function){
			environ_evalFunction(env,astNode);
		}
		else if(astNode->type==ASTType_Class){
			environ_evalClass(env,astNode);
		}
		else if(astNode->type==ASTType_Continue){
			environ_evalContinue(env,astNode);
		}
		else if(astNode->type==ASTType_Break){
			environ_evalBreak(env,astNode);
		}
		else if(astNode->type==ASTType_Return){ // 函数执行
			environ_evalReturn(env,astNode);
		}
		else if(astNode->type==ASTType_Up){
			environ_evalUp(env,astNode);
		}
		else if(astNode->type==ASTType_Global){
			environ_evalGlobal(env,astNode);
		}
		else if(astNode->type==ASTType_Field){
			environ_evalField(env,astNode);
		}
		else if(astNode->type==ASTType_Static){
			environ_evalStatic(env,astNode);
		}
		else if(astNode->type==ASTType_Import){
			environ_evalImport(env,astNode);
		}
		else if(astNode->type==ASTType_Print){
			environ_evalPrint(env,astNode);
		}
		else if(astNode->type==ASTType_Block){
			environ_evalBlock(env,astNode);
		}
		else{ // error
			break;
		}

		if(environ_dealJump(env,astNode)){
			break;
		}

		astNode=astNode->next;
	}

}

static int environ_dealJump(VanEnviron *env,ASTNode *astNode){
	int flag=0;

	if(env->isReturn){
		if(astNode->parent->op==ASTOp_FunctionBlock){

			astNode->parent->wrap=env->returnValue;
			env->isReturn=0;
		}
		flag=1;
	}

	if(env->isBreak){
		flag=1;
	}

	if(env->isContinue){
		flag=1;
	}

	return flag;
}

static void environ_evalExpr(VanEnviron *env,ASTNode *astNode){
	ASTOp op=0;

	op=astNode->op;
	if(op==ASTOp_Indentify){
		environ_evalIndentify(env,astNode);
	}
	else if(op==ASTOp_Int||op==ASTOp_Float){
		environ_evalNumber(env,astNode);
	}
	else if(op==ASTOp_Null){
		environ_evalNull(env,astNode);
	}
	else if(op==ASTOp_True){
		environ_evalTrue(env,astNode);
	}
	else if(op==ASTOp_False){
		environ_evalFalse(env,astNode);
	}
	else if(op==ASTOp_Self){
		environ_evalSelf(env,astNode);
	}
	else if(op==ASTOp_Super){
		environ_evalSuper(env,astNode);
	}
	else if(op==ASTOp_String){
		environ_evalString(env,astNode);
	}
	else if(op==ASTOp_Map){
		environ_evalMap(env,astNode);
	}
	else if(op==ASTOp_List){
		environ_evalList(env,astNode);
	}
	else if(op==ASTOp_FunctionExpr){
		environ_evalFunctionExpr(env,astNode);
	}
	else if(op==ASTOp_ClassExpr){
		environ_evalClassExpr(env,astNode);
	}
	else if(op==ASTOp_Prior){
		environ_evalPrior(env,astNode);
	}
	else if(op==ASTOp_Call){
		environ_evalCall(env,astNode);
	}
	else if(op==ASTOp_Property){
		environ_evalProperty(env,astNode);
	}
	else if(op==ASTOp_Subscript){
		environ_evalSubscript(env,astNode);
	}
	else if(op==ASTOp_Not){
		environ_evalNot(env,astNode);
	}
	else if(op==ASTOp_Reverse){
		environ_evalReverse(env,astNode);
	}
	else if(op==ASTOp_New){
		environ_evalNew(env,astNode);
	}
	else if(op==ASTOp_Negative){
		environ_evalNegative(env,astNode);
	}
	else if(op>ASTOp_BinaryStart&&op<ASTOp_BinaryEnd){
		environ_evalBinary(env,astNode);
	}

}

static void environ_evalComma(VanEnviron *env,ASTNode *astNode){
	ASTNode **nodeArr=NULL;
	int length=0;

	nodeArr=astNode->childArr;
	length=astNode->childLength;
	for(int i=0;i<length;i++){
		environ_evalExpr(env, nodeArr[i]);
	}
}

static void environ_evalAssign(VanEnviron *env,ASTNode *astNode){
	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;

	ASTNode **nodeArr1=NULL;
	ASTNode **nodeArr2=NULL;

	int length1=0;
	int length2=0;

	ASTNode *leftNode1=NULL;
	ASTNode *rightNode1=NULL;

	leftNode=astNode->childArr[0];
	rightNode=astNode->childArr[1];
	nodeArr1=leftNode->childArr;
	nodeArr2=rightNode->childArr;

	length1=leftNode->childLength;
	length2=rightNode->childLength;

	for(int i=0;i<length2;i++){
		rightNode=nodeArr2[i];
		environ_evalExpr(env, rightNode);
	}

	for(int i=0;i<length1;i++){
		leftNode=nodeArr1[i];
		rightNode=nodeArr2[i<length2?i:length2-1];
		
		// environ_evalExpr(env, rightNode);
		if(leftNode->op==ASTOp_Indentify){
			environ_calWriteScope(env, leftNode->data);
			map_setValue(env->curScope, leftNode->data, rightNode->wrap); // ??? value|reference
		}
		else if(leftNode->op==ASTOp_Property){ // Map|Instance|Class|Function
			leftNode1=leftNode->childArr[0];
			rightNode1=leftNode->childArr[1];

			environ_evalExpr(env, leftNode1);
			if(leftNode1->wrap->type==DataType_Map){ // map
				map_setValue(leftNode1->wrap->data, rightNode1->data, rightNode->wrap);
			}
			else if(leftNode1->wrap->type==DataType_Instance){ // instance
				instance_setValue(leftNode1->wrap->data, rightNode1->data, rightNode->wrap);
			}
			else if(leftNode1->wrap->type==DataType_Class){ // class
				class_setValue(leftNode1->wrap->data, rightNode1->data, rightNode->wrap);
			}
			else if(leftNode1->wrap->type==DataType_Function){ // function
				function_setValue(leftNode1->wrap->data, rightNode1->data, rightNode->wrap);
			}
		}
		else if(leftNode->op==ASTOp_Subscript){ // Map/List
			leftNode1=leftNode->childArr[0];
			rightNode1=leftNode->childArr[1];

			environ_evalExpr(env, leftNode1);
			environ_evalExpr(env, rightNode1);
			if(leftNode1->wrap->type==DataType_Map){ // map
				if(rightNode1->wrap->type==DataType_String){
					map_setValue(leftNode1->wrap->data, rightNode1->data, rightNode->wrap);
				}
			}
			else if(leftNode1->wrap->type==DataType_List){ // list
				if(rightNode1->wrap->type==DataType_Int){
					list_setValue(leftNode1->wrap->data, rightNode1->wrap->value.l, rightNode->wrap);
				}

			}
		}
	}
}

static void environ_evalIf(VanEnviron *env,ASTNode *astNode){
	int blockLen=0;
	ASTNode **nodeArr=NULL;

	ASTNode *termNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;

	VanWrap *exprWrap=NULL;
	int flag=0;

	nodeArr=astNode->childArr;
	blockLen=astNode->childLength;
	for(int i=0;i<(blockLen>1?blockLen-1:blockLen);i++){
		termNode=nodeArr[i];
		exprNode=termNode->childArr[0];
		blockNode=termNode->childArr[1];

		environ_evalExpr(env, exprNode);
		exprWrap=exprNode->wrap;
		if(exprWrap->type==DataType_Int||exprWrap->type==DataType_Float){
			if(exprWrap->value.l){
				environ_evalBlock(env, blockNode);

				flag=1;
				break;
			}
		}
	}

	if(!flag){
		termNode=nodeArr[blockLen-1];
		if(termNode->type==ASTType_Block){ // else
			blockNode=termNode;
			environ_evalBlock(env, blockNode);
		}
		else if(termNode->op==ASTOp_ElseIfTerm){
			exprNode=termNode->childArr[0];
			blockNode=termNode->childArr[1];

			environ_evalExpr(env, exprNode);
			exprWrap=exprNode->wrap;
			if(exprWrap->type==DataType_Int||exprWrap->type==DataType_Float){
				if(exprWrap->value.l){
					environ_evalBlock(env, blockNode);
				}
			}
		}
	}

}

static void environ_evalWhile(VanEnviron *env,ASTNode *astNode){
	ASTNode *termNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;

	VanWrap *exprWrap=NULL;

	termNode=astNode->childArr[0];
	exprNode=termNode->childArr[0];
	blockNode=astNode->childArr[1];

	environ_evalExpr(env, exprNode);
	exprWrap=exprNode->wrap;

	if(exprWrap->type==DataType_Int||exprWrap->type==DataType_Float){
		while(exprWrap->value.l){

			if(!env->isBreak){
				environ_evalBlock(env, blockNode);

				environ_evalExpr(env, exprNode);
				exprWrap=exprNode->wrap;
				env->isContinue=0;
			}
			else{
				break;
			}
		}

		env->isBreak=0;
		env->isContinue=0;
	}
}

static void __environ_evalForThree(VanEnviron *env,ASTNode *astNode){
	ASTNode *termNode=NULL;
	ASTNode *startNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *endNode=NULL;
	ASTNode *blockNode=NULL;

	VanWrap *exprWrap=NULL;

	termNode=astNode->childArr[0];
	startNode=termNode->childArr[0];
	exprNode=termNode->childArr[1];
	endNode=termNode->childArr[2];
	blockNode=astNode->childArr[1];

	environ_evalAssign(env, startNode);
	environ_evalExpr(env, exprNode);
	exprWrap=exprNode->wrap;

	if(exprWrap->type==DataType_Int||exprWrap->type==DataType_Float){
		while(exprWrap->value.l){

			if(!env->isBreak){
				environ_evalBlock(env, blockNode);

				environ_evalAssign(env, endNode);
				environ_evalExpr(env, exprNode);
				exprWrap=exprNode->wrap;
				env->isContinue=0;
			}
			else{
				break;
			}
		}

		env->isBreak=0;
		env->isContinue=0;
	}

}

static void __environ_evalForIn(VanEnviron *env,ASTNode *astNode){
	ASTNode *termNode=NULL;
	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;
	ASTNode *blockNode=NULL;

	ASTNode **leftArr=NULL;
	ASTNode **rightArr=NULL;
	ASTNode *_right=NULL;
	ASTNode *_left=NULL;

	int length=0;

	termNode=astNode->childArr[0];
	leftNode=termNode->childArr[0];
	rightNode=termNode->childArr[1];
	blockNode=astNode->childArr[1];

	leftArr=leftNode->childArr;
	rightArr=rightNode->childArr;
	length=leftNode->childLength;

	environ_evalComma(env, rightNode);
	_right=rightArr[0];

	if(_right->wrap->type==DataType_List){ // List 
		int _length=0;
		VanList *_list=NULL;
		VanList *_rList=NULL;

		_list=_right->wrap->data;
		_length=0; // list_length(_list) 
		for(int i=0;i<list_length(_list);i++){
			int _flag=0;

			if(!env->isBreak){ 
				for(int j=0;j<length;j++){
					_left=leftArr[j];
					_right=rightArr[j];

					_rList=_right->wrap->data;

					if(i>=list_length(_rList)){
						_flag=1;
						break;
					}

					environ_calWriteScope(env, _left->data);
					map_setValue(env->curScope, _left->data, list_getValue(_rList, i));
				}
				
				if(_flag){
					break;
				}

				environ_evalBlock(env, blockNode);
				env->isContinue=0;
			}
			else{
				break;
			}
		}
	}

	env->isBreak=0;
	env->isContinue=0;
}

static void environ_evalFor(VanEnviron *env,ASTNode *astNode){
	ASTNode *termNode=NULL;
	int length=0;

	termNode=astNode->childArr[0];
	length=termNode->childLength;
	if(length==2){
		__environ_evalForIn(env,astNode);
	}
	else if(length==3){
		__environ_evalForThree(env,astNode);
	}
}

static void environ_evalFunction(VanEnviron *env,ASTNode *astNode){

	__environ_evalFunction(env,astNode);
}

static void environ_evalClass(VanEnviron *env,ASTNode *astNode){

	__environ_evalClass(env,astNode);
}

static void environ_evalBlock(VanEnviron *env,ASTNode *astNode){

	if(astNode->childLength){
		environ_eval(env,astNode->childArr[0]);
	}
}

// ??? 
static void environ_evalContinue(VanEnviron *env,ASTNode *astNode){
	ASTNode *parent=NULL;

	parent=astNode->parent;
	while(parent){
		if(parent->op==ASTOp_WhileBlock||
			parent->op==ASTOp_ForBlock){
			env->isContinue=1;
			break;
		}
		else if(parent->op==ASTOp_FunctionBlock||
			parent->op==ASTOp_ClassBlock){
			break;
		}

		parent=parent->parent;
	}

}

static void environ_evalBreak(VanEnviron *env,ASTNode *astNode){
	ASTNode *parent=NULL;

	parent=astNode->parent;
	while(parent){
		if(parent->op==ASTOp_WhileBlock||
			parent->op==ASTOp_ForBlock){
			env->isBreak=1;
			break;
		}
		else if(parent->op==ASTOp_FunctionBlock||
			parent->op==ASTOp_ClassBlock){
			break;
		}

		parent=parent->parent;
	}

}

static void environ_evalReturn(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	ASTNode *exprNode=NULL;

	ASTNode *parent=NULL;
	int flag=0;

	parent=astNode->parent;
	while(parent){
		if(parent->op==ASTOp_FunctionBlock){
			flag=1;
			break;
		}
		else if(parent->op==ASTOp_ClassBlock){
			break;
		}

		parent=parent->parent;
	}

	if(flag){
		exprNode=astNode->childArr[0];
		environ_evalExpr(env, exprNode);

		wrap=exprNode->wrap;
		astNode->wrap=wrap;

		env->returnValue=wrap;

		env->isReturn=1;
	}
}

static void environ_evalYield(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	ASTNode *exprNode=NULL;

	ASTNode *parent=NULL;
	int flag=0;

	parent=astNode->parent;
	while(parent){
		if(parent->op==ASTOp_FunctionBlock){
			flag=1;
			break;
		}
		else if(parent->op==ASTOp_ClassBlock){
			break;
		}

		parent=parent->parent;
	}

	if(flag){
		exprNode=astNode->childArr[0];
		environ_evalExpr(env, exprNode);

		wrap=exprNode->wrap;
		astNode->wrap=wrap;

		env->returnValue=wrap;

		env->isReturn=1;
	}
}

static void environ_evalUp(VanEnviron *env,ASTNode *astNode){
	int length=0;
	ASTNode **nodeArr=NULL;

	ASTNode *parent=NULL;
	VanMap *upScopeDict=NULL;
	VanFunction *function=NULL;

	length=astNode->childLength;
	nodeArr=astNode->childArr;

	parent=astNode->parent;
	while(parent){
		if(parent->op==ASTOp_FunctionBlock){
			if(length){
				int _len=0;

				_len=list_length(env->callStack);
				function=list_getValue(env->callStack, _len-1)->data;
				upScopeDict=function->upScopeDict;
				for(int i=0;i<length;i++){

					map_setValue(upScopeDict, nodeArr[i]->data, NULL);
				}
			}
			
			break;
		}

		parent=parent->parent;
	}
}

static void environ_evalGlobal(VanEnviron *env,ASTNode *astNode){
	int length=0;
	ASTNode **nodeArr=NULL;

	ASTNode *parent=NULL;
	VanMap *globalScopeDict=NULL;
	VanFunction *function=NULL;

	length=astNode->childLength;
	nodeArr=astNode->childArr;

	parent=astNode->parent;
	while(parent){
		if(parent->op==ASTOp_FunctionBlock){
			if(length){
				int _len=0;

				_len=list_length(env->callStack);
				function=list_getValue(env->callStack, _len-1)->data;
				globalScopeDict=function->globalScopeDict;
				for(int i=0;i<length;i++){
					
					map_setValue(globalScopeDict, nodeArr[i]->data, NULL);
				}
			}
			
			break;
		}

		parent=parent->parent;
	}
}

static void environ_evalField(VanEnviron *env,ASTNode *astNode){
	int length=0;
	ASTNode **nodeArr=NULL;

	ASTNode *parent=NULL;

	length=astNode->childLength;
	nodeArr=astNode->childArr;

	parent=astNode->parent;
	if(parent){
		VanClass *cls=NULL;

		if(parent->op==ASTOp_ClassBlock){ // ins

			if(length){
				cls=parent->parent->wrap->data;
				for(int i=0;i<length;i++){
					class_addInsField(cls, nodeArr[i]->data,NULL);
				}
			}
		}
		else if(parent->type==ASTType_Static){ // cls

			if(length){
				cls=parent->parent->parent->wrap->data;
				for(int i=0;i<length;i++){
					class_addClsField(cls, nodeArr[i]->data,NULL);
				}
			}
		}
	}
}

static void environ_evalStatic(VanEnviron *env,ASTNode *astNode){
	ASTNode *element=NULL;

	element=astNode->childArr[0];
	if(element->type==ASTType_Function){
		environ_evalFunction(env, element);
	}
	else if(element->type==ASTType_Field){
		environ_evalField(env, element);
	}

}

static void environ_evalImport(VanEnviron *env,ASTNode *astNode){


}

static void environ_evalPrint(VanEnviron *env,ASTNode *astNode){
	ASTNode **nodeArr=NULL;
	ASTNode *element=NULL;
	char *str=NULL;

	int length=0;

	nodeArr=(ASTNode **)astNode->childArr;
	length=astNode->childLength;
	for(int i=0;i<length;i++){
		element=nodeArr[i];
		environ_evalExpr(env, element);
		str=wrap_toString(element->wrap);
		printf("%s",str);
	}
	printf("\n");

}


// expr相关
static void environ_evalMap(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;

	VanMap *map=NULL;
	ASTNode *pair=NULL;
	ASTNode *key=NULL;
	ASTNode *expr=NULL;
	int length=0;

	length=astNode->childLength;
	map=map_create();
	for(int i=0;i<length;i++){
		char *str=NULL;
		int len=0;

		pair=astNode->childArr[i];
		key=pair->childArr[0];
		expr=pair->childArr[1];

		len=strlen(key->data)+1;
		str=(char *)calloc(len, sizeof(char ));
		strcpy(str,key->data);

		environ_evalExpr(env, expr);

		map_setValue(map, str, expr->wrap);
	}

	wrap=wrap_wrapMap(map);
	astNode->wrap=wrap;
}

static void environ_evalList(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;

	VanList *list=NULL;
	ASTNode *node=NULL;
	int length=0;

	length=astNode->childLength;
	list=list_create(0);
	for(int i=0;i<length;i++){
		node=astNode->childArr[i];
		environ_evalExpr(env, node);
		list_push(list, node->wrap);
	}

	wrap=wrap_wrapList(list);
	astNode->wrap=wrap;
}

static void __environ_evalFunction(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	VanFunction *function=NULL;

	char *name=NULL;
	char **argNameArr=NULL;
	int *outFlagArr=NULL;
	int argArrLength=0;

	ASTNode *nameNode=NULL;
	ASTNode *argNode=NULL;
	ASTNode *blockNode=NULL;

	VanList *chainScopeList=NULL;
	int length=0;

	nameNode=astNode->childArr[0];
	argNode=astNode->childArr[1];
	blockNode=astNode->childArr[2];

	if(nameNode){
		if(nameNode->data){
			name=(char *)calloc(strlen(nameNode->data)+1, sizeof(char ));
			strcpy(name,nameNode->data);
		}
	}

	if(argNode){
		if(argNode->childLength){
			argArrLength=argNode->childLength;
			argNameArr=(char **)calloc(argArrLength, sizeof(char *));
			outFlagArr=(int *)calloc(argArrLength, sizeof(int ));
			for(int i=0;i<argArrLength;i++){
				ASTNode *_node=NULL;

				_node=argNode->childArr[i];
				argNameArr[i]=(char *)calloc(strlen(_node->data)+1, sizeof(char ));
				strcpy(argNameArr[i],_node->data);

				if(_node->value.outFlag){
					outFlagArr[i]=1;
				}
			}
		}
	}

	function=function_create(name, argNameArr,outFlagArr, argArrLength);
	function->code=blockNode;

	chainScopeList=list_create(0);
	length=list_length(env->callStack);
	for(int i=0;i<length;i++){
		VanFunction *_func=NULL;
		VanWrap *_wrap=NULL;

		_func=list_getValue(env->callStack, i)->data;
		_wrap=wrap_wrapMap(_func->inScopeDict);
		list_push(chainScopeList, _wrap);
	}
	function->chainScopeList=chainScopeList;

	wrap=wrap_wrapFunction(function);
	if(name){
		ASTNode *parent=NULL;
		int flag=1;

		parent=astNode->parent;
		if(parent){
			VanClass *cls=NULL;
			if(parent->op==ASTOp_ClassBlock){
				
				cls=parent->parent->wrap->data;
				class_addInsField(cls, function->name,wrap);

				flag=0;
			}
			else if(parent->type==ASTType_Static){

				cls=parent->parent->parent->wrap->data;
				class_addClsField(cls, function->name,wrap);

				flag=0;
			}
		}

		// while(parent){
		// 	if(parent->op==ASTOp_ClassBlock){
		// 		VanClass *cls=NULL;

		// 		cls=parent->parent->wrap->data;
		// 		class_addInsField(cls, function->name,wrap);

		// 		flag=0;
		// 		break;
		// 	}
		// 	else if(parent->op==ASTOp_FunctionBlock){
		// 		break;
		// 	}

		// 	parent=parent->parent;
		// }

		if(flag){
			environ_calWriteScope(env, name);
			map_setValue(env->curScope, name, wrap);
		}
	}

	astNode->wrap=wrap;
}

// function-->call
static void environ_evalFunctionExpr(VanEnviron *env,ASTNode *astNode){
	
	__environ_evalFunction(env,astNode);
}

static void __environ_evalClass(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	VanClass *cls=NULL;

	char *name=NULL;
	VanClass *base=NULL; // class

	ASTNode *nameNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;

	nameNode=astNode->childArr[0];
	exprNode=astNode->childArr[1];
	blockNode=astNode->childArr[2];

	if(nameNode){
		if(nameNode->data){
			name=(char *)calloc(strlen(nameNode->data)+1, sizeof(char ));
			strcpy(name,nameNode->data);
		}
	}

	if(exprNode){
		environ_evalExpr(env, exprNode);
		if(exprNode->wrap->type==DataType_Class){
			base=exprNode->wrap->data;
		}
	}

	cls=class_create(name, base);
	wrap=wrap_wrapClass(cls);
	astNode->wrap=wrap;

	environ_evalBlock(env, blockNode);
	if(name){
		environ_calWriteScope(env, name);
		map_setValue(env->curScope, name, wrap);
	}
}

// class-->new
static void environ_evalClassExpr(VanEnviron *env,ASTNode *astNode){
	
	__environ_evalClass(env,astNode);
}

static void environ_evalCall(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;

	VanFunction *function=NULL;
	VanClass *cls=NULL;
	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;
	ASTNode *codeNode=NULL;

	VanList *argList=NULL;
	ASTNode **argArr=NULL;
	int length=0;
	int flag=0;

	VanMap *upScopeDict=NULL;
	VanMap *globalScopeDict=NULL;
	VanMap *inScopeDict=NULL;

	leftNode=astNode->childArr[0];
	rightNode=astNode->childArr[1];

	environ_evalExpr(env, leftNode);
	if(leftNode->wrap){

		flag=1;
		if(leftNode->wrap->type==DataType_Function||
			leftNode->wrap->type==DataType_Class){

			VanClass *_cls=NULL;

			if(leftNode->wrap->type==DataType_Class){
				if(!astNode_hasNewParent(astNode)){
					printf(">>> throw NewError\n");
					exit(0);
				}

				_cls=leftNode->wrap->data;
				if(_cls->type==ClassType_NativeClass){ // __new__
					VanWrap *_value=NULL;
					int _flag=0;

					_value=class_getValue(_cls, "__new__", 0);
					if(_value){
						if(_value->type==DataType_Function){
							function=_value->data;
							_flag=1;
						}
						
					}

					if(!_flag){
						printf(">>> throw CallError\n");
						exit(0);
					}
				}
				else{ // __ctor__
					VanInstance *_ins=NULL;
					VanWrap *_value=NULL;
					int _flag=0;

					wrap=instance_create(_cls,NULL);
					_ins=wrap->data;

					_value=instance_getValue(_ins, "__ctor__", 0);
					if(_value){
						if(_value->type==DataType_Function){
							function=_value->data;
							_flag=1;
						}
						
					}
					
					if(!_flag){
						astNode->wrap=wrap;

						return ;
					}
				}
			}
			else{
				function=leftNode->wrap->data;
			}

			codeNode=function->code;

			length=rightNode->childLength;
			argArr=(ASTNode **)rightNode->childArr;

			upScopeDict=map_create();
			globalScopeDict=map_create();
			inScopeDict=map_create();
			if(length){
				argList=list_create(0);
				for(int i=0;i<length;i++){
					// env->curScope=env->scopeDict;
					// environ_evalExpr(env, argArr[i]);
					// env->curScope=function->inScopeDict;
					// map_setValue(env->curScope, function->argNameArr[i], argArr[i]->wrap);

					if(i<function->argArrLength){
						if(argArr[i]->op==ASTOp_Indentify){
							if(!argArr[i]->value.outFlag&&!function->outFlagArr[i]){
								environ_evalExpr(env, argArr[i]);
								map_setValue(inScopeDict, function->argNameArr[i], argArr[i]->wrap);
								list_push(argList, argArr[i]->wrap);
							}
							else if(argArr[i]->value.outFlag&&function->outFlagArr[i]){
								__environ_evalOutIndentify(env,argArr[i]);
								map_setRefValue(inScopeDict, function->argNameArr[i], argArr[i]->wrap);
								list_push(argList, argArr[i]->wrap);
							}
							else{
								printf(">>> throw CallError\n");
								exit(0);
							}
						}
						else{
							environ_evalExpr(env, argArr[i]);
							map_setValue(inScopeDict, function->argNameArr[i], argArr[i]->wrap);
							list_push(argList, argArr[i]->wrap);
						}	
					}
					else{
						environ_evalExpr(env, argArr[i]);
						list_push(argList, argArr[i]->wrap);
					}
				}
			}

			if(leftNode->wrap->type==DataType_Class){ // __new__
				cls=leftNode->wrap->data;
				wrap=instance_create(cls,argList);
				astNode->wrap=wrap;

				return ;
			}

			if(function->type==FunctionType_Normal){
				function->upScopeDict=upScopeDict;
				function->globalScopeDict=globalScopeDict;
				function->inScopeDict=inScopeDict;
				function->argList=argList;

				list_push(env->callStack, leftNode->wrap); // push 

				// if(function->bindObj){
				// 	env->curIns=function->bindObj->data;
				// }

				// env->curScope=function->inScopeDict;
				// environ_evalBlock(env,codeNode);
				// env->curScope=env->scopeDict;

				environ_evalBlock(env,codeNode);

				if(leftNode->wrap->type==DataType_Function){ // not class __ctor__
					wrap=codeNode->wrap; // ??? is null
				}

				list_pop(env->callStack); // pop
			}
			else{ // NativeFunction
				WrapMethod _method=NULL;

				_method=function->code;
				wrap=_method(function->bindObj,argList);
			}
			
		}
		else if(leftNode->wrap->type==DataType_Class){
			cls=leftNode->wrap->data;

			length=rightNode->childLength;
			argArr=(ASTNode **)rightNode->childArr;

			wrap=instance_create(cls,NULL);
		}
		else if(leftNode->wrap->type==DataType_NativeFunction){
			// ??? 原生方法
			VanNativeFunction *_func=NULL;
			WrapMethod _method=NULL;
			VanList *_argList=NULL;

			length=rightNode->childLength;
			argArr=(ASTNode **)rightNode->childArr;

			_func=leftNode->wrap->data;
			_method=_func->method;

			_argList=list_create(0);
			for(int i=0;i<length;i++){
				environ_evalExpr(env, argArr[i]);
				list_push(_argList, argArr[i]->wrap);
			}

			wrap=_method(NULL,_argList);
		}
		else{
			flag=0;
		}
	}

	if(!flag){
		printf(">>> throw CallError\n");
		exit(0);
	}

	astNode->wrap=wrap;
}

// new A; new A()==> A() evalCall
static void environ_evalNew(VanEnviron *env,ASTNode *astNode){
	ASTNode *node=NULL;
	VanWrap *wrap=NULL;

	node=astNode->childArr[0];
	environ_evalExpr(env, node);

	if(node->wrap){
		if(node->wrap->type==DataType_Class){
			wrap=instance_create(node->wrap->data,NULL);
		}
		else if(astNode_hasClassChild(astNode)){
			wrap=node->wrap;
		}
	}
	
	if(!wrap){
		printf(">>> throw NewError\n");
		exit(0);
	}

	astNode->wrap=wrap;
}

// instance 
static void environ_evalProperty(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	VanWrap *leftWrap=NULL;

	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;
	int flag=0;

	leftNode=astNode->childArr[0];
	rightNode=astNode->childArr[1];

	environ_evalExpr(env, leftNode);
	leftWrap=leftNode->wrap;
	if(leftWrap){

		flag=1;
		if(leftWrap->type==DataType_Instance){ // instance
			wrap=instance_getValue(leftWrap->data, rightNode->data,leftWrap->value.l);
		}
		else if(leftWrap->type==DataType_Class){ // class
			wrap=class_getValue(leftWrap->data, rightNode->data,leftWrap->value.l);
		}
		else if(leftWrap->type==DataType_Map){ // map
			wrap=map_getValue(leftWrap->data, rightNode->data);
		}
		else if(leftWrap->type==DataType_List){ // List ???
			VanClass *_cls=NULL;
			VanFunction *_func=NULL;

			_cls=map_getValue(env->scopeDict, "List")->data;
			wrap=map_getValue(_cls->insDescDict, rightNode->data);
			if(wrap->type==DataType_Function){
				function_bind(wrap->data, leftWrap);
				_func=wrap->data;

				if(wrap->_fieldDesc==FieldDesc_IsGet){

				}
			}
			// wrap=map_getValue(leftWrap->data, rightNode->data);
		}
		else if(leftWrap->type==DataType_Function){ // function
			wrap=function_getValue(leftWrap->data, rightNode->data);
		}
		else{
			flag=0;
		}
	}

	if(!flag){
		printf(">>> throw PropertyError\n");
		exit(0);
	}

	astNode->wrap=wrap;
}

// list/map
static void environ_evalSubscript(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	VanWrap *leftWrap=NULL;
	VanWrap *rightWrap=NULL;

	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;
	int flag=0;

	leftNode=astNode->childArr[0];
	rightNode=astNode->childArr[1];

	environ_evalExpr(env, leftNode);
	environ_evalExpr(env, rightNode);
	leftWrap=leftNode->wrap;
	rightWrap=rightNode->wrap;

	if(leftWrap){
		if(leftWrap->type==DataType_List){ // List
			if(rightWrap->type==DataType_Int){
				wrap=list_getValue(leftWrap->data, rightWrap->value.l);
				flag=1;
			}
		}
		else if(leftWrap->type==DataType_Map){ // Map
			if(rightWrap->type==DataType_String){
				VanString *_vStr=NULL;

				_vStr=rightWrap->data;
				wrap=map_getValue(leftWrap->data, _vStr->data);
				flag=1;
			}
		}
		else if(leftWrap->type==DataType_Instance){ // Instance

		}
		else if(leftWrap->type==DataType_Class){ // Class
			
		}
		else if(leftWrap->type==DataType_Function){ // Function
			
		}
	}

	if(!flag){
		printf(">>> throw SubscriptError\n");
		exit(0);
	}

	astNode->wrap=wrap;
}

/***
	inScopeDict==>cur function
	upScopeDict==>pre function
	globalScopeDict==> global
	global>up>in 
****/
static void environ_calCurScope(VanEnviron *env,char *name){
	
}

// write global declare->up declare->cur
static void environ_calWriteScope(VanEnviron *env,char *name){
	int length=0;
	VanFunction *function=NULL;
	VanMap *curScope=NULL;
	int flag=0;

	VanMap *upScopeDict=NULL;
	VanMap *globalScopeDict=NULL;
	VanMap *inScopeDict=NULL;

	VanList *chainScopeList=NULL;

	length=list_length(env->callStack);
	if(length){
		function=list_getValue(env->callStack, length-1)->data;
		globalScopeDict=function->globalScopeDict;
		upScopeDict=function->upScopeDict;
		inScopeDict=function->inScopeDict;
		chainScopeList=function->chainScopeList;

		// 1. global declare
		flag=map_hasKey(globalScopeDict, name);
		if(flag){
			curScope=env->scopeDict;
		}

		// 2. up declare
		if(!flag){
			flag=map_hasKey(upScopeDict, name);
			if(flag){
				VanMap *_inScopeDict=NULL;

				length=list_length(function->chainScopeList);
				for(int i=length-1;i>=0;i--){
					_inScopeDict=list_getValue(chainScopeList, i)->data;
					flag=map_hasKey(_inScopeDict, name);
					if(flag){
						curScope=_inScopeDict;
						break;
					}
				}
			}
		}

		// cur
		if(!flag){
			curScope=inScopeDict;
			flag=1;
		}
	}
	
	if(!flag){
		curScope=env->scopeDict;
	}

	env->curScope=curScope;
}

// read global declare->up declare->cur->up->global
static void environ_calReadScope(VanEnviron *env,char *name){
	int length=0;
	VanFunction *function=NULL;
	VanMap *curScope=NULL;
	int flag=0;

	VanMap *upScopeDict=NULL;
	VanMap *globalScopeDict=NULL;
	VanMap *inScopeDict=NULL;

	VanList *chainScopeList=NULL;

	length=list_length(env->callStack);
	if(length){
		function=list_getValue(env->callStack, length-1)->data;
		globalScopeDict=function->globalScopeDict;
		upScopeDict=function->upScopeDict;
		inScopeDict=function->inScopeDict;
		chainScopeList=function->chainScopeList;

		// 1. global declare
		flag=map_hasKey(globalScopeDict, name);
		if(flag){
			curScope=env->scopeDict;
		}

		// 2. up declare
		if(!flag){
			flag=map_hasKey(upScopeDict, name);
			if(flag){
				VanMap *_inScopeDict=NULL;

				length=list_length(function->chainScopeList);
				for(int i=length-1;i>=0;i--){
					_inScopeDict=list_getValue(chainScopeList, i)->data;
					flag=map_hasKey(_inScopeDict, name);
					if(flag){
						curScope=_inScopeDict;
						break;
					}
				}
			}
		}

		// cur
		if(!flag){
			flag=map_hasKey(inScopeDict, name);
			if(flag){
				curScope=inScopeDict;
			}
		}

		// up
		if(!flag){
			VanMap *_inScopeDict=NULL;

			length=list_length(function->chainScopeList);
			for(int i=length-1;i>=0;i--){
				_inScopeDict=list_getValue(chainScopeList, i)->data;
				flag=map_hasKey(_inScopeDict, name);
				if(flag){
					curScope=_inScopeDict;
					break;
				}
			}
		}

		// global
		if(!flag){
			curScope=env->scopeDict;
			flag=1;
		}
		
	}

	if(!flag){
		curScope=env->scopeDict;
	}

	env->curScope=curScope;
}

static void environ_calCurBindObj(VanEnviron *env){
	int length=0;
	VanFunction *function=NULL;

	length=list_length(env->callStack);
	if(length){
		function=list_getValue(env->callStack, length-1)->data;
		env->curBindObj=function->bindObj;
		// if(function->bindObj){
		// 	env->curIns=function->bindObj->data;
		// }
	}
}

// simple literal scope==>get
static void environ_evalIndentify(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	int flag=0;
	

	environ_calReadScope(env, astNode->data);
	flag=map_hasKey(env->curScope, astNode->data);
	if(flag){
		wrap=map_getValue(env->curScope, astNode->data);
		wrap_retain(wrap);
	}
	// else {
	// 	flag=map_hasKey(env->scopeDict, astNode->data);
	// 	if(flag){
	// 		wrap=map_getValue(env->scopeDict, astNode->data);
	// 		wrap_retain(wrap);
	// 	}
	// }
	
	astNode->wrap=wrap;
}

static void __environ_evalOutIndentify(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	int flag=0;
	

	environ_calReadScope(env, astNode->data);
	flag=map_hasKey(env->curScope, astNode->data);
	if(flag){
		wrap=map_getRefValue(env->curScope, astNode->data);
		wrap_retain(wrap);
	}
	// else {
	// 	flag=map_hasKey(env->scopeDict, astNode->data);
	// 	if(flag){
	// 		wrap=map_getValue(env->scopeDict, astNode->data);
	// 		wrap_retain(wrap);
	// 	}
	// }
	
	astNode->wrap=wrap;
}

static void environ_evalNumber(VanEnviron *env,ASTNode *astNode){

	if(astNode->op==ASTOp_Int){
		astNode->wrap=wrap_wrapLongLong(astNode->value.l);
	}
	else if(astNode->op==ASTOp_Float){
		astNode->wrap=wrap_wrapDouble(astNode->value.d);
	}

}

static void environ_evalNull(VanEnviron *env,ASTNode *astNode){

	astNode->wrap=wrap_wrapInt(0);
}

static void environ_evalTrue(VanEnviron *env,ASTNode *astNode){

	astNode->wrap=wrap_wrapInt(1);
}

static void environ_evalFalse(VanEnviron *env,ASTNode *astNode){

	astNode->wrap=wrap_wrapInt(0);
}

static void environ_evalSelf(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;

	environ_calCurBindObj(env);
	wrap=env->curBindObj;
	wrap->value.l=0;
	astNode->wrap=wrap;
}

static void environ_evalSuper(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;

	environ_calCurBindObj(env);
	wrap=env->curBindObj;
	wrap->value.l=1; // ???

	// if(wrap){
	// 	if(wrap->type==DataType_Instance){
	// 		wrap->value.l=1; // ???
	// 	}
	// 	else if(wrap->type==DataType_Class){
	// 		VanClass *cls=NULL;

	// 		cls=wrap->data;
	// 		wrap=wrap_wrapClass(cls->base);
	// 	}
	// }
	
	astNode->wrap=wrap;
}

static void environ_evalString(VanEnviron *env,ASTNode *astNode){
	VanWrap *wrap=NULL;
	VanString *vanStr=NULL;

	int len=0;
	char *data1=NULL;

	len=strlen(astNode->data)+1;
	data1=(char *)calloc(len+1, sizeof(char ));
	strcpy(data1,astNode->data);

	vanStr=string_create(data1);
	wrap=wrap_wrapString(vanStr);

	astNode->wrap=wrap;
}

// binary相关 astNode optimizer ???
static void environ_evalBinary(VanEnviron *env,ASTNode *astNode){
	ASTOp op=0;
	ASTNode **childArr=NULL;
	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;

	VanWrap *wrap=NULL;
	VanWrap *leftWrap=NULL;
	VanWrap *rightWrap=NULL;

	childArr=(ASTNode **)astNode->childArr;
	leftNode=childArr[0];
	rightNode=childArr[1];

	environ_evalExpr(env, leftNode);
	environ_evalExpr(env, rightNode);

	leftWrap=leftNode->wrap;
	rightWrap=rightNode->wrap;

	op=astNode->op;
	wrap=wrap_binaryCal(leftWrap,rightWrap,op);

	if(!wrap){
		printf(">>> throw OperateError\n");
		exit(0);
	}

	astNode->wrap=wrap;
}

static void environ_evalPrior(VanEnviron *env,ASTNode *astNode){
	ASTNode *node=NULL;

	node=astNode->childArr[0];
	environ_evalExpr(env,node);

	wrap_retain(node->wrap);
	astNode->wrap=node->wrap;
}

static void environ_evalNot(VanEnviron *env,ASTNode *astNode){
	ASTNode *node=NULL;
	VanWrap *wrap=NULL;

	node=astNode->childArr[0];
	environ_evalExpr(env,node);
	
	if(node->wrap->type==DataType_Int){
		wrap=wrap_wrapLongLong(!node->wrap->value.l);
	}
	else if(node->wrap->type==DataType_Float){
		wrap=wrap_wrapLongLong(!node->wrap->value.d);
	}
	
	astNode->wrap=wrap;
}

static void environ_evalReverse(VanEnviron *env,ASTNode *astNode){
	ASTNode *node=NULL;
	VanWrap *wrap=NULL;

	node=astNode->childArr[0];
	environ_evalExpr(env,node);
	
	if(node->wrap->type==DataType_Int){
		wrap=wrap_wrapLongLong(~node->wrap->value.l);
	}
	
	astNode->wrap=wrap;
}

static void environ_evalNegative(VanEnviron *env,ASTNode *astNode){
	ASTNode *node=NULL;
	VanWrap *wrap=NULL;

	node=astNode->childArr[0];
	environ_evalExpr(env,node);
	
	if(node->wrap->type==DataType_Int){
		wrap=wrap_wrapLongLong(-node->wrap->value.l);
	}
	else if(node->wrap->type==DataType_Float){
		wrap=wrap_wrapLongLong(-node->wrap->value.d);
	}
	
	astNode->wrap=wrap;
}

static void environ_evalUnCome(VanEnviron *env,ASTNode *astNode){

	astNode->wrap=wrap_wrapInt(0);
}




