// clang 
#include "van_lexUtil.h"

Token *filterEmpty(Token *token){
	Token *cur=NULL;

	cur=token;
	while(cur){
		if(cur->subType!=TokenSubType_Semicolon){
			break;
		}
		cur=cur->next;
	}

	return cur;
}

int __calCommaCount(Token *token,Token *end,int flag){
	int length=0;
	Token *start=NULL;

	start=token;
	if(!flag){
		token=token->next;
	}
	while(token!=end){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_Comma){ //,
			length++;
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBracket){ // []
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBracket);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftParenth){ // ()
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightParenth);
			if(!token){
				break;
			}
		}

		token=token->next;
	}

	length++;
	if(!flag){
		if(start->next==end){
			length=0;
		}
	}

	return length;
}

int __calMapLength(Token *start,Token *end){
	int length=0;

	length=__calCommaCount(start,end,0);

	return length;
}

int __calListLength(Token *start,Token *end){
	int length=0;

	length=__calCommaCount(start,end,0);

	return length;
}

int __calArgumentsLength(Token *start,Token *end){
	int length=0;

	length=__calCommaCount(start,end,0);

	return length;
}

int __calIfConditionLength(Token *start,Token *end){
	int length=0;
	Token *token=NULL;

	token=start;
	end=end->next;
	while(token!=end){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftParenth){ // ()
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightParenth);
			if(!token){
				break;
			}
			length++;
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}

		token=token->next;
	}

	return length;
}

int __calIfBlockLength(Token *start,Token *end){
	int length=0;
	Token *token=NULL;

	token=start;
	end=end->next;
	while(token!=end){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftParenth){ // ()
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightParenth);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
			length++;
		}

		token=token->next;
	}

	return length;
}

Token *__leftRight(Token *token,TokenType type,TokenSubType subType){
	Token *right=NULL;
	Token *next=NULL;

	TokenType t1;
	TokenSubType st1;

	if(token){
		t1=token->type;
		st1=token->subType;
		while(token->next){
			next=token->next;
			if(next->type==type&&next->subType==subType){ // 匹配
				right=next;
				break;
			}
			else if(next->type==t1&&next->subType==st1){ // 递归
				next=__leftRight(next,type,subType);
				if(!next){
					break;
				}
			}

			token=next;
		}
	}
	
	return right;
}

int __isExprState(Token *token){
	int flag=1;



	return flag;
}

int __isAssignState(Token *token){
	int flag=0;
	Token *tail=NULL;
	Token *end=NULL;

	tail=__boundSemicolon(token);
	if(!tail){
		return flag;
	}

	end=__boundAssign(token,tail);

	if(end){
		flag=1;
	}

	return flag;
}

int __isIfState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_If){
		flag=1;
	}

	return flag;
}

int __isWhileState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_While){
		flag=1;
	}

	return flag;
}

int __isForState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_For){
		flag=1;
	}

	return flag;
}

int __isFunctionState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Def){

		if(token->next){
			token=token->next;
			if(token->subType!=TokenSubType_LeftParenth&&
				token->subType!=TokenSubType_LeftBrace){ // TokenType_Identify
				flag=1;
			}
		}
	}

	return flag;
}

int __isClassState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Class){

		if(token->next){
			token=token->next;
			if(token->subType!=TokenSubType_LeftParenth&&
				token->subType!=TokenSubType_LeftBrace){ // TokenType_Identify
				flag=1;
			}
		}
	}

	return flag;
}

int __isContinueState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Continue){
		flag=1;
	}

	return flag;
}

int __isBreakState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Break){
		flag=1;
	}

	return flag;
}

int __isReturnState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Return){
		flag=1;
	}

	return flag;
}

int __isUpState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Up){
		flag=1;
	}

	return flag;
}

int __isGlobalState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Global){
		flag=1;
	}

	return flag;
}

int __isFieldState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Field){
		flag=1;
	}

	return flag;
}

int __isYieldState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Yield){
		flag=1;
	}

	return flag;
}

int __isStaticState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Static){
		flag=1;
	}

	return flag;
}

int __isImportState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Import){
		flag=1;
	}

	return flag;
}

