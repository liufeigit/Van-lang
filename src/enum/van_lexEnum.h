#ifndef VAN_LEXENUM_H
#define VAN_LEXENUM_H

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


#ifdef __cplusplus
}
#endif

#endif




