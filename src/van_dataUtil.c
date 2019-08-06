// clang 

#include "van_data.h"


VanWrap *wrap_binaryCal(VanWrap *leftWrap,VanWrap *rightWrap,ASTOp op){
	VanWrap *wrap=NULL;

	if(leftWrap&&rightWrap){
		if((leftWrap->type==DataType_Int||leftWrap->type==DataType_Float)&&
			(rightWrap->type==DataType_Int||rightWrap->type==DataType_Float)){

			if(leftWrap->type==DataType_Int&&
				rightWrap->type==DataType_Int){

				if(op==ASTOp_LeftShift){ // <<
					wrap=wrap_wrapLongLong(leftWrap->value.l<<rightWrap->value.l);
				}
				else if(op==ASTOp_RightShift){ // >>
					wrap=wrap_wrapLongLong(leftWrap->value.l>>rightWrap->value.l);
				}
				else if(op==ASTOp_XOr){ // |
					wrap=wrap_wrapLongLong(leftWrap->value.l|rightWrap->value.l);
				}
				else if(op==ASTOp_XAnd){// &
					wrap=wrap_wrapLongLong(leftWrap->value.l&rightWrap->value.l);
				}
				else if(op==ASTOp_XEr){ // ^
					wrap=wrap_wrapLongLong(leftWrap->value.l^rightWrap->value.l);
				}
				else if(op==ASTOp_Mod){// %
					wrap=wrap_wrapLongLong(leftWrap->value.l%rightWrap->value.l); 
				}

			}

			if(op==ASTOp_Mul){ // *
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.l*rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapDouble(leftWrap->value.d*rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.d*rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l*rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_Div){ // /
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.l/rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapDouble(leftWrap->value.d/rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.d/rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l/rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_Add){ // +
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.l+rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapDouble(leftWrap->value.d+rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.d+rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l+rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_Sub){ // -
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.l-rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapDouble(leftWrap->value.d-rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapDouble(leftWrap->value.d-rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l-rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_GR){ // >
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l>rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d>rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d>rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l>rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_GE){ // >=
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l>=rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d>=rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d>=rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l>=rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_LS){ // <
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l<rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d<rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d<rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l<rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_LE){ // <=
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l<=rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d<=rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d<=rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l<=rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_EQ){ // ==
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l==rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d==rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d==rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l==rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_NE){ // !=
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l!=rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d!=rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d!=rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l!=rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_And){ // &&
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l&&rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d&&rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d&&rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l&&rightWrap->value.l); 
				}
			}
			else if(op==ASTOp_Or){ // ||
				if(leftWrap->type==DataType_Int&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.l||rightWrap->value.d); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Int){
					wrap=wrap_wrapLongLong(leftWrap->value.d||rightWrap->value.l); 
				}
				else if(leftWrap->type==DataType_Float&&
					rightWrap->type==DataType_Float){
					wrap=wrap_wrapLongLong(leftWrap->value.d||rightWrap->value.d); 
				}
				else{ // int
					wrap=wrap_wrapLongLong(leftWrap->value.l||rightWrap->value.l); 
				}
			}
			
		}
	}

	return wrap;
}


