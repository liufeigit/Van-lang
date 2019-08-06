// clang 

#include "van_type.h"

// new String(""|true|false|null|1)
VanWrap *VanString_new(VanWrap *self,VanList *arglist){
	VanWrap *wStr=NULL;
	VanString *vStr=NULL;

	char *cStr="";

	VanWrap *_arg=NULL;

	if(!arglist){
		vStr=string_create(cStr);
		wStr=wrap_wrapString(vStr);

		return wStr;
	}

	_arg=list_getValue(arglist, 0);
	if(_arg->type==DataType_String){
		wStr=_arg;
	}
	else{
		cStr=wrap_toString(_arg->data);
		vStr=string_create(cStr);
		wStr=wrap_wrapString(vStr);
	}

	return wStr;
}

VanWrap *VanString_length(VanWrap *self,VanList *arglist){
	VanString *vStr=NULL;
	int length=0;

	VanWrap *wLength=NULL;

	vStr=self->data;
	length=string_length(vStr);
	wLength=wrap_wrapLongLong(length);

	return wLength;
}

VanWrap *VanString_add(VanWrap *self,VanList *arglist){
	
	return NULL;
}


VanWrap *VanString_split(VanWrap *self,VanList *arglist){
	
	return NULL;
}

VanWrap *VanString_setValue(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanString_getValue(VanWrap *self,VanList *arglist){

	return NULL;
}

VanWrap *VanString_subString(VanWrap *self,VanList *arglist){

	return NULL;
}

VanWrap *VanString_toString(VanWrap *self,VanList *arglist){

	return NULL;
}
VanWrap *VanString_free(VanWrap *self,VanList *arglist){

	return NULL;
}

VanWrap *VanList_new(VanWrap *self,VanList *arglist){
	VanWrap *wList=NULL;
	VanList *vlist=NULL;

	int length=-1;

	VanWrap *_arg=NULL;

	if(!arglist){
		length=0;
	}
	else{
		_arg=list_getValue(arglist, 0);
		if(_arg->type==DataType_Int){
			length=_arg->value.i2;
		}
	}
	
	if(length>-1){
		vlist=list_create(length);
		wList=wrap_wrapList(vlist);
	}

	return wList;
}

VanWrap *VanList_length(VanWrap *self,VanList *arglist){
	VanList *wList=NULL;

	int length=0;

	VanWrap *wLength=NULL;

	wList=self->data;
	length=list_length(wList);
	wLength=wrap_wrapLongLong(length);

	return wLength;
}

VanWrap *VanList_push(VanWrap *self,VanList *arglist){
	VanList *vList=NULL;

	VanWrap *_arg=NULL;

	vList=self->data;

	if(!arglist){
		return self;
	}

	_arg=list_getValue(arglist, 0);

	list_push(vList,_arg);

	return self;
}

VanWrap *VanList_pop(VanWrap *self,VanList *arglist){
	VanList *vList=NULL;
	VanWrap *wValue=NULL;

	vList=self->data;
	wValue=list_pop(vList);

	return wValue;
}

VanWrap *VanList_indexOf(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanList_lastIndexOf(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanList_setValue(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanList_getValue(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanList_toString(VanWrap *self,VanList *arglist){
	VanWrap *wStr=NULL;
	
	VanString *vStr=NULL;
	char *cStr=NULL;

	cStr=wrap_toString(self);
	vStr=string_create(cStr);
	wStr=wrap_wrapString(vStr);

	return wStr;
}

VanWrap *VanList_free(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_new(VanWrap *self,VanList *arglist){
	VanWrap *wMap=NULL;
	VanMap *vMap=NULL;

	vMap=map_create();
	wMap=wrap_wrapMap(vMap);

	return wMap;
}

VanWrap *VanMap_length(VanWrap *self,VanList *arglist){
	VanMap *wMap=NULL;

	int length=0;

	VanWrap *wLength=NULL;

	wMap=self->data;
	length=map_length(wMap);
	wLength=wrap_wrapLongLong(length);

	return wLength;
}

VanWrap *VanMap_setValue(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_getValue(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_hasKey(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_keyArr(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_valueArr(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_toString(VanWrap *self,VanList *arglist){


	return NULL;
}

VanWrap *VanMap_free(VanWrap *self,VanList *arglist){


	return NULL;
}

VanClass *wrap_createStringClass(){
	VanClass *cls=NULL;

	cls=class_create("String",NULL);
	cls->type=ClassType_NativeClass;

	return cls;
}

VanClass *wrap_createListClass(){
	VanClass *cls=NULL;

	cls=class_create("List",NULL);
	cls->type=ClassType_NativeClass;

	class_addClsWrapMethod(cls,"__new__",FieldDesc_IsNull,NULL,VanList_new);

	class_addInsWrapMethod(cls,"length",FieldDesc_IsGet,NULL,VanList_length);
	class_addInsWrapMethod(cls,"push",FieldDesc_IsNull,NULL,VanList_push);
	class_addInsWrapMethod(cls,"pop",FieldDesc_IsNull,NULL,VanList_pop);
	class_addInsWrapMethod(cls,"indexOf",FieldDesc_IsNull,NULL,VanList_indexOf);
	class_addInsWrapMethod(cls,"lastIndexOf",FieldDesc_IsNull,NULL,VanList_lastIndexOf);
	class_addInsWrapMethod(cls,"toString",FieldDesc_IsNull,NULL,VanList_toString);
	class_addInsWrapMethod(cls,"__free__",FieldDesc_IsNull,NULL,VanList_free);

	return cls;
}

VanClass *wrap_createMapClass(){
	VanClass *cls=NULL;

	cls=class_create("Map",NULL);
	cls->type=ClassType_NativeClass;

	class_addClsWrapMethod(cls,"__new__",FieldDesc_IsNull,NULL,VanMap_new);

	class_addInsWrapMethod(cls,"length",FieldDesc_IsGet,NULL,VanMap_length);
	class_addInsWrapMethod(cls,"hasKey",FieldDesc_IsNull,NULL,VanMap_hasKey);
	class_addInsWrapMethod(cls,"keyArr",FieldDesc_IsGet,NULL,VanMap_keyArr);
	class_addInsWrapMethod(cls,"valueArr",FieldDesc_IsGet,NULL,VanMap_valueArr);
	class_addInsWrapMethod(cls,"toString",FieldDesc_IsNull,NULL,VanMap_toString);
	class_addInsWrapMethod(cls,"__free__",FieldDesc_IsNull,NULL,VanMap_free);

	return cls;
}

VanClass *wrap_createFunctionClass(){
	VanClass *cls=NULL;

	cls=class_create("Function",NULL);
	cls->type=ClassType_NativeClass;

	return cls;
}

VanClass *wrap_createClassClass(){
	VanClass *cls=NULL;

	cls=class_create("Type",NULL);
	cls->type=ClassType_NativeClass;

	return cls;
}

VanWrap *wrap_setValueWithKey(VanWrap *wrap,char *key,VanWrap *value){

	return NULL;
}

VanWrap *wrap_setValueWithIndex(VanWrap *wrap,int index,VanWrap *value){

	return NULL;
}

VanWrap *wrap_getValueWithKey(VanWrap *wrap,char *key){

	return NULL;
}

VanWrap *wrap_getValueWithIndex(VanWrap *wrap,int index){

	return NULL;
}










