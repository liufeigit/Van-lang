#ifndef VAN_DATA_H
#define VAN_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "van_enum.h"

typedef enum {
	FieldDesc_IsNull=0, // read

	FieldDesc_IsRead=1, // hidden
	FieldDesc_IsWrite=2, // overwrite
	
	FieldDesc_IsGet=4, // readProperty
	FieldDesc_IsSet=8, // writeProperty

	FieldDesc_IsCall=16, // ins/class call
	FieldDesc_IsNew=32, // static class
	FieldDesc_IsBind=64, // static function

} FieldDesc;

typedef enum {
	FunctionType_Normal=0,
	FunctionType_NativeFunction=1,

} FunctionType;

typedef enum {
	ClassType_Normal=0,
	ClassType_NativeClass=1,

} ClassType;

struct VanClass;

typedef struct VanWrap{
	DataType type;
	union {
		signed char c1;
		unsigned char c2;

		short s1;
		unsigned short s2;

		int i1;
		unsigned int i2;

		float f; 

		long long l;
		double d;

		int isSuper;
		FieldDesc fieldDesc;
	} value;

	FieldDesc _fieldDesc;
	int _referNum;

	void *data;

} VanWrap;

typedef struct Node{
	VanWrap *data;
	void *userData;

	struct Node *next;
	struct Node *pre;

} Node;

typedef struct VanString{
	struct VanClass *cls;

	int length;
	int capacity;

	char *data;
} VanString;

typedef struct VanList{
	struct VanClass *cls;

	int length;
	int capacity;
	
	Node *head;
	Node *tail;

} VanList;

typedef struct VanMap{
	struct VanClass *cls;

	int length;
	int capacity;

	// char **keyArr;
	Node *head;
	Node *tail;

} VanMap;

typedef VanWrap *(*WrapMethod)(VanWrap *self,VanList *list);

typedef struct VanFunction{
	struct VanClass *cls;

	FunctionType type;
	char *name;
	VanMap *dict;

	VanWrap *bindObj;
	struct VanFunction *parent; // call 

	VanList *argList; // call
	void *code; // block ASTNode ??? WrapMethod

	char **argNameArr;
	int argArrLength;
	int *outFlagArr;
	
	VanMap *globalScopeDict;
	VanMap *upScopeDict;
	VanMap *inScopeDict;
	VanList *chainScopeList; // def

} VanFunction;

typedef struct VanClass{
	struct VanClass *cls;

	ClassType type;
	char *name;
	struct VanClass *base; // class

	VanMap *clsDescDict;
	VanMap *insDescDict;

	VanMap *dict;

} VanClass;

typedef struct VanInstance{
	VanClass *cls;

	VanMap *dict;

} VanInstance;

// shared VanFunction
typedef struct VanNativeFunction{
	char *name;
	VanMap *dict;

	VanWrap *bindObj;
	struct VanFunction *parent; // call 

	// void *code;  // WrapMethod
	WrapMethod method;

	char **argNameArr;
	int argArrLength;
	int *outFlagArr;

} VanNativeFunction;

typedef struct VanNativeClass{
	char *name;

	VanNativeFunction *functionArr;

} VanNativeClass;

typedef struct NativeData{
	char *name;

} VanNativeData;


// null/true/false/byte/int/float/native/void
/***
	wrap_retain
	wrap_release
	wrap_toString
****/
VanWrap *wrap_wrapNull();
VanWrap *wrap_wrapTrue();
VanWrap *wrap_wrapFalse();

VanWrap *wrap_wrapByte(signed char c);
VanWrap *wrap_wrapUByte(unsigned char c);
VanWrap *wrap_wrapShort(short c);
VanWrap *wrap_wrapUShort(unsigned short c);

VanWrap *wrap_wrapInt(int l);
VanWrap *wrap_wrapUInt(unsigned int l);
VanWrap *wrap_wrapLong(long l);
VanWrap *wrap_wrapLongLong(long long l);

VanWrap *wrap_wrapFloat(float d);
VanWrap *wrap_wrapDouble(double d);

VanWrap *wrap_wrapString(VanString *str);
VanWrap *wrap_wrapList(VanList *list);
VanWrap *wrap_wrapMap(VanMap *map);
VanWrap *wrap_wrapFunction(VanFunction *func);
VanWrap *wrap_wrapClass(VanClass *cls);
VanWrap *wrap_wrapInstance(VanInstance *ins);

VanWrap *wrap_wrapNativeFunction(VanNativeFunction *func);
VanWrap *wrap_wrapOpaquePointer(void *data);

