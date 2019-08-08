#ifndef VAN_PARSENUM_H
#define VAN_PARSENUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	ASTType_Expr=1,
	ASTType_Assign,
	ASTType_If,
	ASTType_While,
	ASTType_For,
	ASTType_Function,
	ASTType_Class,

	ASTType_Continue,
	ASTType_Break,
	ASTType_Return,
	ASTType_Up,
	ASTType_Global,
	ASTType_Field,
	ASTType_Static,
	ASTType_Import,
	ASTType_Yield,

	ASTType_Print, // 测试

	ASTType_Block,

} ASTType;

typedef enum{
	ASTOp_IfTerm=1,
	ASTOp_ElseIfTerm,

	ASTOp_ForTerm,
	ASTOp_WhileTerm,

	ASTOp_Comma,
	ASTOp_Arguments,
	ASTOp_Pair,
	ASTOp_Slice,

	ASTOp_IfBlock,
	ASTOp_IfElseBlock,
	ASTOp_ElseBlock,
	ASTOp_ForBlock,
	ASTOp_WhileBlock,
	ASTOp_FunctionBlock,
	ASTOp_ClassBlock,

	ASTOp_DeclareAssign,
	ASTOp_MulAssgin,
	ASTOp_DivAssgin,
	ASTOp_ModAssgin,
	ASTOp_AddAssgin,
	ASTOp_SubAssgin,
	ASTOp_LeftShiftAssgin,
	ASTOp_RightShiftAssgin,
	ASTOp_XOrAssgin,
	ASTOp_XAndAssgin,
	ASTOp_XErAssgin,

	ASTOp_Indentify, 
	// ASTOp_Number,
	ASTOp_Int, // long long
	ASTOp_Float, // double
	ASTOp_Null,
	ASTOp_True,
	ASTOp_False,
	ASTOp_String,

	ASTOp_Self,
	ASTOp_Super,

	ASTOp_Map, // {Indentify:Expr}
	ASTOp_List, // [Expr]

	ASTOp_FunctionExpr, // def {}
	ASTOp_ClassExpr, // class {}

	ASTOp_Prior, // (Expr)

	ASTOp_Call, // Expr(Expr,...)
	ASTOp_Property, // Expr.Indentify
	ASTOp_Subscript, // Expr[Number|String]

	ASTOp_Not, // !
	ASTOp_Reverse, // ~
	ASTOp_New, // new Expr
	ASTOp_Negative, // -Expr

	ASTOp_BinaryStart=100,

	ASTOp_Mul=101, // *
	ASTOp_Div=111, // /
	ASTOp_Mod=121, // %

	ASTOp_Add=102, // +
	ASTOp_Sub=112, // -

	ASTOp_LeftShift=103, // <<
	ASTOp_RightShift=113, // >>

	ASTOp_GR=104, // >
	ASTOp_GE=114, // >=
	ASTOp_LS=124, // <
	ASTOp_LE=134, // <=

	ASTOp_EQ=105, // ==
	ASTOp_NE=115, // !=

	ASTOp_XOr=106, // |
	ASTOp_XAnd=116, // &
	ASTOp_XEr=126, // ^

	ASTOp_And=107, // &&
	ASTOp_Or=117, // ||

	ASTOp_Three=108, // ?:

	ASTOp_BinaryEnd=200,

} ASTOp;


#ifdef __cplusplus
}
#endif

#endif