int __isPrintState(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Print){
		flag=1;
	}

	return flag;
}

// 单值
int __isIdentifiExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Identify){
		flag=1;
	}

	return flag;
}

int __isNumberExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Literal&&
		(token->subType==TokenSubType_Int||
			token->subType==TokenSubType_Float)){
		flag=1;
	}

	return flag;
}

int __isNullExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Literal&&
		token->subType==TokenSubType_Null){
		flag=1;
	}

	return flag;
}

int __isTrueExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Literal&&
		token->subType==TokenSubType_True){
		flag=1;
	}

	return flag;
}

int __isFalseExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Literal&&
		token->subType==TokenSubType_False){
		flag=1;
	}

	return flag;
}

int __isSelfExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Self){
		flag=1;
	}

	return flag;
}

int __isSuperExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Super){
		flag=1;
	}

	return flag;
}

int __isStringExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Literal&&
		token->subType==TokenSubType_String){
		flag=1;
	}

	return flag;
}

// 复合
int __isMapExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Delimiter&&
		token->subType==TokenSubType_LeftBrace){
		flag=1;
	}

	return flag;
}

int __isListExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Delimiter&&
		token->subType==TokenSubType_LeftBracket){
		flag=1;
	}

	return flag;
}

int __isFunctionExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Def){

		if(token->next){
			token=token->next;
			if(token->subType==TokenSubType_LeftParenth||
				token->subType==TokenSubType_LeftBrace){
				flag=1;
			}
		}
	}

	return flag;
}

int __isClassExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Keyword&&
		token->subType==TokenSubType_Class){

		if(token->next){
			token=token->next;
			if(token->subType==TokenSubType_LeftParenth||
				token->subType==TokenSubType_LeftBrace){
				flag=1;
			}
		}
	}

	return flag;
}

// 递归
int __isPriorExpr(Token *token){
	int flag=0;

	if(token->type==TokenType_Delimiter&&
		token->subType==TokenSubType_LeftParenth){
		flag=1;
	}

	return flag;
}

int __isNegativeExpr(Token *token){
	int flag=0;

	if(token->subType==TokenSubType_Sub){
		flag=1;
	}

	return flag;
}

int __isNewExpr(Token *token){
	int flag=0;

	if(token->subType==TokenSubType_New){
		flag=1;
	}

	return flag;
}

int __isReverseExpr(Token *token){
	int flag=0;

	if(token->subType==TokenSubType_Reverse){
		flag=1;
	}

	return flag;
}

int __isNotExpr(Token *token){
	int flag=0;

	if(token->subType==TokenSubType_Not){
		flag=1;
	}

	return flag;
}

Token *__boundBrace(Token *token){
	Token *end=NULL;
	Token *left=NULL;

	while(token){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {
			left=token;
			break;	
		}
		token=token->next;
	}

	if(left){
		end=__leftRight(left,TokenType_Delimiter,TokenSubType_RightBrace);
	}
	
	return end;
}

Token *__boundParenth(Token *token){
	Token *end=NULL;
	Token *left=NULL;

	while(token){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftParenth){ // (
			left=token;
			break;	
		}
		token=token->next;
	}

	if(left){
		end=__leftRight(left,TokenType_Delimiter,TokenSubType_RightParenth);
	}
	
	return end;
}

Token *__boundBracket(Token *token){
	Token *end=NULL;
	Token *left=NULL;

	while(token){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBracket){ // [
			left=token;
			break;	
		}
		token=token->next;
	}

	if(left){
		end=__leftRight(left,TokenType_Delimiter,TokenSubType_RightBracket);
	}
	
	return end;
}

Token *__boundSemicolon(Token *token){
	Token *end=NULL;

	while(token){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_Semicolon){ // ;
			end=token;
			break;	
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}

		token=token->next;
	}
	
	return end;
}

Token *__boundIn(Token *token,Token *tail){
	Token *end=NULL;

	while(token!=tail){
		if(token->type==TokenType_Keyword&&
			token->subType==TokenSubType_In){ // ;
			end=token;
			break;	
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBracket){ // []
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBracket);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftParenth){ // ()
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightParenth);
			if(!token){
				break;
			}
		}

		token=token->next;
	}
	
	return end;
}

