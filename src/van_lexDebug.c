// clang 

#include "van_lex.h"

void lexDebug(Token *token){
	printf(">>> lexDebug start:\n");

	while(token){
		printf(">>> %d:%d=",token->row+1,token->column+1);

		if(token->type==TokenType_Identify){
			printf("I  ");
			if(token->data){
				printf("%s",token->data);
			}
		}
		else if(token->type==TokenType_Keyword){
			printf("K  ");
			if(token->subType==TokenSubType_If){
				printf("if");
			}
			else if(token->subType==TokenSubType_In){
				printf("in");
			}
			else if(token->subType==TokenSubType_Is){
				printf("is");
			}
			else if(token->subType==TokenSubType_Up){
				printf("up");
			}
			else if(token->subType==TokenSubType_Def){
				printf("def");
			}
			else if(token->subType==TokenSubType_New){
				printf("new");
			}
			else if(token->subType==TokenSubType_For){
				printf("for");
			}
			else if(token->subType==TokenSubType_Var){
				printf("var");
			}
			else if(token->subType==TokenSubType_Out){
				printf("out");
			}
			else if(token->subType==TokenSubType_End){
				printf("end");
			}
			else if(token->subType==TokenSubType_Ref){
				printf("ref");
			}
			else if(token->subType==TokenSubType_Self){
				printf("self");
			}
			else if(token->subType==TokenSubType_Base){
				printf("base");
			}
			else if(token->subType==TokenSubType_Else){
				printf("else");
			}
			else if(token->subType==TokenSubType_Super){
				printf("super");
			}
			else if(token->subType==TokenSubType_Break){
				printf("break");
			}
			else if(token->subType==TokenSubType_Field){
				printf("field");
			}
			else if(token->subType==TokenSubType_Yield){
				printf("yield");
			}
			else if(token->subType==TokenSubType_Print){
				printf("print");
			}
			else if(token->subType==TokenSubType_Return){
				printf("return");
			}
			else if(token->subType==TokenSubType_Static){
				printf("static");
			}
			else if(token->subType==TokenSubType_Global){
				printf("global");
			}
			else if(token->subType==TokenSubType_Import){
				printf("import");
			}
			else if(token->subType==TokenSubType_Export){
				printf("export");
			}
			else if(token->subType==TokenSubType_Extern){
				printf("extern");
			}
			else if(token->subType==TokenSubType_Declare){
				printf("declare");
			}
			else if(token->subType==TokenSubType_Include){
				printf("include");
			}
			else if(token->subType==TokenSubType_Nataive){
				printf("nataive");
			}
			else if(token->subType==TokenSubType_Continue){
				printf("continue");
			}
			else if(token->subType==TokenSubType_Instance){
				printf("instance");
			}
		}
		else if(token->type==TokenType_Literal){
			printf("L  ");
			if(token->subType==TokenSubType_String){
				if(token->data){
					printf("\"%s\"",token->data);
				}
				
			}
			else if(token->subType==TokenSubType_Int){
				printf("%lld",token->value.l);
			}
			else if(token->subType==TokenSubType_Float){
				printf("%f",token->value.d);
			}
			else if(token->subType==TokenSubType_True){
				printf("true");
			}
			else if(token->subType==TokenSubType_False){
				printf("false");
			}
			else if(token->subType==TokenSubType_Null){
				printf("null");
			}
		}
		else if(token->type==TokenType_Operator){
			printf("O  ");
			if(token->subType==TokenSubType_Assign){
				printf("=");
			}
			else if(token->subType==TokenSubType_MulAssgin){
				printf("*=");
			}
			else if(token->subType==TokenSubType_DivAssgin){
				printf("/=");
			}
			else if(token->subType==TokenSubType_ModAssgin){
				printf("%c=",'%');
			}
			else if(token->subType==TokenSubType_AddAssgin){
				printf("+=");
			}
			else if(token->subType==TokenSubType_SubAssgin){
				printf("-=");
			}
			else if(token->subType==TokenSubType_LeftShiftAssgin){
				printf("<<=");
			}
			else if(token->subType==TokenSubType_RightShiftAssgin){
				printf(">>=");
			}
			else if(token->subType==TokenSubType_XOrAssgin){
				printf("|=");
			}
			else if(token->subType==TokenSubType_XAndAssgin){
				printf("&=");
			}
			else if(token->subType==TokenSubType_XErAssgin){
				printf("^=");
			}
			else if(token->subType==TokenSubType_Reverse){
				printf("~");
			}
			else if(token->subType==TokenSubType_Not){
				printf("!");
			}
			else if(token->subType==TokenSubType_Mul){
				printf("*");
			}
			else if(token->subType==TokenSubType_Div){
				printf("/");
			}
			else if(token->subType==TokenSubType_Mod){
				printf("%c",'%');
			}
			else if(token->subType==TokenSubType_Add){
				printf("+");
			}
			else if(token->subType==TokenSubType_Sub){
				printf("-");
			}
			else if(token->subType==TokenSubType_LeftShift){
				printf("<<");
			}
			else if(token->subType==TokenSubType_RightShift){
				printf(">>");
			}
			else if(token->subType==TokenSubType_GR){
				printf(">");
			}
			else if(token->subType==TokenSubType_GE){
				printf(">=");
			}
			else if(token->subType==TokenSubType_LS){
				printf("<");
			}
			else if(token->subType==TokenSubType_LE){
				printf("<=");
			}
			else if(token->subType==TokenSubType_EQ){
				printf("==");
			}
			else if(token->subType==TokenSubType_NE){
				printf("!=");
			}
			else if(token->subType==TokenSubType_XOr){
				printf("|");
			}
			else if(token->subType==TokenSubType_XAnd){
				printf("&");
			}
			else if(token->subType==TokenSubType_XEr){
				printf("^");
			}
			else if(token->subType==TokenSubType_And){
				printf("&&");
			}
			else if(token->subType==TokenSubType_Or){
				printf("||");
			}
			else if(token->subType==TokenSubType_Three){
				printf("?");
			}

		}
		else if(token->type==TokenType_Delimiter){
			printf("D  ");
			if(token->subType==TokenSubType_LeftParenth){
				printf("(");
			}
			else if(token->subType==TokenSubType_RightParenth){
				printf(")");
			}
			else if(token->subType==TokenSubType_LeftBracket){
				printf("[");
			}
			else if(token->subType==TokenSubType_RightBracket){
				printf("]");
			}
			else if(token->subType==TokenSubType_LeftBrace){
				printf("{");
			}
			else if(token->subType==TokenSubType_RightBrace){
				printf("}");
			}
			else if(token->subType==TokenSubType_Dot){
				printf(".");
			}
			else if(token->subType==TokenSubType_Comma){
				printf(",");
			}
			else if(token->subType==TokenSubType_Colon){
				printf(":");
			}
			else if(token->subType==TokenSubType_Semicolon){
				printf(";");
			}

		}

		printf("\n");
		token=token->next;
	}

	printf(">>> lexDebug end\n");
}









