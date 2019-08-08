// clang 

#include "van_parse.h"
#include "van_lexUtil.h"
#include "van_parseUtil.h"

static Token *parseState(Token *token, ASTNode **node);
static Token *parseBlock(Token *start,Token *end,ASTNode **node);

static Token *parseExprState(Token *token, ASTNode **node);
static Token *parseAssignState(Token *token,ASTNode **node);
static Token *parseIfState(Token *token,ASTNode **node);
static Token *parseWhileState(Token *token,ASTNode **node);
static Token *parseForState(Token *token,ASTNode **node);
static Token *__parseForInState(Token *token,Token *inToken,ASTNode **node);
static Token *__parseForThreeState(Token *token,ASTNode **node);
static Token *parseFunctionState(Token *token,ASTNode **node);
static Token *parseClassState(Token *token,ASTNode **node);

static Token *parseContinueState(Token *token,ASTNode **node);
static Token *parseBreakState(Token *token,ASTNode **node);
static Token *parseReturnState(Token *token,ASTNode **node);
static Token *parseYieldState(Token *token,ASTNode **node);

// flag 0 expr 1 state 
static Token *__parseArguments(Token *token,int flag,ASTNode **node);
// flag 0 right 1 left 2 for
static Token *__parseComma(Token *token,Token *end,int flag,ASTNode **node);
// static Token *__parseFunction(Token *token,int flag,ASTNode **node);
// static Token *__parseClass(Token *token,int flag,ASTNode **node);

static Token *parseArguments(Token *token,ASTNode **node);
static Token *parseUpState(Token *token,ASTNode **node);
static Token *parseGlobalState(Token *token,ASTNode **node);
static Token *parseFieldState(Token *token,ASTNode **node);
static Token *parseStaticState(Token *token,ASTNode **node);
static Token *parseImportState(Token *token,ASTNode **node);
static Token *parsePrintState(Token *token,ASTNode **node);
static Token *__parseSemState(Token *token,ASTNode **node,ASTType type,int flag);

static Token *parseExpr(Token *token,Token *terminal,ASTNode *preNode, ASTNode **node);
static Token *__parseOpenExpr(Token *token,Token *terminal, ASTNode **node);
static Token *__parseReduceExpr(Token *token,Token *terminal,ASTNode *preNode, ASTNode **node);
static Token *__parseIncompleteExpr(Token *token,Token *terminal,ASTNode *preNode, ASTNode **node);

static Token *parseMap(Token *start, ASTNode **node);
static Token *parseList(Token *start, ASTNode **node);

static Token *parseFunctionExpr(Token *token,ASTNode **node);
static Token *parseClassExpr(Token *token,ASTNode **node);

static Token *parseArgumentsExpr(Token *token,ASTNode **node);
static Token *parsePair(Token *token,Token *tail, ASTNode **node);

static Token *parseCall(Token *token,ASTNode *preNode,ASTNode **node);
static Token *parseProperty(Token *token,ASTNode *preNode,ASTNode **node);
static Token *parseSubscript(Token *token,ASTNode *preNode,ASTNode **node);

static Token *parseIdentifi(Token *token ,ASTNode **node);
static Token *parseNumber(Token *token,ASTNode **node);
static Token *parseNull(Token *token,ASTNode **node);
static Token *parseTrue(Token *token,ASTNode **node);
static Token *parseFalse(Token *token,ASTNode **node);
static Token *parseSelf(Token *token,ASTNode **node);
static Token *parseSuper(Token *token,ASTNode **node);
static Token *parseStringExpr(Token *token,ASTNode **node);

static Token *parsePrior(Token *token,ASTNode **node);

static Token *parseMono(Token *token,Token *terminal,ASTNode **node);
static Token *parseNegative(Token *token,Token *terminal,ASTNode **node);
static Token *parseNew(Token *token,Token *terminal,ASTNode **node);
static Token *parseReverse(Token *token,Token *terminal,ASTNode **node);
static Token *parseNot(Token *token,Token *terminal,ASTNode **node);


int parseToken(Token *token,ASTNode **node){
	int status=0;
	int flag=1;

	ASTNode *head=NULL;
	ASTNode *cur=NULL;

	token=filterEmpty(token);
	token=parseState(token,&head);

	cur=head;
	token=filterEmpty(token);
	while(token){
		ASTNode *_cur=NULL;
		
		token=parseState(token,&_cur);
		if(!_cur){
			flag=0;
			freeASTNode(_cur);
			break;
		}
		cur->next=_cur;
		cur=cur->next;
		token=filterEmpty(token);
	}

	if(flag){
		*node=head;
	}
	else{
		freeASTNode(head);
	}

	return status;
}

static Token *parseMap(Token *token, ASTNode **node){
	Token *end=NULL;
	Token *tail=NULL;
	Token *split=NULL;

	ASTNode *cur=NULL;
	ASTNode *elemet=NULL;
	ASTNode **nodeArr=NULL;
	int length=0;
	int len=0;

	end=__boundMapExpr(token);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	cur=(ASTNode *)calloc(1, sizeof(ASTNode ));
	cur->type=ASTType_Expr;
	cur->op=ASTOp_Map;

	length=__calListLength(token,end);
	if(length){
		nodeArr=(ASTNode **)calloc(length, sizeof(ASTNode *));
		token=token->next;
		tail=end;
		while(token!=end){
			split=__boundComma(token,end);
			if(split){ 
				tail=parsePair(token,split,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);

					freeASTNode(cur);
					for(int i=0;i<len;i++){
						freeASTNode(nodeArr[i]);
					}
					free(nodeArr);
					freeASTNode(elemet);

					return NULL;
				}

				token=split->next;
			}
			else{
				tail=parsePair(token,end,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);

					freeASTNode(cur);
					for(int i=0;i<len;i++){
						freeASTNode(nodeArr[i]);
					}
					free(nodeArr);
					freeASTNode(elemet);

					return NULL;
				}

				token=end;
			}

			nodeArr[len]=elemet;
			elemet->parent=cur;
			len++;
		}
	}

	cur->childLength=length;
	cur->childArr=(ASTNode **)nodeArr;

	*node=cur;
	end=end->next;
	return end;
}

static Token *parseList(Token *token, ASTNode **node){
	Token *end=NULL;
	Token *tail=NULL;
	Token *split=NULL;

	ASTNode *cur=NULL;
	ASTNode *elemet=NULL;
	ASTNode **nodeArr=NULL;
	int length=0;
	int len=0;

	end=__boundListExpr(token);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	cur=(ASTNode *)calloc(1, sizeof(ASTNode ));
	cur->type=ASTType_Expr;
	cur->op=ASTOp_List;

	length=__calListLength(token,end);
	if(length){
		nodeArr=(ASTNode **)calloc(length, sizeof(ASTNode *));
		token=token->next;
		while(token!=end){
			split=__boundComma(token,end);
			if(split){ 
				tail=parseExpr(token,split,NULL,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);

					freeASTNode(cur);
					for(int i=0;i<len;i++){
						freeASTNode(nodeArr[i]);
					}
					free(nodeArr);
					freeASTNode(elemet);

					return NULL;
				}

				token=split->next;
			}
			else{
				tail=parseExpr(token,end,NULL,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);

					freeASTNode(cur);
					for(int i=0;i<len;i++){
						freeASTNode(nodeArr[i]);
					}
					free(nodeArr);
					freeASTNode(elemet);

					return NULL;
				}

				token=end;
			}

			nodeArr[len]=elemet;
			elemet->parent=cur;
			len++;
		}
	}

	cur->childLength=length;
	cur->childArr=(ASTNode **)nodeArr;

	*node=cur;
	end=end->next;
	return end;
}

