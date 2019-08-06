#ifndef VAN_TYPE_H
#define VAN_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "van_data.h"

// String
VanWrap *VanString_new(VanWrap *self,VanList *arglist);
VanWrap *VanString_length(VanWrap *self,VanList *arglist);
VanWrap *VanString_add(VanWrap *self,VanList *arglist);
VanWrap *VanString_split(VanWrap *self,VanList *arglist);
VanWrap *VanString_setValue(VanWrap *self,VanList *arglist);
VanWrap *VanString_getValue(VanWrap *self,VanList *arglist);
VanWrap *VanString_subString(VanWrap *self,VanList *arglist);
VanWrap *VanString_toString(VanWrap *self,VanList *arglist);
VanWrap *VanString_free(VanWrap *self,VanList *arglist);

// List
VanWrap *VanList_new(VanWrap *self,VanList *arglist);
VanWrap *VanList_length(VanWrap *self,VanList *arglist);
VanWrap *VanList_push(VanWrap *self,VanList *arglist);
VanWrap *VanList_pop(VanWrap *self,VanList *arglist);
VanWrap *VanList_indexOf(VanWrap *self,VanList *arglist);
VanWrap *VanList_lastIndexOf(VanWrap *self,VanList *arglist);
VanWrap *VanList_setValue(VanWrap *self,VanList *arglist);
VanWrap *VanList_getValue(VanWrap *self,VanList *arglist);
VanWrap *VanList_toString(VanWrap *self,VanList *arglist);
VanWrap *VanList_free(VanWrap *self,VanList *arglist);

// Map
VanWrap *VanMap_new(VanWrap *self,VanList *arglist);
VanWrap *VanMap_length(VanWrap *self,VanList *arglist);
VanWrap *VanMap_setValue(VanWrap *self,VanList *arglist);
VanWrap *VanMap_getValue(VanWrap *self,VanList *arglist);
VanWrap *VanMap_hasKey(VanWrap *self,VanList *arglist);
VanWrap *VanMap_keyArr(VanWrap *self,VanList *arglist);
VanWrap *VanMap_valueArr(VanWrap *self,VanList *arglist);
VanWrap *VanMap_toString(VanWrap *self,VanList *arglist);
VanWrap *VanMap_free(VanWrap *self,VanList *arglist);

VanClass *wrap_createStringClass();
VanClass *wrap_createListClass();
VanClass *wrap_createMapClass();

VanClass *wrap_createFunctionClass();
VanClass *wrap_createClassClass();
VanClass *wrap_createMapClass();

VanWrap *wrap_setValueWithKey(VanWrap *wrap,char *key,VanWrap *value);
VanWrap *wrap_setValueWithIndex(VanWrap *wrap,int index,VanWrap *value);
VanWrap *wrap_getValueWithKey(VanWrap *wrap,char *key);
VanWrap *wrap_getValueWithIndex(VanWrap *wrap,int index);

#ifdef __cplusplus
}
#endif

#endif