Token *__boundAssign(Token *token,Token *tail){
	Token *end=NULL;

	while(token!=tail){
		if(token->subType>TokenSubType_AssignStart&&
			token->subType<TokenSubType_AssignEnd){ // = += *= ...
			end=token;
			break;	
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}

		token=token->next;
	}
	
	return end;
}

// ,
Token *__boundComma(Token *token,Token *tail){
	Token *end=NULL;

	while(token!=tail){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_Comma){ //,
			end=token;
			break;	
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBracket){ // []
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBracket);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftParenth){ // ()
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightParenth);
			if(!token){
				break;
			}
		}

		token=token->next;
	}
	
	return end;
}

// :
Token *__boundColon(Token *token,Token *tail){
	Token *end=NULL;

	while(token!=tail){
		if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_Colon){ // :
			end=token;
			break;	
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBrace){ // {}
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBrace);
			if(!token){
				break;
			}
		}
		else if(token->type==TokenType_Delimiter&&
			token->subType==TokenSubType_LeftBracket){ // []
			token=__leftRight(token,TokenType_Delimiter,TokenSubType_RightBracket);
			if(!token){
				break;
			}
		}

		token=token->next;
	}
	
	return end;
}

Token *__boundMapExpr(Token *token){
	Token *end=NULL;

	end=__boundBrace(token);

	return end;
}

Token *__boundListExpr(Token *token){
	Token *end=NULL;

	end=__boundBracket(token);

	return end;
}

Token *__boundFunctionExpr(Token *token){
	Token *end=NULL;
	
	end=__boundBrace(token);

	return end;
}

Token *__boundClassExpr(Token *token){
	Token *end=NULL;
	
	end=__boundBrace(token);

	return end;
}

Token *__boundMonoExpr(Token *token){
	Token *end=NULL;
	
	while(token){
		if(!(token->subType==TokenSubType_Sub||
			token->subType==TokenSubType_Reverse||
			token->subType==TokenSubType_Not||
			token->subType==TokenSubType_New)){

			end=token->pre;
			break;
		}
		else{
			token=token->next;
		}
	}

	return end;
}

Token *__boundExprState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundAssignState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundIfState(Token *token){
	Token *end=NULL;
	
	// if 
	token=__boundParenth(token);
	if(!token){
		return NULL;
	}

	token=__boundBrace(token->next);
	if(!token){
		return NULL;
	}

	if(token->next){
		token=token->next;
		while(token){
			if(token->subType==TokenSubType_Else&&
				token->next){
				if(token->next->subType==TokenSubType_If){ // else if
					token=__boundParenth(token);
					if(token){
						token=__boundBrace(token->next);
						if(token){
							token=token->next;
						}
						else{
							return NULL;
						}
					}
					else{
						return NULL;
					}
				}
				else{ // else
					token=__boundBrace(token);
					if(token){
						token=token->next;
					}
					else{
						return NULL;
					}
				}
			}
			else{
				break;
			}
		}

		end=token->pre;
	}
	
	return end;
}

Token *__boundWhileState(Token *token){
	Token *end=NULL;
	
	token=__boundParenth(token);
	if(!token){
		return NULL;
	}

	end=__boundBrace(token);
	if(!end){
		return end;
	}

	return end;
}

Token *__boundForState(Token *token){
	Token *end=NULL;
	
	token=__boundParenth(token);
	if(!token){
		return NULL;
	}

	end=__boundBrace(token);
	if(!end){
		return end;
	}

	return end;
}

Token *__boundFunctionState(Token *token){
	Token *end=NULL;
	
	// token=__boundParenth(token);
	end=__boundBrace(token);

	return end;
}

Token *__boundClassState(Token *token){
	Token *end=NULL;
	
	// token=__boundParenth(token);
	end=__boundBrace(token);

	return end;
}

Token *__boundContinueState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundBreakState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundReturnState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundUpState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundGlobalState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundFieldState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundYieldState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundImportState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}

Token *__boundPrintState(Token *token){
	Token *end=NULL;
	
	end=__boundSemicolon(token);

	return end;
}