VanWrap *wrap_wrapRefData(VanWrap **data);

void wrap_retain(VanWrap *wrap);
void wrap_release(VanWrap *wrap);
char *wrap_toString(VanWrap * wrap);

// string
/***
	string_length
	string_getValue
	string_setValue
	string_add
	string_mul
	string_subString
	string_replace
	string_find
	string_split
	string_reverse
	string_convertNumber
	string_slice
	string_equal
	string_toString
****/
VanString *string_create(char *data);
int string_length(VanString *str);
char *string_add(VanString *str1,char *data);
VanList *string_split(VanString *str1,char *data);
int string_setValue(VanString *str,char c);
char *string_getValue(VanString *str);
char *string_subString(VanString *str,int start,int end);

char *string_toString(VanString *str);
void string_free(VanString *str);

// list
/***
	list_push
	list_pop
	list_setValue
	list_getValue
	list_add
	list_remove
	list_sort
	list_filter
	list_map
	list_equal
	list_forEach
	list_reverse
	list_slice
	list_indexOf
	list_lastIndexOf
	list_toString
****/
VanList *list_create(int length);
int list_length(VanList *list);
VanList *list_push(VanList *list,VanWrap *value);
VanWrap *list_pop(VanList *list);
int list_indexOf(VanList *list,VanWrap *value);
int list_lastIndexOf(VanList *list,VanWrap *value);
void list_setValue(VanList *list,int index,VanWrap *value);
VanWrap *list_getValue(VanList *list,int index);

char *list_toString(VanList *list);
void list_free(VanList *list);


// map
/***
	map_setValue
	map_getValue
	map_keyArr
	map_valueArr
****/
VanMap *map_create();
int map_length(VanMap *map);
void map_setValue(VanMap *map,char *key,VanWrap *value);
void map_setRefValue(VanMap *map,char *key,VanWrap *value);
VanWrap *map_getValue(VanMap *map,char *key);
VanWrap *map_getRefValue(VanMap *map,char *key);
int map_hasKey(VanMap *map,char *key);
char **map_keyArr(VanMap *map);
VanList *map_valueArr(VanMap *map);

char *map_toString(VanMap *map);
void map_free(VanMap *map);

// function
/***
	function_bind
	function_unBind
	function_setValue
	function_getValue
	function_setInScopeValue
	function_getInScopeValue
	function_setUpScopeValue
	function_getUpScopeValue
****/
VanFunction *function_create(char *name,char **argNameArr,int *outFlagArr,int length);
void function_bind(VanFunction *func,VanWrap *obj);
VanWrap *function_unBind(VanFunction *func);
void function_setValue(VanFunction *func,char *key,VanWrap *value);
VanWrap *function_getValue(VanFunction *func,char *key);
void function_setInScopeValue(VanFunction *func,char *name,VanWrap *value);
VanWrap *function_getInScopeValue(VanFunction *func,char *name);
void function_setUpScopeValue(VanFunction *func,char *name,VanWrap *value);
VanWrap *function_getUpScopeValue(VanFunction *func,char *name);

char *function_toString(VanFunction *func);
void function_free(VanFunction *func);


// class
/***
	class_addClsField
	class_addInsField
	class_setValue
	class_getValue
	class_base

****/
VanClass *class_create(char *name,VanClass *base);
void class_addClsField(VanClass *cls,char *name,VanWrap *value);
void class_addInsField(VanClass *cls,char *name,VanWrap *value);
void class_addClsWrapMethod(VanClass *cls,char *name,FieldDesc desc,int *outFlag,WrapMethod method);
void class_addInsWrapMethod(VanClass *cls,char *name,FieldDesc desc,int *outFlag,WrapMethod method);
void class_setValue(VanClass *cls,char *key,VanWrap *value);
VanWrap *class_getValue(VanClass *cls,char *key,int isSuper);
VanClass *class_base(VanClass *cls);

char *class_toString(VanClass *cls);
void class_free(VanClass *cls);

// instance
/***
	instance_setValue
	instance_getValue
	instance_class

****/
VanWrap *instance_create(VanClass *cls,VanList *argList);
void instance_setValue(VanInstance *ins,char *key,VanWrap *value);
VanWrap *instance_getValue(VanInstance *ins,char *key,int isSuper);
VanClass *instance_class(VanInstance *ins);

char *instance_toString(VanInstance *ins);
void instance_free(VanInstance *ins);



// util 

VanWrap *wrap_binaryCal(VanWrap *leftWrap,VanWrap *rightWrap,ASTOp op);



#ifdef __cplusplus
}
#endif

#endif




