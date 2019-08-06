// clang  

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "van_lex.h"

int __lex_strLen=0;
int __lex_curRow=0;
int __lex_curCol=0;

static Token *lexComment(char *str,int cur,int end);
static Token *lexIdentify(char *str,int cur,int end);
static Token *lexKeyword(char *str,int cur,int end);
static Token *lexLiteral(char *str,int cur,int end);
static Token *lexOperator(char *str,int cur,int end);
static Token *lexDelimiter(char *str,int cur,int end);

static int lexFilterWs(char *str,int cur);

static int __convertUTF8(char *str,int index,char *str1,int index1);
static char *__convertString(char *str);
static int __isHex(char c);
static int __lexUniHex(char *str,int cur); 
static int __lexEscape(char *str,int cur); 

static int __lexString(char *str,char **data);
static int __lexInt(char *str,long long *data);
static int __lexFloat(char *str,double *data);

static int __isComment(char *str,int cur);
static int __isIdentify(char *str,int cur);
static int __isKeyword(char *str,int cur);
static int __isLiteral(char *str,int cur);
static int __isOperator(char *str,int cur);
static int __isDelimiter(char *str,int cur);

static int __isString(char *str,int cur);
static int __isNumber(char *str,int cur);
static int __isInt(char *str);
static int __isFloat(char *str);

static int __isTrue(char *str,int cur);
static int __isFalse(char *str,int cur);
static int __isNull(char *str,int cur);

static int __isDigit(char *str,int cur);

static int __isDotDigit(char *str,int cur);

int lexStr(char *str,Token **token){
	int offset=0;
	int end=0;
	Token *head=NULL;
	Token *curToken=NULL;
	
	__lex_strLen=strlen(str);
	offset=lexFilterWs(str, offset);
	while(offset<__lex_strLen){
		Token *_token=NULL;

		end=0;
		if((end=__isComment(str,offset))){
			_token=lexComment(str,offset,end);
		}
		else if((end=__isLiteral(str,offset))){
			_token=lexLiteral(str,offset,end);
		}
		else if((end=__isKeyword(str,offset))){
			_token=lexKeyword(str,offset,end);
		}
		else if((end=__isIdentify(str,offset))){
			_token=lexIdentify(str,offset,end);
		}
		else if((end=__isOperator(str,offset))){
			_token=lexOperator(str,offset,end);
		}
		else if((end=__isDelimiter(str,offset))){
			_token=lexDelimiter(str,offset,end);
		}

		if(end&&_token){
			offset=end;
			offset=lexFilterWs(str, offset);
			if(_token->type!=TokenType_Comment){
				if(!head){
					head=_token;
					curToken=_token;
				}
				else{
					curToken->next=_token;
					_token->pre=curToken;
					curToken=_token;
				}
			}
		}
		else{ // error处理
			printf(">>> throw LexError: line %d, column %d\n",__lex_curRow+1,__lex_curCol+1);
			// exit(0);

			offset=0;
			break;
		}
		
	}

	if(offset){
		Token *tail=NULL;

		tail=(Token *)calloc(1, sizeof(Token ));
		tail->type=TokenType_Delimiter;
		tail->subType=TokenSubType_Semicolon;

		curToken->next=tail;
		tail->pre=curToken;

		*token=head;
	}

	return offset;
}

void lexDebug(Token *token){


}

static int lexFilterWs(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return cur;
	}

	offset=cur;
	while(cur<__lex_strLen){
		if(str[cur]=='\x0a'||str[cur]=='\x09'||
			str[cur]=='\x0d'||str[cur]=='\x20'){ // \n \t \r space

			if(str[cur]=='\x0a'){
				__lex_curCol=0;
				__lex_curRow++;
			}
			else if(str[cur]=='\x09'||str[cur]=='\x20'){ // ??? \r
				__lex_curCol++;
			}

			cur++;
			offset=cur;
		}
		else{
			break;
		}
	}

	return offset;
}

