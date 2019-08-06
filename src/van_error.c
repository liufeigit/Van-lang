
#include "van_lex.h"


ErrorInfor __van_errorInfor;

static void error_printError();

void error_throwError(ErrorType type,Token *token){

	if(!__van_errorInfor.type){
		__van_errorInfor.type=type;
		__van_errorInfor.row=token->row;
		__van_errorInfor.column=token->column;

		error_printError();
	}

}

void error_printError(){
	ErrorType type=0;
	unsigned int row=0;
	unsigned int column=0;

	type=__van_errorInfor.type;
	row=__van_errorInfor.row+1;
	column=__van_errorInfor.column+1;
	if(type){

		if(type==ErrorType_LexError){
			printf(">>> throw LexError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_ExprSyntaxError){
			printf(">>> throw ExprSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_AssignSyntaxError){
			printf(">>> throw AssignSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_ContinueSyntaxError){
			printf(">>> throw ContinueSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_BreakSyntaxError){
			printf(">>> throw BreakSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_ReturnSyntaxError){
			printf(">>> throw ReturnSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_UpSyntaxError){
			printf(">>> throw UpSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_GlobalSyntaxError){
			printf(">>> throw GlobalSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_FieldSyntaxError){
			printf(">>> throw FieldSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_StaticSyntaxError){
			printf(">>> throw StaticSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_ImportSyntaxError){
			printf(">>> throw ImportSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_PrintSyntaxError){
			printf(">>> throw PrintSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_IfSyntaxError){
			printf(">>> throw IfSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_WhileSyntaxError){
			printf(">>> throw WhileSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_ForSyntaxError){
			printf(">>> throw ForSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_FunctionSyntaxError){
			printf(">>> throw FunctionSyntaxError: line %d, column %d\n",row,column);
		}
		else if(type==ErrorType_ClassSyntaxError){
			printf(">>> throw ClassSyntaxError: line %d, column %d\n",row,column);
		}

	}
}






