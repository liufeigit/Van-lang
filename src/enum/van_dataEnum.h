#ifndef VAN_DATAENUM_H
#define VAN_DATAENUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


#ifdef __cplusplus
}
#endif

#endif