/***
	0000~007f --> 0xxxxxxx
	0080~07ff --> 110xxxxx 10xxxxxx
	0800~ffff --> 1110xxxx 10xxxxxx 10xxxxxx
	.....
	>=2 取位存码 第一字节补0按8位掩码 其它字节移位后6位掩码取位
****/
static int __convertUTF8(char *str,int index,char *str1,int index1){
	int len=0;
	int value=0;
	int fact=1;
	char a='\0';

	for(int i=index+3;i>=index;i--){
		if(str[i]>='0'&&str[i]<='9'){
			a=str[i]-'0';
		}
		else if(str[i]>='a'&&str[i]<='f'){
			a=10+str[i]-'a';
		}
		else if(str[i]>='A'&&str[i]<='F'){
			a=10+str[i]-'A';
		}

		if(i==index+3){
			value=value+a;
		}
		else{
			fact*=16;
			value=value+a*fact;
		}
	}

	if(value>=0x0000&&value<=0x007f){
		str1[index1]=value;
		len++;
	}
	else if(value>=0x0080&&value<=0x07ff){
		str1[index1]=0xc0|((value>>6)&0xff); // 110
		str1[index1+1]=0x80|(value&0x3f); // 10
		len+=2;
	}
	else if(value>=0x0800&&value<=0xffff){
		str1[index1]=0xe0|((value>>12)&0xff); // 1110
		str1[index1+1]=0x80|((value>>6)&0x3f); // 10
		str1[index1+2]=0x80|(value&0x3f); // 10
		len+=3;
	}

	return len;
}

// str=> " "
static char *__convertString(char *str){
	char *data=NULL;

	int length=0;
	int _length=0;

	length=strlen(str);
	data=(char *)malloc((length+1)*sizeof(char));
	for(int i=1,j=0;i<length-1;i++,j++){
		if(str[i]=='\\'){
			if(str[i+1]=='\"'){
				data[j]='\"';
				i++;
			}
			else if(str[i+1]=='\\'){
				data[j]='\\';
				i++;
			}
			else if(str[i+1]=='/'){
				data[j]='/';
				i++;
			}
			else if(str[i+1]=='b'){
				data[j]='\b';
				i++;
			}
			else if(str[i+1]=='f'){
				data[j]='\f';
				i++;
			}
			else if(str[i+1]=='n'){
				data[j]='\n';
				i++;
			}
			else if(str[i+1]=='r'){
				data[j]='\r';
				i++;
			}
			else if(str[i+1]=='t'){
				data[j]='\t';
				i++;
			}
			else if(str[i+1]=='u'){ 
				int _len=0;

				_len=__convertUTF8(str,i+2,data,j);
				j+=(_len-1);
				i+=5;
			}
			else{ // error
				data[j]=str[i];
			}
		}
		else{
			data[j]=str[i];
		}

		_length=j;
	}
	data[_length+1]='\0';

	return data;
}

static int __isHex(char c){
	int status=0;

	if((c>='0'&&c<='9')||
		(c>='A'&&c<='F')||
		(c>='a'&&c<='f')){
		status=1;
	}

	return status;
}

static int __lexUniHex(char *str,int cur){
	int offset=0;
	int num=0;
	int _length=0;

	_length=strlen(str);

	cur++;
	if(cur>=_length){
		return 0;
	}
	while(__isHex(str[cur])){
		cur++;
		num++;
		if(num==4){
			break;
		}

		if(cur>=_length){
			return 0;
		}
	}

	if(num==4){
		offset=cur;
	}

	return offset;
}

static int __lexEscape(char *str,int cur){
	int offset=0;

	int _length=0;

	_length=strlen(str);

	cur++;
	if(cur>=_length){
		return 0;
	}
	if(str[cur]=='\"'||
		str[cur]=='\\'||
		str[cur]=='/'||
		str[cur]=='b'||
		str[cur]=='f'||
		str[cur]=='n'||
		str[cur]=='r'||
		str[cur]=='t'){
		cur++;
	}
	else if(str[cur]=='u'){
		cur=__lexUniHex(str,cur);
	}
	else{
		cur=0;
	}

	offset=cur;
	return offset;
}