static Token *parseFunctionExpr(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *start=NULL;

	ASTNode *functionNode=NULL;
	ASTNode *nameNode=NULL;
	ASTNode *argNode=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundFunctionState(token);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}
	start=token->next;

	functionNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
	functionNode->type=ASTType_Expr;
	functionNode->op=ASTOp_FunctionExpr;

	functionNode->childLength=3;
	functionNode->childArr=(ASTNode **)calloc(3, sizeof(ASTNode *));

	if(start->subType==TokenSubType_LeftBrace){ // block
		end=parseBlock(start, end, &blockNode);
		if(!end){
			// error_throwError(ErrorType_ExprSyntaxError, start);

			freeASTNode(functionNode);
			freeASTNode(blockNode);

			return NULL;
		}

		if(end){
			functionNode->childArr[2]=blockNode;
			blockNode->op=ASTOp_FunctionBlock;
			blockNode->parent=functionNode;

			*node=functionNode;
			return end;
		}
	}
	else if(start->subType==TokenSubType_LeftParenth){ // arg

		_token=start;
		start=parseArguments(start, &argNode);
		if(!start){
			// error_throwError(ErrorType_ExprSyntaxError, _token);

			freeASTNode(functionNode);
			freeASTNode(argNode);

			return NULL;
		}

		if(start->subType==TokenSubType_LeftBrace){ // block
			end=parseBlock(start, end, &blockNode);
			if(!end){
				// error_throwError(ErrorType_ExprSyntaxError, start);

				freeASTNode(functionNode);
				freeASTNode(argNode);
				freeASTNode(blockNode);

				return NULL;
			}

			if(end){
				functionNode->childArr[1]=argNode;
				argNode->parent=functionNode;

				functionNode->childArr[2]=blockNode;
				blockNode->op=ASTOp_FunctionBlock;
				blockNode->parent=functionNode;

				*node=functionNode;
				return end;
			}
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);
	
	freeASTNode(functionNode);
	freeASTNode(argNode);
	freeASTNode(blockNode);

	return NULL;
}

// Class={Name,Expr,ClassBlock}
static Token *parseClassExpr(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *start=NULL;

	ASTNode *classNode=NULL;
	ASTNode *nameNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundClassState(token);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}
	start=token->next;

	classNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
	classNode->type=ASTType_Expr;
	classNode->op=ASTOp_ClassExpr;

	classNode->childLength=3;
	classNode->childArr=(ASTNode **)calloc(3, sizeof(ASTNode *));

	if(start->subType==TokenSubType_LeftParenth||
		start->subType==TokenSubType_LeftBrace){ // (||{

		if(start->subType==TokenSubType_LeftParenth){
			Token *_end=NULL;

			_end=__boundParenth(start);
			if(!_end){
				// error_throwError(ErrorType_ExprSyntaxError, start);
				freeASTNode(classNode);
				return NULL;
			}

			if(start->next!=_end){

				_token=start->next;
				start=parseExpr(start->next, _end,NULL,&exprNode);
				if(!start){
					// error_throwError(ErrorType_ExprSyntaxError, _token);

					freeASTNode(classNode);
					freeASTNode(exprNode);

					return NULL;
				}

			}
			else{
				start=start->next;
			}

			start=start->next;
		}
		
		if(start->subType==TokenSubType_LeftBrace){ // block
			end=__boundBrace(start);
			if(!end){
				// error_throwError(ErrorType_ExprSyntaxError, start);

				freeASTNode(classNode);
				freeASTNode(exprNode);

				return NULL;
			}

			end=parseBlock(start, end, &blockNode);
			if(!end){
				// error_throwError(ErrorType_ExprSyntaxError, start);

				freeASTNode(classNode);
				freeASTNode(exprNode);
				freeASTNode(blockNode);

				return NULL;
			}

			if(end){
				if(exprNode){
					classNode->childArr[1]=exprNode;
					exprNode->parent=classNode;
				}

				classNode->childArr[2]=blockNode;
				blockNode->op=ASTOp_ClassBlock;
				blockNode->parent=classNode;

				*node=classNode;
				return end;
			}
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(classNode);
	freeASTNode(exprNode);
	freeASTNode(blockNode);

	return NULL;
}

static Token *__parseOpenExpr(Token *token,Token *terminal, ASTNode **node){
	Token *end=NULL;
	Token *error=token;

	if(__isIdentifiExpr(token)){
		end=parseIdentifi(token,node);
	}
	else if(__isNumberExpr(token)){
		end=parseNumber(token,node);
	}
	else if(__isNullExpr(token)){
		end=parseNull(token,node);
	}
	else if(__isTrueExpr(token)){
		end=parseTrue(token,node);
	}
	else if(__isFalseExpr(token)){
		end=parseFalse(token,node);
	}
	else if(__isSelfExpr(token)){
		end=parseSelf(token,node);
	}
	else if(__isSuperExpr(token)){
		end=parseSuper(token,node);
	}
	else if(__isStringExpr(token)){
		end=parseStringExpr(token,node);
	}
	else if(__isMapExpr(token)){
		end=parseMap(token,node);
	}
	else if(__isListExpr(token)){
		end=parseList(token,node);
	}
	else if(__isFunctionExpr(token)){
		end=parseFunctionExpr(token,node);
	}
	else if(__isClassExpr(token)){
		end=parseClassExpr(token,node);
	}
	else if(__isPriorExpr(token)){
		end=parsePrior(token,node);
	}
	else if(__isNegativeExpr(token)){
		end=parseNegative(token,terminal,node);
	}
	else if(__isNewExpr(token)){
		end=parseNew(token,terminal,node);
	}
	else if(__isReverseExpr(token)){
		end=parseReverse(token,terminal,node);
	}
	else if(__isNotExpr(token)){
		end=parseNot(token,terminal,node);
	}
	else{ // error

	}

	// if(!end){
	// 	error_throwError(ErrorType_ExprSyntaxError, error);
	// }

	return end;
}

static Token *__parseIncompleteExpr(Token *token,Token *terminal,ASTNode *preNode, ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	TokenSubType curSubType=0;
	ASTOp op=0;
	Token *error=token;

	if(token!=terminal){ // right 递归
		curSubType=token->subType;
		if(curSubType>TokenSubType_BinaryStart&&
			curSubType<TokenSubType_BinaryEnd){ 
			op=(ASTOp )curSubType;
		}
	}

	if(op){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

		curNode->type=ASTType_Expr;
		curNode->op=op;
		curNode->childLength=2;
		curNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));
		curNode->childArr[0]=preNode;

		*node=curNode;
		end=token->next;
		return end;
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);
	freeASTNode(curNode);
	return NULL;
}

static Token *__parseReduceExpr(Token *token,Token *terminal,ASTNode *preNode, ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	TokenSubType curSubType=0;
	Token *_token=NULL;

	while(token!=terminal){
		curSubType=token->subType;
		if(curSubType==TokenSubType_LeftParenth||
			curSubType==TokenSubType_Dot||
			curSubType==TokenSubType_LeftBracket){ // left 归约

			_token=token;
			if(curSubType==TokenSubType_LeftParenth){ // Expr(Arguments)
				token=parseCall(token, preNode, &curNode);
			}
			else if(curSubType==TokenSubType_Dot){ // Expr.Identifi
				token=parseProperty(token, preNode, &curNode);
			}
			else { // Expr[Expr]
				token=parseSubscript(token, preNode, &curNode);
			}

			if(!token){
				// error_throwError(ErrorType_ExprSyntaxError, _token);
				freeASTNode(curNode);
				return NULL;
			}

			preNode=curNode;
			continue;
		}
		else{
			curNode=preNode;
			break;
		}
	}

	*node=curNode;
	end=token;
	return end;
}

