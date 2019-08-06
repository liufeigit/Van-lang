#ifndef VAN_ENUM_H
#define VAN_ENUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	TokenType_Comment=1,
	TokenType_Identify,
	TokenType_Keyword,
	TokenType_Literal,
	TokenType_Operator,
	TokenType_Delimiter

} TokenType;

typedef enum {
	// keyword
	TokenSubType_If=1,
	TokenSubType_In,
	TokenSubType_Is,
	TokenSubType_Up,

	TokenSubType_Def,
	TokenSubType_New,
	TokenSubType_For,
	TokenSubType_Var,
	TokenSubType_Out,
	TokenSubType_End,
	TokenSubType_Ref,

	TokenSubType_Self,
	TokenSubType_Base,
	TokenSubType_Else,

	TokenSubType_Class,
	TokenSubType_Super,
	TokenSubType_While,
	TokenSubType_Break,
	TokenSubType_Field,
	TokenSubType_Yield,
	TokenSubType_Print, // 测试

	TokenSubType_Return,
	TokenSubType_Static,
	TokenSubType_Global,
	TokenSubType_Import,
	TokenSubType_Export,
	TokenSubType_Extern,
	
	TokenSubType_Declare,
	TokenSubType_Include,
	TokenSubType_Nataive,

	TokenSubType_Continue,
	TokenSubType_Instance,

	// literal
	TokenSubType_String,
	// TokenSubType_Number,
	TokenSubType_Int,
	TokenSubType_Float,
	TokenSubType_True,
	TokenSubType_False,
	TokenSubType_Null,

	// delimiter .,:;()[]{}  ()[]{}<>''"".,;:`~!@#$^& =?_|
	TokenSubType_LeftParenth, // '('
	TokenSubType_RightParenth,
	TokenSubType_LeftBracket, // '['
	TokenSubType_RightBracket,
	TokenSubType_LeftBrace, // '{'
	TokenSubType_RightBrace,
	TokenSubType_Dot, // '.'
	TokenSubType_Comma, // ','
	TokenSubType_Colon, // ':'
	TokenSubType_Semicolon, // ';'

	TokenSubType_AssignStart,
	TokenSubType_Assign, // '='
	TokenSubType_MulAssgin, // *=
	TokenSubType_DivAssgin, // /=
	TokenSubType_ModAssgin, // %=
	TokenSubType_AddAssgin, // +=
	TokenSubType_SubAssgin, // -=
	TokenSubType_LeftShiftAssgin, // <<=
	TokenSubType_RightShiftAssgin, // >>=
	TokenSubType_XOrAssgin, // |=
	TokenSubType_XAndAssgin, // &=
	TokenSubType_XErAssgin, // ^=
	TokenSubType_AssignEnd,

	TokenSubType_LeftAngle, // '<'
	TokenSubType_RightAngle,
	TokenSubType_At, // '@'
	TokenSubType_Single, // ' ' '
	TokenSubType_Double, // ' " '
	TokenSubType_Sharp, // '#'
	TokenSubType_Slash, // '/'
	TokenSubType_Escape, // '\'
	TokenSubType_Bar, // '|'
	TokenSubType_Underline, // '_'
	TokenSubType_Grave, // '`'
	TokenSubType_Question, // '?'
	TokenSubType_Doller, // '$'
	TokenSubType_Ampersand, // '&'
	TokenSubType_Caret, // '^'
	TokenSubType_Tilde, // '~'

	// operator +-*/% &&||! |&^~<<>> >>=<<
	TokenSubType_Reverse, // ~
	TokenSubType_Not, // !

	TokenSubType_BinaryStart=100,

	TokenSubType_Mul=101, // *
	TokenSubType_Div=111, // /
	TokenSubType_Mod=121, // %

	TokenSubType_Add=102, // +
	TokenSubType_Sub=112, // -

	TokenSubType_LeftShift=103, // <<
	TokenSubType_RightShift=113, // >>

	TokenSubType_GR=104, // >
	TokenSubType_GE=114, // >=
	TokenSubType_LS=124, // <
	TokenSubType_LE=134, // <=

	TokenSubType_EQ=105, // ==
	TokenSubType_NE=115, // !=

	TokenSubType_XOr=106, // |
	TokenSubType_XAnd=116, // &
	TokenSubType_XEr=126, // ^

	TokenSubType_And=107, // &&
	TokenSubType_Or=117, // ||

	TokenSubType_Three=108, // ?

	TokenSubType_BinaryEnd=200,

} TokenSubType;

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


typedef enum {
	DataType_Null=1,
	DataType_True,
	DataType_False,

	DataType_Int,
	DataType_Float,

	DataType_String,
	DataType_List,
	DataType_Map,

	DataType_Function,
	DataType_Class,
	DataType_Instance,

	DataType_RefData,

	DataType_NativeClass,
	DataType_NativeFunction,
	DataType_NativeData,
	DataType_VoidPointer,
	DataType_OpaquePointer,

} DataType;

// van_error 相关
typedef enum {
	ErrorType_LexError=1, // 词法错误

	ErrorType_ExprSyntaxError, // 语法错误
	ErrorType_AssignSyntaxError,
	ErrorType_ContinueSyntaxError,
	ErrorType_BreakSyntaxError,
	ErrorType_ReturnSyntaxError,
	ErrorType_UpSyntaxError,
	ErrorType_GlobalSyntaxError,
	ErrorType_FieldSyntaxError,
	ErrorType_YieldSyntaxError,
	ErrorType_StaticSyntaxError,
	ErrorType_ImportSyntaxError,
	ErrorType_PrintSyntaxError,

	ErrorType_IfSyntaxError,
	ErrorType_WhileSyntaxError,
	ErrorType_ForSyntaxError,
	ErrorType_FunctionSyntaxError,
	ErrorType_ClassSyntaxError,

	ErrorType_SubscriptError, // 语义错误
	ErrorType_PropertyError,
	ErrorType_CallError,
	ErrorType_NewError,
	ErrorType_OperateError,

} ErrorType;


#ifdef __cplusplus
}
#endif

#endif