// str==>" "
static int __lexString(char *str,char **data){
	int offset=0;

	int cur=0;
	int _length=0;

	_length=strlen(str);

	cur++;
	while(str[cur]!='\"'){
		if(str[cur]=='\\'){
			cur=__lexEscape(str,cur);
			if(cur==0){
				break;
			}
		}
		else{
			cur++;
		}

		if(cur>=_length){
			return 0;
		}
	}

	if(cur&&str[cur]=='\"'){
		offset=cur+1;
	}

	if(offset){
		*data=__convertString(str);
	}

	return offset;
}

static int __lexInt(char *str,long long *data){
	int status=1;

	long long v=0;
	char *str1=NULL;

	v=strtol(str,&str1,10);
	*data=v;

	if(v==0&&str[0]!='0'){
		status=0;
	}

	return status;
}

static int __lexFloat(char *str,double *data){
	int status=1;

	double v=0;
	char *str1=NULL;

	v=strtod(str,&str1);
	*data=v;

	if(v==0){
		status=0;
	}

	return status;
}

static Token *lexDelimiter(char *str,int cur,int end){
	Token *token=NULL;
	int len=0;

	len=end-cur;
	token=(Token *)calloc(1, sizeof(Token ));
	token->type=TokenType_Delimiter;
	if(str[cur]=='.'){
		token->subType=TokenSubType_Dot;
	}
	else if(str[cur]==','){
		token->subType=TokenSubType_Comma;
	}
	else if(str[cur]==':'){
		token->subType=TokenSubType_Colon;
	}
	else if(str[cur]==';'){
		token->subType=TokenSubType_Semicolon;
	}
	else if(str[cur]=='('){
		token->subType=TokenSubType_LeftParenth;
	}
	else if(str[cur]==')'){
		token->subType=TokenSubType_RightParenth;
	}
	else if(str[cur]=='['){
		token->subType=TokenSubType_LeftBracket;
	}
	else if(str[cur]==']'){
		token->subType=TokenSubType_RightBracket;
	}
	else if(str[cur]=='{'){
		token->subType=TokenSubType_LeftBrace;
	}
	else if(str[cur]=='}'){
		token->subType=TokenSubType_RightBrace;
	}
	// else if(str[cur]=='='){
	// 	token->subType=TokenSubType_Assign;
	// }

	token->row=__lex_curRow;
	token->column=__lex_curCol;
	__lex_curCol+=len;

	return token;
}