/***
	parseExpr
		1. open 起始可终结
		2. reduce 归约可终结
		3. incomplete 不完全不可终结

		1. parseExpr=open
		2. parseExpr=open+reduce
		3. parseExpr=open+reduce+incomplete+parseExpr
****/
static Token *parseExpr(Token *token,Token *terminal,ASTNode *preNode, ASTNode **node){
	Token *end=NULL;

	ASTNode *openNode=NULL;
	ASTNode *reduceNode=NULL;
	ASTNode *incometNode=NULL;
	
	Token *error=token;
	Token *_token=NULL;

	// 1. 
	_token=token;
	token=__parseOpenExpr(token,terminal,&openNode);
	if(!token){
		// error_throwError(ErrorType_ExprSyntaxError, _token);
		freeASTNode(openNode);
		return NULL;
	}

	if(token&&token==terminal){
		*node=astNode_mergeNode(preNode,openNode);
		return terminal;
	}

	// 2. 
	_token=token;
	token=__parseReduceExpr(token,terminal, openNode,&reduceNode);
	if(!token){
		// error_throwError(ErrorType_ExprSyntaxError, _token);

		freeASTNode(openNode);
		freeASTNode(reduceNode);

		return NULL;
	}

	if(token&&token==terminal){
		*node=astNode_mergeNode(preNode,reduceNode);
		return terminal;
	}
	
	// 3. 
	_token=token;
	token=__parseIncompleteExpr(token,terminal, reduceNode,&incometNode);
	if(!token){
		// error_throwError(ErrorType_ExprSyntaxError, _token);

		freeASTNode(openNode);
		freeASTNode(reduceNode);
		freeASTNode(incometNode);

		return NULL;
	}

	if(token){
		preNode=astNode_mergeNode(preNode,incometNode);
		end=parseExpr(token,terminal,preNode,node);
	}

	if(end!=terminal){
		end=NULL;
	}

	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, error);

		freeASTNode(openNode);
		freeASTNode(reduceNode);
		freeASTNode(incometNode);
		freeASTNode(*node);
		*node=NULL;
	}

	return end;
}

static Token *parsePair(Token *token,Token *tail, ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	ASTNode *keyNode=NULL;
	ASTNode *valueNode=NULL;
	Token *error=token;

	if(token->type==TokenType_Identify){ // String ???
		// keyNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
		// keyNode->type=ASTType_Expr;
		// keyNode->op=ASTOp_Indentify;
		// keyNode->data=(char *)calloc(strlen(token->data)+1, sizeof(char ));
		// strcpy(keyNode->data,token->data);
		end=parseExpr(token, token->next, NULL, &keyNode);
		if(!end){
			// error_throwError(ErrorType_ExprSyntaxError, token);
			freeASTNode(keyNode);
			return NULL;
		}

		if(token->next->subType==TokenSubType_Colon){
			end=parseExpr(token->next->next, tail, NULL,&valueNode);
			if(!end){
				error_throwError(ErrorType_ExprSyntaxError, token->next->next);

				freeASTNode(keyNode);
				freeASTNode(valueNode);

				return NULL;
			}

			if(end==tail){
				curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

				curNode->type=ASTType_Expr;
				curNode->childLength=2;
				curNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

				curNode->childArr[0]=keyNode;
				curNode->childArr[1]=valueNode;
				keyNode->parent=curNode;
				valueNode->parent=curNode;

				*node=curNode;
				end=end->next;
				return end;
			}
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(keyNode);
	freeASTNode(valueNode);
	freeASTNode(curNode);

	return NULL;
}

static Token *parseArgumentsExpr(Token *token,ASTNode **node){
	Token *end=NULL;

	end=__parseArguments(token,0,node);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		freeASTNode(*node);
		*node=NULL;
	}

	return end;
}

static Token *parseCall(Token *token,ASTNode *preNode,ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	ASTNode *argNode=NULL;
	Token *error=token;

	end=__boundParenth(token);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	if(end){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

		curNode->type=ASTType_Expr;
		curNode->op=ASTOp_Call;
		curNode->childLength=2;
		curNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

		end=parseArgumentsExpr(token, &argNode);
		if(!end){
			// error_throwError(ErrorType_ExprSyntaxError, token);

			freeASTNode(curNode);
			freeASTNode(argNode);

			return NULL;
		}

		if(end){
			curNode->childArr[0]=preNode;
			curNode->childArr[1]=argNode;
			preNode->parent=curNode;
			argNode->parent=curNode;

			*node=curNode;
			return end;
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(curNode);
	freeASTNode(argNode);

	return NULL;
}

static Token *parseProperty(Token *token,ASTNode *preNode,ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	ASTNode *propertyNode=NULL;
	Token *error=token;

	if(token->next){
		if(token->next->type==TokenType_Identify){
			curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

			curNode->type=ASTType_Expr;
			curNode->op=ASTOp_Property;
			curNode->childLength=2;
			curNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

			end=parseIdentifi(token->next,&propertyNode);
			if(!end){
				// error_throwError(ErrorType_ExprSyntaxError, token->next);

				freeASTNode(curNode);
				freeASTNode(propertyNode);

				return NULL;
			}

			if(end){
				curNode->childArr[0]=preNode;
				curNode->childArr[1]=propertyNode;
				preNode->parent=curNode;
				propertyNode->parent=curNode;

				*node=curNode;
				return end;
			}
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(curNode);
	freeASTNode(propertyNode);

	return NULL;
}

static Token *parseSubscript(Token *token,ASTNode *preNode,ASTNode **node){
	Token *end=NULL;
	Token *tail=NULL;
	ASTNode *curNode=NULL;
	ASTNode *subNode=NULL;
	Token *error=token;

	end=__boundBracket(token);
	if(!end){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	tail=end;
	if(end){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

		curNode->type=ASTType_Expr;
		curNode->op=ASTOp_Subscript;
		curNode->childLength=2;
		curNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

		end=parseExpr(token->next, tail, NULL, &subNode);
		if(!end){
			// error_throwError(ErrorType_ExprSyntaxError, token->next);

			freeASTNode(curNode);
			freeASTNode(subNode);

			return NULL;
		}

		if(end==tail){
			curNode->childArr[0]=preNode;
			curNode->childArr[1]=subNode;
			preNode->parent=curNode;
			subNode->parent=curNode;

			*node=curNode;
			end=end->next;
			return end;
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(curNode);
	freeASTNode(subNode);

	return NULL;
}

static Token *parseIdentifi(Token *token ,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	int len=0;

	len=strlen(token->data)+1;
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));

	cur->type=ASTType_Expr;
	cur->op=ASTOp_Indentify;

	cur->data=(char *)calloc(len, sizeof(char ));
	strcpy(cur->data, token->data);

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseNumber(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;

	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));

	cur->type=ASTType_Expr;
	if(token->subType==TokenSubType_Int){
		cur->op=ASTOp_Int;
		cur->value.l=token->value.l;
	}
	else if(token->subType==TokenSubType_Float){
		cur->op=ASTOp_Float;
		cur->value.d=token->value.d;
	}

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseNull(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	int len=0;

	len=strlen(token->data)+1;
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));

	cur->type=ASTType_Expr;
	cur->op=ASTOp_Null;

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseTrue(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	int len=0;

	len=strlen(token->data)+1;
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));

	cur->type=ASTType_Expr;
	cur->op=ASTOp_True;

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseFalse(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	int len=0;

	len=strlen(token->data)+1;
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));

	cur->type=ASTType_Expr;
	cur->op=ASTOp_False;

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseSelf(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));
	cur->type=ASTType_Expr;
	cur->op=ASTOp_Self;

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseSuper(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));
	cur->type=ASTType_Expr;
	cur->op=ASTOp_Super;

	*node=cur;
	end=token->next;
	return end;
}

static Token *parseStringExpr(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	int len=0;

	len=strlen(token->data)+1;
	cur=(ASTNode *)calloc(1,sizeof(ASTNode ));

	cur->type=ASTType_Expr;
	cur->op=ASTOp_String;

	cur->data=(char *)calloc(len, sizeof(char ));
	strcpy(cur->data, token->data);

	*node=cur;
	end=token->next;
	return end;
}

