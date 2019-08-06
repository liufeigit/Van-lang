// clang

#include "van_builtin.h"


VanWrap *wrap_builtinList(){
	VanWrap *wrap=NULL;
	VanClass *listClass=NULL;

	VanWrap *lengthFunc=NULL;

	listClass=class_create("List", NULL);

	class_addInsField(listClass, "length", lengthFunc);
	
	
	

	return wrap;
}



VanWrap *VanString_new(VanWrap *data){

}

VanWrap *VanString_length(VanWrap *str){

}

VanWrap *VanString_add(VanWrap *str1,VanWrap *str2){

}

VanWrap *VanString_split(VanWrap *str1,VanWrap *data){

}

VanWrap *VanString_setValue(VanWrap *str,VanWrap *c){

}
VanWrap *VanString_getValue(VanWrap *str){

}
VanWrap *VanString_subString(VanWrap *str,VanWrap *start,VanWrap *end){

}

VanWrap *VanString_toString(VanWrap *str){

}

VanWrap *VanString_free(VanWrap *str){

}

VanWrap *VanList_new(VanWrap *length){

}

VanWrap *VanList_length(VanWrap *list){

}

VanWrap *VanList_push(VanWrap *list,VanWrap *value){

}

VanWrap *VanList_pop(VanWrap *list){

}

VanWrap *VanList_indexOf(VanWrap *list,VanWrap *value){

}

VanWrap *VanList_lastIndexOf(VanWrap *list,VanWrap *value){

}

VanWrap *VanList_setValue(VanWrap *list,VanWrap *index,VanWrap *value){

}

VanWrap *VanList_getValue(VanWrap *list,VanWrap *index){

}

VanWrap *VanList_toString(VanWrap *list){

}

VanWrap *VanList_free(VanWrap  *list){

}

VanWrap *VanMap_new(){

}

VanWrap *VanMap_length(VanWrap *map){

}

VanWrap *VanMap_setValue(VanWrap *map,VanWrap *key,VanWrap *value){

}

VanWrap *VanMap_getValue(VanWrap *map,VanWrap *key){

}

VanWrap *VanMap_hasKey(VanWrap *map,VanWrap *key){

}

VanWrap *VanMap_keyArr(VanWrap *map){

}

VanWrap *VanMap_valueArr(VanWrap *map){

}

VanWrap *VanMap_toString(VanWrap *map){

}

VanWrap *VanMap_free(VanWrap *map){
	
}