static Token *lexOperator(char *str,int cur,int end){
	Token *token=NULL;
	int len=0;

	len=end-cur;
	token=(Token *)calloc(1, sizeof(Token ));
	token->type=TokenType_Operator;

	if(len==1){
		if(str[cur]=='~'){
			token->subType=TokenSubType_Reverse;
		}
		else if(str[cur]=='!'){
			token->subType=TokenSubType_Not;
		}
		else if(str[cur]=='*'){
			token->subType=TokenSubType_Mul;
		}
		else if(str[cur]=='/'){
			token->subType=TokenSubType_Div;
		}
		else if(str[cur]=='%'){
			token->subType=TokenSubType_Mod;
		}
		else if(str[cur]=='+'){
			token->subType=TokenSubType_Add;
		}
		else if(str[cur]=='-'){
			token->subType=TokenSubType_Sub;
		}
		else if(str[cur]=='>'){
			token->subType=TokenSubType_GR;
		}
		else if(str[cur]=='<'){
			token->subType=TokenSubType_LS;
		}
		else if(str[cur]=='|'){
			token->subType=TokenSubType_XOr;
		}
		else if(str[cur]=='&'){
			token->subType=TokenSubType_XAnd;
		}
		else if(str[cur]=='^'){
			token->subType=TokenSubType_XEr;
		}
		else if(str[cur]=='='){
			token->subType=TokenSubType_Assign;
		}
	}
	else if(len==2){
		if(strncmp(str+cur,"<<",len)==0){
			token->subType=TokenSubType_LeftShift;
		}
		else if(strncmp(str+cur,">>",len)==0){
			token->subType=TokenSubType_RightShift;
		}
		else if(strncmp(str+cur,">=",len)==0){
			token->subType=TokenSubType_GE;
		}
		else if(strncmp(str+cur,"<=",len)==0){
			token->subType=TokenSubType_LE;
		}
		else if(strncmp(str+cur,"==",len)==0){
			token->subType=TokenSubType_EQ;
		}
		else if(strncmp(str+cur,"!=",len)==0){
			token->subType=TokenSubType_NE;
		}
		else if(strncmp(str+cur,"&&",len)==0){
			token->subType=TokenSubType_And;
		}
		else if(strncmp(str+cur,"||",len)==0){
			token->subType=TokenSubType_Or;
		}
		else if(strncmp(str+cur,"*=",len)==0){
			token->subType=TokenSubType_MulAssgin;
		}
		else if(strncmp(str+cur,"/=",len)==0){
			token->subType=TokenSubType_DivAssgin;
		}
		else if(strncmp(str+cur,"%=",len)==0){
			token->subType=TokenSubType_ModAssgin;
		}
		else if(strncmp(str+cur,"+=",len)==0){
			token->subType=TokenSubType_AddAssgin;
		}
		else if(strncmp(str+cur,"-=",len)==0){
			token->subType=TokenSubType_SubAssgin;
		}
		else if(strncmp(str+cur,"|=",len)==0){
			token->subType=TokenSubType_XOrAssgin;
		}
		else if(strncmp(str+cur,"&=",len)==0){
			token->subType=TokenSubType_XAndAssgin;
		}
		else if(strncmp(str+cur,"^=",len)==0){
			token->subType=TokenSubType_XErAssgin;
		}
	}
	else if(len==3){
		if(strncmp(str+cur,"<<=",len)==0){
			token->subType=TokenSubType_LeftShiftAssgin;
		}
		else if(strncmp(str+cur,">>=",len)==0){
			token->subType=TokenSubType_RightShiftAssgin;
		}
	}

	token->row=__lex_curRow;
	token->column=__lex_curCol;
	__lex_curCol+=len;

	return token;
}