static Token *parsePrior(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *bound=NULL;
	ASTNode *parent=NULL;
	ASTNode *child=NULL;
	Token *error=token;

	bound=__boundParenth(token);
	if(!bound){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	if(bound){
		parent=(ASTNode *)calloc(1, sizeof(ASTNode ));

		parent->type=ASTType_Expr;
		parent->op=ASTOp_Prior;

		parent->childArr=(ASTNode **)calloc(1, sizeof(ASTNode *));
		parent->childLength=1;

		end=parseExpr(token->next,bound,NULL,&child);
		if(!end){
			// error_throwError(ErrorType_ExprSyntaxError, token->next);

			freeASTNode(parent);
			freeASTNode(child);

			return NULL;
		}

		if(end==bound){ // 存在
			parent->childArr[0]=child;
			child->parent=parent;

			*node=parent;
			end=end->next;
			return end;
		}
	}

	// error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(parent);
	freeASTNode(child);

	return NULL;
}

static Token *parseMono(Token *token,Token *terminal,ASTNode **node){
	Token *end=NULL;
	Token *bound=NULL;
	ASTNode *monoNode=NULL;
	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;
	ASTNode *curNode=NULL;
	Token *_token=NULL;

	bound=__boundMonoExpr(token);
	if(!bound){
		// error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	while(token&&bound){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

		curNode->type=ASTType_Expr;
		curNode->childLength=1;
		curNode->childArr=(ASTNode **)calloc(1, sizeof( ASTNode *));
		if(token->subType==TokenSubType_Not){ // !
			curNode->op=ASTOp_Not;
		}
		else if(token->subType==TokenSubType_Reverse){ // ~
			curNode->op=ASTOp_Reverse;
		}
		else if(token->subType==TokenSubType_New){ // New
			curNode->op=ASTOp_New;
		}
		else{ // -
			curNode->op=ASTOp_Negative;
		}

		if(monoNode){
			monoNode->childArr[0]=curNode;
			curNode->parent=monoNode;
		}
		monoNode=curNode;
		
		if(token==bound){
			break;
		}
		token=token->next;
	}

	token=__parseOpenExpr(bound->next,terminal,&leftNode);
	if(!token){
		// error_throwError(ErrorType_ExprSyntaxError, bound->next);

		freeASTNode(curNode);
		freeASTNode(leftNode);

		return NULL;
	}

	if(token==terminal){
		monoNode->childArr[0]=leftNode;
		leftNode->parent=monoNode;
		*node=astNode_getRootNode(monoNode);

		return terminal;
	}

	_token=token;
	token=__parseReduceExpr(token,terminal,leftNode,&rightNode);
	if(!token){
		// error_throwError(ErrorType_ExprSyntaxError, _token);

		freeASTNode(curNode);
		freeASTNode(leftNode);
		freeASTNode(rightNode);

		return NULL;
	}

	monoNode->childArr[0]=rightNode;
	rightNode->parent=monoNode;
	*node=astNode_getRootNode(monoNode);

	end=token;

	return end;
}

static Token *parseNot(Token *token,Token *terminal,ASTNode **node){
	Token *end=NULL;
	
	end=parseMono(token,terminal,node);

	return end;
}

static Token *parseNegative(Token *token,Token *terminal,ASTNode **node){
	Token *end=NULL;
	
	end=parseMono(token,terminal,node);

	return end;
}

// new Indentity|(Expr) || new Expr ???
static Token *parseNew(Token *token,Token *terminal,ASTNode **node){
	Token *end=NULL;
	
	end=parseMono(token,terminal,node);

	return end;
}

static Token *parseReverse(Token *token,Token *terminal,ASTNode **node){
	Token *end=NULL;
	
	end=parseMono(token,terminal,node);

	return end;
}

// state 相关 ???
static Token *parseState(Token *token, ASTNode **node){
	Token *end=NULL;

	if(__isIfState(token)){
		end=parseIfState(token,node);
	}
	else if(__isWhileState(token)){
		end=parseWhileState(token,node);
	}
	else if(__isStaticState(token)){ // ??? function/field pre
		end=parseStaticState(token,node);
	}
	else if(__isForState(token)){
		end=parseForState(token,node);
	}
	else if(__isFunctionState(token)){
		end=parseFunctionState(token,node);
	}
	else if(__isClassState(token)){
		end=parseClassState(token,node);
	}
	else if(__isContinueState(token)){
		end=parseContinueState(token,node);
	}
	else if(__isBreakState(token)){
		end=parseBreakState(token,node);
	}
	else if(__isReturnState(token)){
		end=parseReturnState(token,node);
	}
	else if(__isUpState(token)){
		end=parseUpState(token,node);
	}
	else if(__isGlobalState(token)){
		end=parseGlobalState(token,node);
	}
	else if(__isFieldState(token)){
		end=parseFieldState(token,node);
	}
	else if(__isYieldState(token)){
		end=parseYieldState(token,node);
	}
	else if(__isImportState(token)){
		end=parseImportState(token,node);
	}
	else if(__isPrintState(token)){
		end=parsePrintState(token,node);
	}
	else if(__isAssignState(token)){ // ??? block next
		end=parseAssignState(token, node);
	}
	else{ // ??? __isExprState
		end=parseExprState(token, node);
	}

	return end;
}

/***
	block check ???
****/
static Token *parseBlock(Token *start,Token *end,ASTNode **node){
	Token *token=NULL;

	ASTNode *blockNode=NULL;
	ASTNode *head=NULL;
	ASTNode *cur=NULL;

	blockNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
	blockNode->type=ASTType_Block;

	token=start->next;
	if(token==end){
		*node=blockNode;
		end=end->next;
		return end;
	}

	token=filterEmpty(token);
	if(token==end){
		*node=blockNode;
		end=end->next;
		return end;
	}

	token=parseState(token,&head);
	if(token){
		blockNode->childLength=1;
		blockNode->childArr=(ASTNode **)calloc(1, sizeof(ASTNode *));
		blockNode->childArr[0]=head;
		head->parent=blockNode;
		
		cur=head;

		if(token==end){
			*node=blockNode;
			end=end->next;
			return end;
		}

		token=filterEmpty(token);
		while(token!=end){
			ASTNode *_cur=NULL;

			token=parseState(token,&_cur);
			if(!_cur){ // block失败

				freeASTNode(blockNode);
				freeASTNode(_cur);

				break;
			}
			cur->next=_cur;
			_cur->parent=blockNode;
			cur=cur->next;
			token=filterEmpty(token);
		}

		*node=blockNode;
		end=end->next;
		return end;
	}

	freeASTNode(blockNode);

	return NULL;
}

static Token *parseExprState(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *tail=NULL;
	Token *error=token;

	end=__boundExprState(token);
	if(!end){
		error_throwError(ErrorType_ExprSyntaxError, token);
		return NULL;
	}

	if(end){
		tail=parseExpr(token, end, NULL, node);
		if(!tail){
			error_throwError(ErrorType_ExprSyntaxError, token);

			freeASTNode(*node);
			*node=NULL;

			return NULL;
		}

		if(tail==end){
			end=end->next;
			return end;
		}
	}

	error_throwError(ErrorType_ExprSyntaxError, error);

	freeASTNode(*node);
	*node=NULL;

	return NULL;
}

static Token *parseAssignState(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *aToken=NULL;

	ASTNode *curNode=NULL;
	ASTNode *leftNode=NULL;
	ASTNode *rightNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundAssignState(token);
	if(!end){
		error_throwError(ErrorType_AssignSyntaxError, token);
		return NULL;
	}

	if(end){
		aToken=__boundAssign(token, end);
		if(!aToken){
			error_throwError(ErrorType_AssignSyntaxError, token);
			return NULL;
		}

		if(aToken){
			_token=token;
			// token=parseExpr(token, aToken, NULL, &leftNode);
			token=__parseComma(token, aToken, 1, &leftNode);
			if(!token){
				error_throwError(ErrorType_AssignSyntaxError, _token);
				freeASTNode(leftNode);
				return NULL;
			}

			if(token==aToken){
				_token=token->next;
				// token=parseExpr(token->next, end, NULL, &rightNode);
				token=__parseComma(token->next, end, 0, &rightNode);
				if(!token){
					error_throwError(ErrorType_AssignSyntaxError, _token);

					freeASTNode(leftNode);
					freeASTNode(rightNode);

					return NULL;
				}

				if(token==end){
					curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));

					curNode->type=ASTType_Assign;
					curNode->childLength=2;
					curNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

					curNode->childArr[0]=leftNode;
					leftNode->parent=curNode;
					curNode->childArr[1]=rightNode;
					rightNode->parent=curNode;

					if(aToken->subType==TokenSubType_Assign){
						curNode->op=ASTOp_DeclareAssign;
					}
					else if(aToken->subType==TokenSubType_MulAssgin){
						curNode->op=ASTOp_MulAssgin;
					}
					else if(aToken->subType==TokenSubType_DivAssgin){
						curNode->op=ASTOp_DivAssgin;
					}
					else if(aToken->subType==TokenSubType_ModAssgin){
						curNode->op=ASTOp_ModAssgin;
					}
					else if(aToken->subType==TokenSubType_AddAssgin){
						curNode->op=ASTOp_AddAssgin;
					}
					else if(aToken->subType==TokenSubType_SubAssgin){
						curNode->op=ASTOp_SubAssgin;
					}
					else if(aToken->subType==TokenSubType_LeftShiftAssgin){
						curNode->op=ASTOp_LeftShiftAssgin;
					}
					else if(aToken->subType==TokenSubType_RightShiftAssgin){
						curNode->op=ASTOp_RightShiftAssgin;
					}
					else if(aToken->subType==TokenSubType_XOrAssgin){
						curNode->op=ASTOp_XOrAssgin;
					}
					else if(aToken->subType==TokenSubType_XAndAssgin){
						curNode->op=ASTOp_XAndAssgin;
					}
					else if(aToken->subType==TokenSubType_XErAssgin){
						curNode->op=ASTOp_XErAssgin;
					}
					else{
						error_throwError(ErrorType_AssignSyntaxError, aToken);
						freeASTNode(curNode);
						return NULL;
					}

					*node=curNode;
					end=end->next;
					return end;
				}
			}
		}
	}

	error_throwError(ErrorType_AssignSyntaxError, error);

	return NULL;
}

