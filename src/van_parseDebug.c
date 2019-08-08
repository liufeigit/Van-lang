// clang 

#include "van_parse.h"

static void __debugASTNode(ASTNode *node,int n);

void debugASTNode(ASTNode *node){
	printf(">>> astNodeDebug start:\n");

	__debugASTNode(node,0);

	printf(">>> tokenDebug end\n");
}

static void __debugASTNode(ASTNode *node,int n){
	int childLength=0;
	ASTNode **childArr=NULL;

	char *str=NULL;

	str=(char *)calloc(n*4+1, sizeof(char ));
	for(int i=0;i<n*4;i++){
		str[i]='-';
	}
	while(node){
		childLength=node->childLength;
		childArr=node->childArr;

		printf("%s %d:",str,n);
		
		if(node->op){
			if(node->op==ASTOp_IfTerm){
				printf("ifTerm");
			}
			else if(node->op==ASTOp_ForTerm){
				printf("forTerm");
			}
			else if(node->op==ASTOp_WhileTerm){
				printf("whileTerm");
			}
			else if(node->op==ASTOp_Comma){
				printf("comma");
			}
			else if(node->op==ASTOp_Arguments){
				printf("arguments");
			}
			else if(node->op==ASTOp_Pair){
				printf("pair");
			}
			else if(node->op==ASTOp_Slice){
				printf("slice");
			}
			else if(node->op==ASTOp_IfBlock){
				printf("ifBlock");
			}
			else if(node->op==ASTOp_IfElseBlock){
				printf("ifElseBlock");
			}
			else if(node->op==ASTOp_ElseBlock){
				printf("elseBlock");
			}
			else if(node->op==ASTOp_ForBlock){
				printf("forBlock");
			}
			else if(node->op==ASTOp_WhileBlock){
				printf("whileBlock");
			}
			else if(node->op==ASTOp_FunctionBlock){
				printf("functionBlock");
			}
			else if(node->op==ASTOp_ClassBlock){
				printf("classBlock");
			}
			else if(node->op==ASTOp_DeclareAssign){
				printf("=");
			}
			else if(node->op==ASTOp_MulAssgin){
				printf("*=");
			}
			else if(node->op==ASTOp_DivAssgin){
				printf("/=");
			}
			else if(node->op==ASTOp_ModAssgin){
				printf("%c=",'%');
			}
			else if(node->op==ASTOp_AddAssgin){
				printf("+=");
			}
			else if(node->op==ASTOp_SubAssgin){
				printf("-=");
			}
			else if(node->op==ASTOp_LeftShiftAssgin){
				printf("<<=");
			}
			else if(node->op==ASTOp_RightShiftAssgin){
				printf(">>=");
			}
			else if(node->op==ASTOp_XOrAssgin){
				printf("|=");
			}
			else if(node->op==ASTOp_XAndAssgin){
				printf("&=");
			}
			else if(node->op==ASTOp_XErAssgin){
				printf("^=");
			}
			else if(node->op==ASTOp_Indentify){
				printf("%s",node->data);
				if(node->value.outFlag){
					printf(" out");
				}
			}
			else if(node->op==ASTOp_Int){
				printf("%lld",node->value.l);
			}
			else if(node->op==ASTOp_Float){
				printf("%f",node->value.d);
			}
			else if(node->op==ASTOp_Null){
				printf("null");
			}
			else if(node->op==ASTOp_True){
				printf("true");
			}
			else if(node->op==ASTOp_False){
				printf("false");
			}
			else if(node->op==ASTOp_String){
				printf("\"%s\"",node->data);
			}
			else if(node->op==ASTOp_Self){
				printf("self");
			}
			else if(node->op==ASTOp_Super){
				printf("super");
			}
			else if(node->op==ASTOp_Map){
				printf("map");
			}
			else if(node->op==ASTOp_List){
				printf("list");
			}
			else if(node->op==ASTOp_FunctionExpr){
				printf("functionExpr");
			}
			else if(node->op==ASTOp_ClassExpr){
				printf("classExpr");
			}
			else if(node->op==ASTOp_Prior){
				printf("()");
			}
			else if(node->op==ASTOp_Call){
				printf("call");
			}
			else if(node->op==ASTOp_Property){
				printf(".");
			}
			else if(node->op==ASTOp_Subscript){
				printf("[]");
			}
			else if(node->op==ASTOp_Not){
				printf("!");
			}
			else if(node->op==ASTOp_Reverse){
				printf("~");
			}
			else if(node->op==ASTOp_New){
				printf("new");
			}
			else if(node->op==ASTOp_Negative){
				printf("-");
			}
			else if(node->op==ASTOp_Mul){
				printf("*");
			}
			else if(node->op==ASTOp_Div){
				printf("/");
			}
			else if(node->op==ASTOp_Mod){
				printf("%c",'%');
			}
			else if(node->op==ASTOp_Add){
				printf("+");
			}
			else if(node->op==ASTOp_Sub){
				printf("-");
			}
			else if(node->op==ASTOp_LeftShift){
				printf("<<");
			}
			else if(node->op==ASTOp_RightShift){
				printf(">>");
			}
			else if(node->op==ASTOp_GR){
				printf(">");
			}
			else if(node->op==ASTOp_GE){
				printf(">=");
			}
			else if(node->op==ASTOp_LS){
				printf("<");
			}
			else if(node->op==ASTOp_LE){
				printf("<=");
			}
			else if(node->op==ASTOp_EQ){
				printf("==");
			}
			else if(node->op==ASTOp_NE){
				printf("!=");
			}
			else if(node->op==ASTOp_XOr){
				printf("|");
			}
			else if(node->op==ASTOp_XAnd){
				printf("&");
			}
			else if(node->op==ASTOp_XEr){
				printf("^");
			}
			else if(node->op==ASTOp_And){
				printf("&&");
			}
			else if(node->op==ASTOp_Or){
				printf("||");
			}
			else if(node->op==ASTOp_Three){
				printf("?:");
			}
		}
		else{
			if(node->type==ASTType_Expr){
				printf("expr");
			}
			else if(node->type==ASTType_Assign){
				printf("assign");
			}
			else if(node->type==ASTType_If){
				printf("if");
			}
			else if(node->type==ASTType_While){
				printf("while");
			}
			else if(node->type==ASTType_For){
				printf("for");
			}
			else if(node->type==ASTType_Function){
				printf("function");
			}
			else if(node->type==ASTType_Class){
				printf("class");
			}
			else if(node->type==ASTType_Continue){
				printf("continue");
			}
			else if(node->type==ASTType_Break){
				printf("break");
			}
			else if(node->type==ASTType_Return){
				printf("return");
			}
			else if(node->type==ASTType_Up){
				printf("up");
			}
			else if(node->type==ASTType_Global){
				printf("global");
			}
			else if(node->type==ASTType_Field){
				printf("field");
			}
			else if(node->type==ASTType_Static){
				printf("static");
			}
			else if(node->type==ASTType_Import){
				printf("import");
			}
			else if(node->type==ASTType_Yield){
				printf("yield");
			}
			else if(node->type==ASTType_Print){
				printf("print");
			}
			else if(node->type==ASTType_Block){
				printf("block");
			}
		}

		printf("\n");

		for(int i=0;i<childLength;i++){
			__debugASTNode(childArr[i],n+1);
		}

		node=node->next;
	}
	free(str);
}