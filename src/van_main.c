// clang -g van_lex.c van_lexUtil.c van_parse.c van_type.c van_parseUtil.c van_data.c van_dataUtil.c van_environ.c van_environBridge.c van_error.c van_main.c -o van

// clang -g -fPIC -shared van_lex.c van_lexUtil.c van_parse.c van_type.c van_parseUtil.c van_data.c van_dataUtil.c van_environ.c van_environBridge.c van_error.c -o libvan.so

// clang -g van_main.c -L. -lvan -o van

#include "van_lex.h"
#include "van_parse.h"
#include "van_data.h"
#include "van_environ.h"

void execStr(char *data);

int main(int argc,char **argv){
	char *fileName=NULL;
	FILE *file=NULL;
	int len=0;

	char *data=NULL;
	char str[128];

	if(argc<2){
		return 0;
	}

	fileName=argv[1];
	file=fopen(fileName, "r");
	data=(char *)calloc(100*1024, sizeof(char ));
	while(!feof(file)) {
        if(fgets(str,128,file)!=NULL){
        	strncpy(data+len, str, strlen(str));
    		len+=strlen(str);
        }
    }

    execStr(data);

	return 0;
}


void execStr(char *data){
	VanEnviron *env=NULL;

	int length=0;
	Token *token=NULL;
	Token *token1=NULL;
	ASTNode *node=NULL;

	length=strlen(data);
	lexStr(data, &token,&token1);
	env=environ_create();
	if(token){

		// tokenDebug(token);

		parseToken(token,&node);

		// debugASTNode(node);

		environ_eval(env,node);
	}

}