/***
	If={IfTerm,EleseIfTerm,EleseBlock}
	Ifterm={Expr,IfBlock}
	EleseIfTerm={Expr,EleseIfBlock}
****/
static Token *parseIfState(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *start=NULL;

	ASTNode *ifNode=NULL;
	ASTNode *termNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	int termLen=0;
	int blockLen=0;
	int len=0;

	end=__boundIfState(token);
	if(!end){
		error_throwError(ErrorType_IfSyntaxError, token);
		return NULL;
	}

	termLen=__calIfConditionLength(token,end);
	blockLen=__calIfBlockLength(token,end);

	ifNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
	ifNode->type=ASTType_If;

	ifNode->childLength=blockLen;
	ifNode->childArr=(ASTNode **)calloc(blockLen, sizeof(ASTNode *));

	// if
	start=token->next;
	if(start->subType==TokenSubType_LeftParenth){ // (
		end=__boundParenth(start);
		if(!end){
			error_throwError(ErrorType_IfSyntaxError, start);
			return NULL;
		}

		_token=start->next;
		start=parseExpr(start->next, end, NULL, &exprNode);
		if(!start){
			error_throwError(ErrorType_IfSyntaxError, _token);
			freeASTNode(exprNode);
			return NULL;
		}

		start=start->next;
		if(start->subType==TokenSubType_LeftBrace){ // {
			end=__boundBrace(start);
			if(!end){
				error_throwError(ErrorType_IfSyntaxError, start);
				freeASTNode(exprNode);
				return NULL;
			}

			end=parseBlock(start,end,&blockNode);
			if(!end){
				// error_throwError(ErrorType_ExprSyntaxError, start);

				freeASTNode(exprNode);
				freeASTNode(blockNode);

				return NULL;
			}

			if(end){ // term
				termNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
				termNode->type=ASTType_Expr;
				termNode->op=ASTOp_IfTerm;

				termNode->childLength=2;
				termNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

				termNode->childArr[0]=exprNode;
				exprNode->parent=termNode;
				termNode->childArr[1]=blockNode;
				blockNode->parent=termNode;
				blockNode->op=ASTOp_IfBlock;

				ifNode->childArr[0]=termNode;
				termNode->parent=ifNode;

				len++;
				start=end;
				*node=ifNode;
			}
		}
	}

	while(start){ // else if/else
		if(start->subType==TokenSubType_Else){
			start=start->next;
			if(start->subType==TokenSubType_LeftBrace){ // else {
				end=__boundBrace(start);
				if(!end){
					error_throwError(ErrorType_IfSyntaxError, start);

					freeASTNode(exprNode);
					freeASTNode(blockNode);

					return NULL;
				}

				end=parseBlock(start,end,&blockNode);
				if(!end){
					// error_throwError(ErrorType_ExprSyntaxError, start);

					freeASTNode(exprNode);
					freeASTNode(blockNode);

					return NULL;
				}

				if(end){ // block
					ifNode->childArr[len]=blockNode;
					blockNode->parent=ifNode;
					blockNode->op=ASTOp_ElseBlock;

					len++;
					start=end;
					*node=ifNode;
				}
			}
			else if(start->subType==TokenSubType_If){ // else if
				start=start->next;
				if(start->subType==TokenSubType_LeftParenth){
					end=__boundParenth(start);
					if(!end){
						error_throwError(ErrorType_IfSyntaxError, start);

						freeASTNode(exprNode);
						freeASTNode(blockNode);

						return NULL;
					}

					_token=start->next;
					start=parseExpr(start->next, end, NULL, &exprNode);
					if(!start){
						error_throwError(ErrorType_IfSyntaxError, _token);

						freeASTNode(exprNode);
						freeASTNode(blockNode);

						return NULL;
					}

					start=start->next;
					if(start->subType==TokenSubType_LeftBrace){ // {
						end=__boundBrace(start);
						if(!end){
							error_throwError(ErrorType_IfSyntaxError, start);

							freeASTNode(exprNode);
							freeASTNode(blockNode);

							return NULL;
						}

						end=parseBlock(start,end,&blockNode);
						if(!end){
							// error_throwError(ErrorType_ExprSyntaxError, start);

							freeASTNode(exprNode);
							freeASTNode(blockNode);

							return NULL;
						}

						if(end){ // term
							termNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
							termNode->type=ASTType_Expr;
							termNode->op=ASTOp_ElseIfTerm;

							termNode->childLength=2;
							termNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

							termNode->childArr[0]=exprNode;
							exprNode->parent=termNode;
							termNode->childArr[1]=blockNode;
							blockNode->parent=termNode;
							blockNode->op=ASTOp_ElseIfTerm;

							ifNode->childArr[len]=termNode;
							termNode->parent=ifNode;

							len++;
							start=end;
							*node=ifNode;
						}
					}
				}
			}
			else{ // error
				error_throwError(ErrorType_IfSyntaxError, start);

				freeASTNode(exprNode);
				freeASTNode(blockNode);
				freeASTNode(ifNode);

				return NULL;
			}
		}
		else{
			break;
		}
	}

	if(!end||len!=blockLen){
		error_throwError(ErrorType_IfSyntaxError, error);

		freeASTNode(exprNode);
		freeASTNode(blockNode);
		freeASTNode(ifNode);

		return NULL;
	}
	
	return end;
}

/***
	While={WhileTerm,WhileBlock}
	WhileTerm={Expr}
****/
static Token *parseWhileState(Token *token,ASTNode **node){
	Token *end=NULL;

	Token *start=NULL;

	ASTNode *whileNode=NULL;
	ASTNode *termNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;

	start=token->next;
	if(start->subType==TokenSubType_LeftParenth){
		end=__boundParenth(start);
		if(!end){
			error_throwError(ErrorType_WhileSyntaxError, start);
			return NULL;
		}

		if(end){
			termNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
			termNode->type=ASTType_Expr;
			termNode->op=ASTOp_WhileTerm;

			termNode->childLength=1;
			termNode->childArr=(ASTNode **)calloc(1, sizeof(ASTNode *));

			end=parseExpr(start->next, end, NULL, &exprNode);
			if(!end){
				error_throwError(ErrorType_WhileSyntaxError, start->next);
				return NULL;
			}

			if(end){
				termNode->childArr[0]=exprNode;
				exprNode->parent=termNode;

				start=end->next;
				if(start->subType==TokenSubType_LeftBrace){

					end=__boundBrace(start);
					if(!end){
						error_throwError(ErrorType_WhileSyntaxError, start);
						return NULL;
					}

					if(end){
						end=parseBlock(start,end,&blockNode);
						if(!end){
							// error_throwError(ErrorType_ExprSyntaxError, start);
							return NULL;
						}

						if(end){
							whileNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
							whileNode->type=ASTType_While;

							whileNode->childLength=2;
							whileNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

							whileNode->childArr[0]=termNode;
							termNode->parent=whileNode;

							whileNode->childArr[1]=blockNode;
							blockNode->op=ASTOp_WhileBlock;
							blockNode->parent=whileNode;

							*node=whileNode;
							return end;
						}
					}
				}
			}
		}
	}

	error_throwError(ErrorType_WhileSyntaxError, error);

	return NULL;
}