static Token *lexKeyword(char *str,int cur,int end){
	Token *token=NULL;
	int len=0;

	len=end-cur;
	token=(Token *)calloc(1, sizeof(Token ));
	token->type=TokenType_Keyword;

	if(len==2){
		if(strncmp(str+cur, "if", len)==0){
			token->subType=TokenSubType_If;
		}
		else if(strncmp(str+cur, "in", len)==0){
			token->subType=TokenSubType_In;
		}
		else if(strncmp(str+cur, "is", len)==0){
			token->subType=TokenSubType_Is;
		}
		else if(strncmp(str+cur, "up", len)==0){
			token->subType=TokenSubType_Up;
		}
	}
	else if(len==3){
		if(strncmp(str+cur, "def", len)==0){
			token->subType=TokenSubType_Def;
		}
		else if(strncmp(str+cur, "new", len)==0){
			token->subType=TokenSubType_New;
		}
		else if(strncmp(str+cur, "for", len)==0){
			token->subType=TokenSubType_For;
		}
		else if(strncmp(str+cur, "var", len)==0){
			token->subType=TokenSubType_Var;
		}
		else if(strncmp(str+cur, "out", len)==0){
			token->subType=TokenSubType_Out;
		}
	}
	else if(len==4){
		if(strncmp(str+cur, "self", len)==0){
			token->subType=TokenSubType_Self;
		}
		else if(strncmp(str+cur, "base", len)==0){
			token->subType=TokenSubType_Base;
		}
		else if(strncmp(str+cur, "else", len)==0){
			token->subType=TokenSubType_Else;
		}
	}
	else if(len==5){
		if(strncmp(str+cur, "class", len)==0){
			token->subType=TokenSubType_Class;
		}
		else if(strncmp(str+cur, "super", len)==0){
			token->subType=TokenSubType_Super;
		}
		else if(strncmp(str+cur, "while", len)==0){
			token->subType=TokenSubType_While;
		}
		else if(strncmp(str+cur, "break", len)==0){
			token->subType=TokenSubType_Break;
		}
		else if(strncmp(str+cur, "field", len)==0){
			token->subType=TokenSubType_Field;
		}
		else if(strncmp(str+cur, "yield", len)==0){
			token->subType=TokenSubType_Yield;
		}
		else if(strncmp(str+cur, "print", len)==0){
			token->subType=TokenSubType_Print;
		}
	}
	else if(len==6){
		if(strncmp(str+cur, "return", len)==0){
			token->subType=TokenSubType_Return;
		}
		else if(strncmp(str+cur, "static", len)==0){
			token->subType=TokenSubType_Static;
		}
		else if(strncmp(str+cur, "global", len)==0){
			token->subType=TokenSubType_Global;
		}
		else if(strncmp(str+cur, "import", len)==0){
			token->subType=TokenSubType_Import;
		}
	}
	else if(len==7){
		if(strncmp(str+cur, "declare", len)==0){
			token->subType=TokenSubType_Declare;
		}
		else if(strncmp(str+cur, "include", len)==0){
			token->subType=TokenSubType_Include;
		}
	}
	else if(len==8){
		if(strncmp(str+cur, "continue", len)==0){
			token->subType=TokenSubType_Continue;
		}
		else if(strncmp(str+cur, "instance", len)==0){
			token->subType=TokenSubType_Instance;
		}
	}

	token->row=__lex_curRow;
	token->column=__lex_curCol;
	__lex_curCol+=len;

	return token;
}

static Token *lexIdentify(char *str,int cur,int end){
	Token *token=NULL;
	int len=0;

	len=end-cur;
	token=(Token *)calloc(1, sizeof(Token ));
	token->type=TokenType_Identify;

	token->data=(char *)calloc(len+1,sizeof(char ));
	strncpy(token->data,str+cur,len);

	token->row=__lex_curRow;
	token->column=__lex_curCol;
	__lex_curCol+=len;

	return token;
}

static Token *lexLiteral(char *str,int cur,int end){
	Token *token=NULL;
	int len=0;

	len=end-cur;
	token=(Token *)calloc(1, sizeof(Token ));
	token->type=TokenType_Literal;

	if(__isTrue(str, cur)){
		token->subType=TokenSubType_True;
	}
	else if(__isFalse(str, cur)){
		token->subType=TokenSubType_False;
	}
	else if(__isNull(str, cur)){
		token->subType=TokenSubType_Null;
	}
	else if(__isString(str, cur)){
		int _flag=0;
		char *_str=NULL;

		token->subType=TokenSubType_String;

		_str=(char *)calloc(len+1,sizeof(char ));
		strncpy(_str,str+cur,len);
		_flag=__lexString(_str, &token->data);
		
		free(_str);

		if(!token->data){
			free(token);
			return NULL;
		}
	}
	else if(__isNumber(str, cur)){
		int _flag=0;
		char *_str=NULL;

		_str=(char *)calloc(len+1,sizeof(char ));
		strncpy(_str,str+cur,len);

		if(__isInt(_str)){
			long long v=0;

			token->subType=TokenSubType_Int;
			_flag=__lexInt(_str, &v);
			token->value.l=v;
		}
		else if(__isFloat(_str)){
			double v=0;

			token->subType=TokenSubType_Float;
			_flag=__lexFloat(_str, &v);
			token->value.d=v;
		}

		free(_str);

		if(!_flag){
			free(token);
			return NULL;
		}
	}

	token->row=__lex_curRow;
	token->column=__lex_curCol;
	__lex_curCol+=len;

	return token;
}

