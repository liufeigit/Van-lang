// clang -g -fPIC -shared  -L. -lvan extendTest1.c -o ./External/libextendTest1.so

#include "../src/van_data.h"
#include "../src/van_environ.h"

void module_init();

int sub(int a,int b);

VanWrap *wrap_sub(VanWrap *self,VanList *argList);


// static/dynamic 
void nativeFunction_Load(VanEnviron *env){

	VanFunction *func=(VanFunction *)calloc(1, sizeof(VanFunction));

	func->name=(char *)calloc(1, strlen("sub")+1);
	strcpy(func->name,"sub");
	func->code=wrap_sub;
	func->type=FunctionType_NativeFunction;

	environ_brigeNativeFunction(env, func);

}

int sub(int a,int b){

	return a+b;
}

VanWrap *wrap_sub(VanWrap *self,VanList *argList){
	VanWrap *result=NULL;

	int v=0;

	if(list_length(argList)>=2){
		v=sub(list_getValue(argList, 0)->value.l,list_getValue(argList, 1)->value.l);
		result=wrap_wrapLongLong(v);

		printf("sub is %s\n",wrap_toString(result));
	}

	return result;
}