/***
	For={ForTerm,ForBlock}
	ForTerm={Expr,Expr}
	ForTerm={Assign,Expr,Assign}
****/
static Token *parseForState(Token *token,ASTNode **node){
	Token *end=NULL;

	Token *inToken=NULL;

	end=__boundParenth(token);
	if(!end){
		error_throwError(ErrorType_ForSyntaxError, token);
		return NULL;
	}
	inToken=__boundIn(token->next->next, end);
	
	if(inToken){
		end=__parseForInState(token,inToken,node);
	}
	else{
		end=__parseForThreeState(token, node);
	}

	return end;
}

static Token *__parseForInState(Token *token,Token *inToken,ASTNode **node){
	Token *end=NULL;

	Token *start=NULL;

	ASTNode *forNode=NULL;
	ASTNode *termNode=NULL;
	ASTNode *exprNode1=NULL;
	ASTNode *exprNode2=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	start=token->next;
	if(start->subType==TokenSubType_LeftParenth){ // (
		end=__boundParenth(start);
		if(!end){
			error_throwError(ErrorType_ForSyntaxError, start);
			return NULL;
		}

		if(end){
			termNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
			termNode->type=ASTType_Expr;
			termNode->op=ASTOp_ForTerm;

			termNode->childLength=2;
			termNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

			token=start->next;
			start=inToken;
			
			_token=token;
			// token=parseExpr(token, start, NULL, &exprNode1);
			token=__parseComma(token, start, 2, &exprNode1);
			if(!token){
				error_throwError(ErrorType_ForSyntaxError, _token);
				return NULL;
			}

			if(token){

				// end=parseExpr(start->next, end, NULL, &exprNode2);
				end=__parseComma(start->next, end, 0, &exprNode2);
				if(!end){
					error_throwError(ErrorType_ForSyntaxError, start->next);
					return NULL;
				}

				if(exprNode1->childLength!=exprNode2->childLength){
					error_throwError(ErrorType_ForSyntaxError, start->next);
					return NULL;
				}

				if(end){
					termNode->childArr[0]=exprNode1;
					exprNode1->parent=termNode;
					termNode->childArr[1]=exprNode2;
					exprNode2->parent=termNode;

					start=end->next;
					if(start->subType==TokenSubType_LeftBrace){ // {

						end=__boundBrace(start);
						if(!end){
							error_throwError(ErrorType_ForSyntaxError, start);
							return NULL;
						}

						if(end){

							end=parseBlock(start,end,&blockNode);
							if(!end){
								// error_throwError(ErrorType_ExprSyntaxError, start);
								return NULL;
							}

							if(end){
								forNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
								forNode->type=ASTType_For;

								forNode->childLength=2;
								forNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

								forNode->childArr[0]=termNode;
								termNode->parent=forNode;

								forNode->childArr[1]=blockNode;
								blockNode->op=ASTOp_ForBlock;
								blockNode->parent=forNode;

								*node=forNode;
								return end;
							}
						}
					}
				}
			}
		}
	}
	
	error_throwError(ErrorType_ForSyntaxError, error);

	return NULL;
}

static Token *__parseForThreeState(Token *token,ASTNode **node){
	Token *end=NULL;

	Token *start=NULL;

	ASTNode *forNode=NULL;
	ASTNode *termNode=NULL;
	ASTNode *exprNode1=NULL;
	ASTNode *exprNode2=NULL;
	ASTNode *exprNode3=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	start=token->next;
	if(start->subType==TokenSubType_LeftParenth){ // (
		end=__boundParenth(start);
		if(!end){
			error_throwError(ErrorType_ForSyntaxError, start);
			return NULL;
		}

		if(end){
			termNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
			termNode->type=ASTType_Expr;
			termNode->op=ASTOp_ForTerm;

			termNode->childLength=3;
			termNode->childArr=(ASTNode **)calloc(3, sizeof(ASTNode *));

			token=start->next;
			
			_token=token;
			token=parseAssignState(token, &exprNode1);
			token=filterEmpty(token);
			if(!token){
				error_throwError(ErrorType_ForSyntaxError, _token);
				return NULL;
			}

			if(token){

				_token=token;
				token=parseExprState(token, &exprNode2);
				token=filterEmpty(token);
				if(!token){
					error_throwError(ErrorType_ForSyntaxError, _token);
					return NULL;
				}

				if(token){
					Token *_token=NULL;
					Token *_pre=NULL;

					if(end->pre->subType!=TokenSubType_Semicolon){ // ;
						_token=(Token *)calloc(1, sizeof(Token ));
						_pre=end->pre;

						_token->type=TokenType_Delimiter;
						_token->subType=TokenSubType_Semicolon;
						_token->row=_pre->row;
						_token->column=_pre->column;

						_pre->next=_token;
						_token->pre=_pre;
						_token->next=end;
						end->pre=_pre;
					}

					end=parseAssignState(token,&exprNode3);
					end=filterEmpty(end);
					if(!end){
						error_throwError(ErrorType_ForSyntaxError, token);
						return NULL;
					}

					if(end){
						termNode->childArr[0]=exprNode1;
						exprNode1->parent=termNode;
						termNode->childArr[1]=exprNode2;
						exprNode2->parent=termNode;
						termNode->childArr[2]=exprNode3;
						exprNode3->parent=termNode;

						start=end->next;
						if(start->subType==TokenSubType_LeftBrace){ // {

							end=__boundBrace(start);
							if(!end){
								error_throwError(ErrorType_ForSyntaxError, start);
								return NULL;
							}

							if(end){

								end=parseBlock(start,end,&blockNode);
								if(!end){
									// error_throwError(ErrorType_ExprSyntaxError, start);
									return NULL;
								}

								if(end){
									forNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
									forNode->type=ASTType_For;

									forNode->childLength=2;
									forNode->childArr=(ASTNode **)calloc(2, sizeof(ASTNode *));

									forNode->childArr[0]=termNode;
									termNode->parent=forNode;

									forNode->childArr[1]=blockNode;
									blockNode->op=ASTOp_ForBlock;
									blockNode->parent=forNode;

									*node=forNode;
									return end;
								}
							}
						}
					}
				}
			}
		}
	}
	
	error_throwError(ErrorType_ForSyntaxError, error);

	return NULL;
}

static Token *parseFunctionState(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *start=NULL;

	ASTNode *functionNode=NULL;
	ASTNode *nameNode=NULL;
	ASTNode *argNode=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundFunctionState(token);
	if(!end){
		error_throwError(ErrorType_FunctionSyntaxError, token);
		return NULL;
	}

	start=token->next;

	functionNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
	functionNode->type=ASTType_Function;

	functionNode->childLength=3;
	functionNode->childArr=(ASTNode **)calloc(3, sizeof(ASTNode *));

	if(start->type==TokenType_Identify){ // name

		_token=start;
		start=parseExpr(start, start->next, NULL, &nameNode);
		if(!start){
			error_throwError(ErrorType_FunctionSyntaxError, _token);
			return NULL;
		}
		if(nameNode->op!=ASTOp_Indentify){
			error_throwError(ErrorType_FunctionSyntaxError, _token);
			return NULL;
		}

		if(start->subType==TokenSubType_LeftParenth){ // arg

			_token=start;
			start=parseArguments(start, &argNode);
			if(!start){
				error_throwError(ErrorType_FunctionSyntaxError, _token);
				return NULL;
			}

			if(start->subType==TokenSubType_LeftBrace){ // state
				end=parseBlock(start, end, &blockNode);
				if(!end){
					// error_throwError(ErrorType_ExprSyntaxError, start);
					return NULL;
				}

				if(end){
					functionNode->childArr[0]=nameNode;
					nameNode->parent=functionNode;

					functionNode->childArr[1]=argNode;
					argNode->parent=functionNode;

					functionNode->childArr[2]=blockNode;
					blockNode->op=ASTOp_FunctionBlock;
					blockNode->parent=functionNode;

					*node=functionNode;
					return end;
				}
			}
		}
	}

	error_throwError(ErrorType_FunctionSyntaxError, error);

	return NULL;
}

