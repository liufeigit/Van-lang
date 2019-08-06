#ifndef VAN_BUILTIN_H
#define VAN_BUILTIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "van_data.h"
#include "van_lex.h"

/***
	str1="123";
	str1=new String("123");
****/

VanWrap *wrap_setValueWithKey(VanWrap *wrap,char *key,VanWrap *value);
VanWrap *wrap_setValueWithIndex(VanWrap *wrap,int index,VanWrap *value);
VanWrap *wrap_getValueWithKey(VanWrap *wrap,char *key);
VanWrap *wrap_getValueWithIndex(VanWrap *wrap,int index);

VanWrap *wrap_not(VanWrap *wrap);
VanWrap *wrap_reverse(VanWrap *wrap);
VanWrap *wrap_neggative(VanWrap *wrap);

VanWrap *wrap_mul(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_div(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_mod(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_add(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_sub(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_leftShift(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_rightShift(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_gr(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_ge(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_ls(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_le(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_eq(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_ne(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_xor(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_xand(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_xer(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_and(VanWrap *wrap1,VanWrap *wrap2);
VanWrap *wrap_or(VanWrap *wrap1,VanWrap *wrap2);

VanWrap *wrap_builtinNumber();
VanWrap *wrap_builtinBoolean();
VanWrap *wrap_builtinString();
VanWrap *wrap_builtinList();
VanWrap *wrap_builtinMap();
VanWrap *wrap_builtinFunction();
VanWrap *wrap_builtinClass();

VanWrap *VanString_new(VanWrap *data);
VanWrap *VanString_length(VanWrap *str);
VanWrap *VanString_add(VanWrap *str1,VanWrap *str2);
VanWrap *VanString_split(VanWrap *str1,VanWrap *data);
VanWrap *VanString_setValue(VanWrap *str,VanWrap *c);
VanWrap *VanString_getValue(VanWrap *str);
VanWrap *VanString_subString(VanWrap *str,VanWrap *start,VanWrap *end);
VanWrap *VanString_toString(VanWrap *str);
VanWrap *VanString_free(VanWrap *str);

VanWrap *VanList_new(VanWrap *length);
VanWrap *VanList_length(VanWrap *list);
VanWrap *VanList_push(VanWrap *list,VanWrap *value);
VanWrap *VanList_pop(VanWrap *list);
VanWrap *VanList_indexOf(VanWrap *list,VanWrap *value);
VanWrap *VanList_lastIndexOf(VanWrap *list,VanWrap *value);
VanWrap *VanList_setValue(VanWrap *list,VanWrap *index,VanWrap *value);
VanWrap *VanList_getValue(VanWrap *list,VanWrap *index);
VanWrap *VanList_toString(VanWrap *list);
VanWrap *VanList_free(VanWrap *list);

VanWrap *VanMap_new();
VanWrap *VanMap_length(VanWrap *map);
VanWrap *VanMap_setValue(VanWrap *map,VanWrap *key,VanWrap *value);
VanWrap *VanMap_getValue(VanWrap *map,VanWrap *key);
VanWrap *VanMap_hasKey(VanWrap *map,VanWrap *key);
VanWrap *VanMap_keyArr(VanWrap *map);
VanWrap *VanMap_valueArr(VanWrap *map);
VanWrap *VanMap_toString(VanWrap *map);
VanWrap *VanMap_free(VanWrap *map);






#ifdef __cplusplus
}
#endif

#endif