static Token *lexComment(char *str,int cur,int end){
	Token *token=NULL;

	token=(Token *)calloc(1, sizeof(Token ));
	token->type=TokenType_Comment;

	token->row=__lex_curRow;
	token->column=__lex_curCol;
	__lex_curCol+=(end-cur);

	return token;
}

static int __isComment(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return 0;
	}

	if(str[cur]=='#'){
		cur++;
		while(cur<__lex_strLen){
			if(str[cur]=='\n'){
				break;
			}
			cur++;
		}
		offset=cur;
	}

	return offset;
}

static int __isIdentify(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return 0;
	}

	if(str[cur]=='_'||
		(str[cur]>='a'&&str[cur]<='z')||
		(str[cur]>='A'&&str[cur]<='Z')){
		cur++;
		for(int i=cur;i<__lex_strLen;i++){
			if(!(str[i]=='_'||
				(str[i]>='a'&&str[i]<='z')||
				(str[i]>='A'&&str[i]<='Z')||
				(str[i]>='0'&&str[i]<='9'))){
				cur=i;
				break;
			}
		}

		offset=cur;
	}

	return offset;
}

static int __isDelimiter(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return 0;
	}

	if(str[cur]=='.'||str[cur]==','||
		str[cur]==':'||str[cur]==';'||
		str[cur]=='('||str[cur]==')'||
		str[cur]=='['||str[cur]==']'||
		str[cur]=='{'||str[cur]=='}'||
		str[cur]=='='){
		offset=cur+1;
	}

	return offset;
}

static int __isKeyword(char *str,int cur){
	int offset=0;

	int flag1=0;
	int len=0;

	flag1=__isIdentify(str,cur);
	len=flag1-cur;
	if(len==2){
		if(strncmp(str+cur, "if", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "in", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "is", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "up", len)==0){
			offset=flag1;
		}
	}
	else if(len==3){
		if(strncmp(str+cur, "def", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "new", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "for", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "var", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "out", len)==0){
			offset=flag1;
		}
	}
	else if(len==4){
		if(strncmp(str+cur, "self", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "base", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "else", len)==0){
			offset=flag1;
		}
	}
	else if(len==5){
		if(strncmp(str+cur, "class", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "super", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "while", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "break", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "field", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "yield", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "print", len)==0){
			offset=flag1;
		}
	}
	else if(len==6){
		if(strncmp(str+cur, "return", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "static", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "global", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "import", len)==0){
			offset=flag1;
		}
	}
	else if(len==7){
		if(strncmp(str+cur, "declare", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "include", len)==0){
			offset=flag1;
		}
	}
	else if(len==8){
		if(strncmp(str+cur, "continue", len)==0){
			offset=flag1;
		}
		else if(strncmp(str+cur, "instance", len)==0){
			offset=flag1;
		}
	}

	return offset;
}

static int __isOperator(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return 0;
	}

	if(str[cur]=='~'||str[cur]=='*'||
		str[cur]=='/'||str[cur]=='%'||
		str[cur]=='^'||str[cur]=='+'||
		str[cur]=='-'){

		offset=cur+1;
		if(cur+2<=__lex_strLen){
			if(strncmp(str+cur,"*=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"/=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"%>",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"^=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"+=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"-=",2)==0){
				offset++;
			}
		}
	}
	else if(str[cur]=='!'||str[cur]=='>'||
		str[cur]=='<'||str[cur]=='&'||
		str[cur]=='|'){

		offset=cur+1;
		if(cur+2<=__lex_strLen){
			if(strncmp(str+cur,"!=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,">=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,">>",2)==0){
				offset++;
				if(cur+3<=__lex_strLen){
					if(strncmp(str+cur,">>=",3)==0){
						offset++;
					}
				}
			}
			else if(strncmp(str+cur,"<=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"<<",2)==0){
				offset++;
				if(cur+3<=__lex_strLen){
					if(strncmp(str+cur,"<<=",3)==0){
						offset++;
					}
				}
			}
			else if(strncmp(str+cur,"&&",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"||",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"&=",2)==0){
				offset++;
			}
			else if(strncmp(str+cur,"|=",2)==0){
				offset++;
			}
		}
	}
	else if(str[cur]=='='){
		offset=cur+1;
		if(cur+2<=__lex_strLen){
			if(strncmp(str+cur,"==",2)==0){
				offset=cur+2;
			}
		}
	}

	return offset;
}