// Class={Name,Expr,ClassBlock}
static Token *parseClassState(Token *token,ASTNode **node){
	Token *end=NULL;
	Token *start=NULL;

	ASTNode *classNode=NULL;
	ASTNode *nameNode=NULL;
	ASTNode *exprNode=NULL;
	ASTNode *blockNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundClassState(token);
	if(!end){
		error_throwError(ErrorType_ClassSyntaxError, token);
		return NULL;
	}

	start=token->next;

	classNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
	classNode->type=ASTType_Class;

	classNode->childLength=3;
	classNode->childArr=(ASTNode **)calloc(3, sizeof(ASTNode *));

	if(start->type==TokenType_Identify){ // name

		_token=start;
		start=parseExpr(start, start->next, NULL, &nameNode);
		if(!start){
			error_throwError(ErrorType_ClassSyntaxError, _token);
			return NULL;
		}
		if(nameNode->op!=ASTOp_Indentify){
			error_throwError(ErrorType_ClassSyntaxError, _token);
			return NULL;
		}

		if(start->subType==TokenSubType_LeftParenth||
			start->subType==TokenSubType_LeftBrace){ // (||{

			if(start->subType==TokenSubType_LeftParenth){
				Token *_end=NULL;

				_end=__boundParenth(start);
				if(!_end){
					error_throwError(ErrorType_ClassSyntaxError, start);
					return NULL;
				}

				if(start->next!=_end){

					_token=start->next;
					start=parseExpr(start->next, _end,NULL,&exprNode);
					if(!start){
						error_throwError(ErrorType_ClassSyntaxError, _token);
						return NULL;
					}
				}
				else{
					start=start->next;
				}

				start=start->next;
			}
			
			if(start->subType==TokenSubType_LeftBrace){ // block
				end=__boundBrace(start);
				if(!end){
					error_throwError(ErrorType_ClassSyntaxError, start);
					return NULL;
				}

				end=parseBlock(start, end, &blockNode);
				if(!end){
					// error_throwError(ErrorType_ExprSyntaxError, start);
					return NULL;
				}

				if(end){
					classNode->childArr[0]=nameNode;
					nameNode->parent=classNode;

					if(exprNode){
						classNode->childArr[1]=exprNode;
						exprNode->parent=classNode;
					}

					classNode->childArr[2]=blockNode;
					blockNode->op=ASTOp_ClassBlock;
					blockNode->parent=classNode;

					*node=classNode;
					return end;
				}
			}
		}
	}

	error_throwError(ErrorType_ClassSyntaxError, error);

	return NULL;
}

static Token *parseContinueState(Token *token,ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	Token *error=token;

	end=__boundContinueState(token);
	if(!end){
		error_throwError(ErrorType_ContinueSyntaxError, token);
		return NULL;
	}

	if(end==token->next){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
		curNode->type=ASTType_Continue;

		*node=curNode;
		end=end->next;
		return end;
	}

	error_throwError(ErrorType_ContinueSyntaxError, error);

	return NULL;
}

static Token *parseBreakState(Token *token,ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	Token *error=token;

	end=__boundContinueState(token);
	if(!end){
		error_throwError(ErrorType_BreakSyntaxError, token);
		return NULL;
	}

	if(end==token->next){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
		curNode->type=ASTType_Break;

		*node=curNode;
		end=end->next;
		return end;
	}

	error_throwError(ErrorType_BreakSyntaxError, error);

	return NULL;
}

static Token *parseReturnState(Token *token,ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	ASTNode *exprNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundReturnState(token);
	if(!end){
		error_throwError(ErrorType_ReturnSyntaxError, token);
		return NULL;
	}

	if(end){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
		curNode->type=ASTType_Return;
		curNode->childLength=1;
		curNode->childArr=(ASTNode **)calloc(1, sizeof(ASTNode *));

		if(token->next==end){ // return ;
			exprNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
			exprNode->type=ASTType_Expr;
			exprNode->op=ASTOp_Null;

			curNode->childArr[0]=exprNode;
			exprNode->parent=curNode;

			*node=curNode;
			end=end->next;
			return end;
		}
		else{
			_token=token->next;
			token=parseExpr(token->next, end, NULL, &exprNode);
			if(!token){
				error_throwError(ErrorType_ReturnSyntaxError, _token);
				return NULL;
			}

			if(token==end){
				curNode->childArr[0]=exprNode;
				exprNode->parent=curNode;
				
				*node=curNode;
				end=end->next;
				return end;
			}
		}
	}

	error_throwError(ErrorType_ReturnSyntaxError, error);

	return NULL;
}

static Token *parseYieldState(Token *token,ASTNode **node){
	Token *end=NULL;
	ASTNode *curNode=NULL;
	ASTNode *exprNode=NULL;
	Token *error=token;
	Token *_token=NULL;

	end=__boundYieldState(token);
	if(!end){
		error_throwError(ErrorType_YieldSyntaxError, token);
		return NULL;
	}

	if(end){
		curNode=(ASTNode *)calloc(1, sizeof(ASTNode ));
		curNode->type=ASTType_Yield;
		curNode->childLength=1;
		curNode->childArr=(ASTNode **)calloc(1, sizeof(ASTNode *));

		_token=token->next;
		token=parseExpr(token->next, end, NULL, &exprNode);
		if(!token){
			error_throwError(ErrorType_YieldSyntaxError, _token);
			return NULL;
		}

		if(token==end){
			curNode->childArr[0]=exprNode;
			exprNode->parent=curNode;
			
			*node=curNode;
			end=end->next;
			return end;
		}
	}

	error_throwError(ErrorType_YieldSyntaxError, error);

	return NULL;
}

static Token *__parseArguments(Token *token,int flag,ASTNode **node){
	Token *end=NULL;
	Token *tail=NULL;
	Token *split=NULL;

	ASTNode *cur=NULL;
	ASTNode *elemet=NULL;
	ASTNode **nodeArr=NULL;
	int length=0;
	int len=0;
	Token *_token=NULL;

	end=__boundParenth(token);
	if(!end){
		return NULL;
	}

	cur=(ASTNode *)calloc(1, sizeof(ASTNode ));
	cur->type=ASTType_Expr;
	cur->op=ASTOp_Arguments;

	length=__calArgumentsLength(token,end);
	if(length){
		nodeArr=(ASTNode **)calloc(length, sizeof(ASTNode *));
		token=token->next;
		tail=end;
		while(token!=end){

			_token=token;
			split=__boundComma(token,end);
			if(split){ 
				if(token->subType==TokenSubType_Out){
					tail=parseExpr(token->next,split,NULL,&elemet);
					if(tail){
						if(elemet->op!=ASTOp_Indentify){
							// error_throwError(ErrorType_ExprSyntaxError, token);
							return NULL;
						}
						elemet->value.outFlag=1;
					}
					else {
						// error_throwError(ErrorType_ExprSyntaxError, token);
						return NULL;
					}
				}
				else{
					tail=parseExpr(token,split,NULL,&elemet);
					if(!tail){
						// error_throwError(ErrorType_ExprSyntaxError, token);
						return NULL;
					}
				}

				token=split->next;
			}
			else{
				if(token->subType==TokenSubType_Out){
					tail=parseExpr(token->next,end,NULL,&elemet);
					if(tail){
						if(elemet->op!=ASTOp_Indentify){
							// error_throwError(ErrorType_ExprSyntaxError, token);
							return NULL;
						}
						elemet->value.outFlag=1;
					}
					else{
						// error_throwError(ErrorType_ExprSyntaxError, token);
						return NULL;
					}
				}
				else{
					tail=parseExpr(token,end,NULL,&elemet);
					if(!tail){
						// error_throwError(ErrorType_ExprSyntaxError, token);
						return NULL;
					}
				}

				token=end;
			}

			if(flag){ // state
				if(elemet->op==ASTOp_Indentify){
					nodeArr[len]=elemet;
					elemet->parent=cur;
					len++;
				}
				else{
					return NULL;
				}
			}
			else{
				nodeArr[len]=elemet;
				elemet->parent=cur;
				len++;
			}
		}
	}

	cur->childLength=length;
	cur->childArr=(ASTNode **)nodeArr;

	*node=cur;
	return end->next;
}

// length 1 expr >1 comma
static Token *__parseComma(Token *token,Token *tail,int flag,ASTNode **node){
	Token *end=NULL;
	Token *split=NULL;

	ASTNode *cur=NULL;
	ASTNode *elemet=NULL;
	ASTNode **nodeArr=NULL;
	int length=0;
	int len=0;
	int _flag=0;
	Token *_token=NULL;

	length=__calCommaCount(token,tail,1);
	if(length==1){
		cur=(ASTNode *)calloc(1, sizeof(ASTNode ));
		cur->type=ASTType_Expr;
		cur->op=ASTOp_Comma;

		nodeArr=(ASTNode **)calloc(length, sizeof(ASTNode *));

		end=parseExpr(token,tail,NULL,&elemet);
		if(end){
			if(flag){
				if(flag==1){
					if(elemet->op==ASTOp_Indentify||
						elemet->op==ASTOp_Property||
						elemet->op==ASTOp_Subscript){

						_flag=1;
					}
				}
				else{
					if(elemet->op==ASTOp_Indentify){
						_flag=1;
					}
				}
			}
			else{
				_flag=1;
			}
		}

		if(_flag){
			nodeArr[0]=elemet;
			elemet->parent=cur;

			cur->childLength=length;
			cur->childArr=(ASTNode **)nodeArr;

			*node=cur;
			return end;
		}
		else{
			return NULL;
		}
	}
	else if(length>1){
		cur=(ASTNode *)calloc(1, sizeof(ASTNode ));
		cur->type=ASTType_Expr;
		cur->op=ASTOp_Comma;

		nodeArr=(ASTNode **)calloc(length, sizeof(ASTNode *));
		end=tail;
		while(token!=end){

			_token=token;
			split=__boundComma(token,end);
			if(split){ 
				tail=parseExpr(token,split,NULL,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);
					return NULL;
				}

				token=split->next;
			}
			else{
				tail=parseExpr(token,end,NULL,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);
					return NULL;
				}

				token=end;
			}

			if(flag){ // state
				if(flag==1){
					if(elemet->op==ASTOp_Indentify||
						elemet->op==ASTOp_Property||
						elemet->op==ASTOp_Subscript){

						_flag=1;
					}
				}
				else {
					if(elemet->op==ASTOp_Indentify){
						_flag=1;
					}
				}
			}
			else{
				_flag=1;
			}

			if(_flag){
				nodeArr[len]=elemet;
				elemet->parent=cur;
				len++;
			}
			else{
				return NULL;
			}
		}

		cur->childLength=length;
		cur->childArr=(ASTNode **)nodeArr;

		*node=cur;
		return end;
	}
	
	return NULL;
}

static Token *parseArguments(Token *token,ASTNode **node){
	Token *end=NULL;

	end=__parseArguments(token,1,node);
	if(!end){
		// error_throwError(ErrorType_FunctionSyntaxError, token);
	}

	return end;
}

static int __isImportHander(ASTNode *node){
	int flag=0;

	if(node){
		if(node->op==ASTOp_Property||
			node->op==ASTOp_Indentify){
			flag=1;
		}
	}

	return flag;
}

static Token *parseImportState(Token *token,ASTNode **node){
	Token *end=NULL;

	ASTNode *cur=NULL;
	ASTNode *exprNode=NULL;
	int flag=0;
	Token *error=token;

	end=__boundImportState(token);
	if(!end){
		error_throwError(ErrorType_ImportSyntaxError, token);
		return NULL;
	}

	end=parseExpr(token->next, end, NULL, &exprNode);
	if(!end){
		error_throwError(ErrorType_ImportSyntaxError, token->next);
		freeASTNode(exprNode);
		return NULL;
	}

	if(end){
		flag=astNode_all(exprNode, __isImportHander);
		if(flag){
			cur=(ASTNode *)calloc(1, sizeof(ASTNode *));
			cur->type=ASTType_Import;
			cur->childLength=1;
			cur->childArr=(ASTNode **)calloc(1, sizeof(ASTNode ));

			cur->childArr[0]=exprNode;
			exprNode->parent=cur;

			*node=cur;
			end=end->next;
			return end;
		}
	}

	error_throwError(ErrorType_ImportSyntaxError, error);
	freeASTNode(exprNode);
	return NULL;
}

static Token *__parseSemState(Token *token,ASTNode **node,ASTType type,int flag){
	Token *end=NULL;
	Token *tail=NULL;
	Token *split=NULL;

	ASTNode *cur=NULL;
	ASTNode *elemet=NULL;
	ASTNode **nodeArr=NULL;
	int length=0;
	int len=0;

	end=__boundSemicolon(token);
	if(!end){
		return NULL;
	}

	length=__calArgumentsLength(token,end);
	if(length){
		cur=(ASTNode *)calloc(1, sizeof(ASTNode ));
		cur->type=type;

		nodeArr=(ASTNode **)calloc(length, sizeof(ASTNode *));
		token=token->next;
		tail=end;
		while(token!=end){
			split=__boundComma(token,end);
			if(split){ 
				tail=parseExpr(token,split,NULL,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);
					return NULL;
				}

				token=split->next;
			}
			else{
				tail=parseExpr(token,end,NULL,&elemet);
				if(!tail){
					// error_throwError(ErrorType_ExprSyntaxError, token);
					return NULL;
				}

				token=end;
			}

			if(flag){
				if(elemet->op==ASTOp_Indentify){
					nodeArr[len]=elemet;
					elemet->parent=cur;
					len++;
				}
				else{
					return NULL;
				}
			}
			else{
				nodeArr[len]=elemet;
				elemet->parent=cur;
				len++;
			}
		}

		cur->childLength=length;
		cur->childArr=(ASTNode **)nodeArr;

		*node=cur;
		end=end->next;
		return end;
	}
	
	return NULL;
}

static Token *parseUpState(Token *token,ASTNode **node){
	Token *end=NULL;

	end=__parseSemState(token, node, ASTType_Up, 1);
	if(!end){
		error_throwError(ErrorType_UpSyntaxError, token);
	}

	return end;
}

static Token *parseGlobalState(Token *token,ASTNode **node){
	Token *end=NULL;

	end=__parseSemState(token, node, ASTType_Global, 1);
	if(!end){
		error_throwError(ErrorType_GlobalSyntaxError, token);
	}

	return end;
}

static Token *parseFieldState(Token *token,ASTNode **node){
	Token *end=NULL;

	end=__parseSemState(token, node, ASTType_Field, 1);
	if(!end){
		error_throwError(ErrorType_FieldSyntaxError, token);
	}

	return end;
}

static Token *parseStaticState(Token *token,ASTNode **node){
	Token *end=NULL;
	ASTNode *cur=NULL;
	ASTNode *element=NULL;
	Token *error=token;

	if(token->next){
		token=token->next;
		if(token->subType==TokenSubType_Field){
			end=parseFieldState(token, &element);
		}
		else if(token->subType==TokenSubType_Def){
			end=parseFunctionState(token, &element);
		}
		else{
			error_throwError(ErrorType_StaticSyntaxError, error);
		}

		if(end){
			cur=(ASTNode *)calloc(1,sizeof(ASTNode ));
			cur->type=ASTType_Static;
			cur->childLength=1;
			cur->childArr=(ASTNode **)calloc(1, sizeof(ASTNode *));

			cur->childArr[0]=element;
			element->parent=cur;

			*node=cur;
		}

	}
	else{
		error_throwError(ErrorType_StaticSyntaxError, error);
	}

	return end;
}

static Token *parsePrintState(Token *token,ASTNode **node){
	Token *end=NULL;

	end=__parseSemState(token, node, ASTType_Print, 0);
	if(!end){
		error_throwError(ErrorType_PrintSyntaxError, token);
	}

	return end;
}	

void freeASTNode(ASTNode *node){

	if(node){
		free(node->data);
		wrap_release(node->wrap);

		for(int i=0;i<node->childLength;i++){
			freeASTNode(node->childArr[i]);
		}
		free(node->childArr);

		freeASTNode(node->next);

		free(node);
	}
}