static int __isLiteral(char *str,int cur){
	int offset=0;
	int flag1=0;

	flag1=__isNumber(str, cur);
	if(flag1){
		return flag1;
	}

	flag1=__isString(str, cur);
	if(flag1){
		return flag1;
	}

	flag1=__isTrue(str, cur);
	if(flag1){
		return flag1;
	}

	flag1=__isFalse(str, cur);
	if(flag1){
		return flag1;
	}

	flag1=__isNull(str, cur);
	if(flag1){
		return flag1;
	}

	return offset;
}

static int __isDigit(char *str,int cur){
	int offset=0;

	cur++;
	if(cur>=__lex_strLen){
		return cur;
	}
	while(str[cur]>='0'&&str[cur]<='9'){
		cur++;
		if(cur>=__lex_strLen){
			break;
		}
	}

	offset=cur;
	return offset;
}

static int __isDotDigit(char *str,int cur){
	int offset=0;

	cur++;
	if(cur>=__lex_strLen){
		return 0;
	}
	if(str[cur]>='0'&&str[cur]<='9'){
		offset=__isDigit(str,cur);
	}

	return offset;
}

// ???
static int __isNumber(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return 0;
	}

	if(str[cur]=='0'){ // 0
		cur++;
	}
	else if(str[cur]>='1'&&str[cur]<='9'){ // [1-9]
		cur=__isDigit(str,cur);
	}
	else{
		return offset;
	}

	if(cur>=__lex_strLen){ // right
		return cur;
	}

	if(str[cur]=='.'){
		cur=__isDotDigit(str,cur);
		if(cur==0){
			return offset;
		}
	}

	if(cur>=__lex_strLen){ // right
		return cur;
	}

	offset=cur;
	return offset;
}

static int __isInt(char *str){
	int flag=1;

	int i=0;

	while(str[i]!='\0'){
		if(str[i]=='.'){
			flag=0;
			break;
		}

		i++;
	}

	return flag;
}

static int __isFloat(char *str){
	int flag=0;

	int i=0;

	while(str[i]!='\0'){
		if(str[i]=='.'){
			flag=1;
			break;
		}

		i++;
	}

	return flag;
}

static int __isString(char *str,int cur){
	int offset=0;

	if(cur>=__lex_strLen){
		return 0;
	}

	if(str[cur]=='\"'){
		cur++;
		for(int i=cur;i<__lex_strLen;i++){
			if(str[i]=='\"'&&str[i-1]!='\\'){
				offset=i+1;
				break;
			}
		}
	}

	return offset;
}

static int __isTrue(char *str,int cur){
	int offset=0;
	int flag1=0;

	flag1=__isIdentify(str,cur);
	if(flag1-cur==4){
		if(strncmp(str+cur, "true", 4)==0){
			offset=flag1;
		}
	}

	return offset;
}

static int __isFalse(char *str,int cur){
	int offset=0;
	int flag1=0;

	flag1=__isIdentify(str,cur);
	if(flag1-cur==4){
		if(strncmp(str+cur, "false", 5)==0){
			offset=flag1;
		}
	}

	return offset;
}

static int __isNull(char *str,int cur){
	int offset=0;
	int flag1=0;

	flag1=__isIdentify(str,cur);
	if(flag1-cur==4){
		if(strncmp(str+cur, "null", 4)==0){
			offset=flag1;
		}
	}

	return offset;
}













