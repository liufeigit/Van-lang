// clang 

#include "van_data.h"
#include <stdlib.h>

static Node *node_getNodeFromIndex(Node *head,int index,int length);
static Node *node_getNodeFormKey(Node *head,char *key,int length,int *flag);

void __map_setValue(VanMap *map,char *key,VanWrap *value,int outFlag);

// string
VanString *string_create(char *data){
	VanString *str=NULL;
	int length=0;

	str=(VanString *)calloc(1, sizeof(VanString ));

	length=strlen(data);
	str->length=length;
	str->capacity=length;
	str->data=data;

	return str;
}

int string_length(VanString *str){
	int length=0;

	if(str){
		length=str->length;
	}

	return length;
}

char *string_add(VanString *str1,char *data){


	return NULL;
}

VanList *string_split(VanString *str1,char *data){


	return NULL;
}

int string_setValue(VanString *str,char c){


	return 0;
}

char *string_getValue(VanString *str){


	return NULL;
}

char *string_subString(VanString *str,int start,int end){


	return NULL;
}


char *string_toString(VanString *str){
	char *str1=NULL;
	int len1=0;

	if(str){
		if(str->data){
			len1=strlen(str->data);
			str1=(char *)calloc(len1+5, sizeof(char ));

			strcpy(str1, str->data);

			// str1[0]='\"';
			// strcpy(str1+1, str->data);
			// str1[len1+1]='\"';
		}
	}

	return str1;
}

void string_free(VanString *str){

	if(str){
		free(str->data);
		free(str);
	}
}

// list
VanList *list_create(int length){
	VanList *list=NULL;
	Node *head=NULL;
	Node *tail=NULL;

	Node *preNode=NULL;
	Node *curNode=NULL;

	list=(VanList *)calloc(1, sizeof(VanList ));
	head=(Node *)calloc(1, sizeof(Node ));
	tail=(Node *)calloc(1, sizeof(Node ));
	head->next=tail;
	tail->pre=head;

	list->length=length;
	list->capacity=length;

	preNode=head;
	if(length){
		for(int i=0;i<length;i++){
			curNode=(Node *)calloc(1, sizeof(Node ));
			curNode->data=wrap_wrapNull();

			preNode->next=curNode;
			curNode->pre=preNode;
			preNode=curNode;
		}
		preNode->next=tail;
		tail->pre=preNode;
	}

	list->head=head;
	list->tail=tail;

	return list;
}

int list_length(VanList *list){
	int length=0;

	if(list){
		length=list->length;
	}

	return length;
}

VanList *list_push(VanList *list,VanWrap *value){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *cur=NULL;
	Node *last=NULL;

	length=list->length;
	head=list->head;
	tail=list->tail;

	cur=(Node *)calloc(1, sizeof(Node ));
	wrap_retain(value);
	cur->data=value;

	if(length){
		last=tail->pre;

		last->next=cur;
		cur->pre=last;

		cur->next=tail;
		tail->pre=cur;
	}
	else{
		head->next=cur;
		cur->pre=head;

		cur->next=tail;
		tail->pre=cur;
		
	}

	length++;
	list->length=length;

	return list;
}

VanWrap *list_pop(VanList *list){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *last=NULL;
	Node *lastPre=NULL;
	VanWrap *value=NULL;

	length=list->length;
	head=list->head;
	tail=list->tail;
	if(length){
		last=tail->pre;
		lastPre=last->pre;

		lastPre->next=tail;
		tail->pre=lastPre;

		// last->next=NULL;
		// last->pre=NULL;

		length--;
	}

	list->length=length;

	return value;
}

int list_indexOf(VanList *list,VanWrap *value){
	int index=-1;


	return index;
}

int list_lastIndexOf(VanList *list,VanWrap *value){
	int index=-1;


	return index;
}

void list_setValue(VanList *list,int index,VanWrap *value){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *rNode=NULL;
	Node *preNode=NULL;
	Node *nextNode=NULL;
	Node *cur=NULL;

	length=list->length;
	if(index<length&&index>=0){
		head=list->head;
		tail=list->tail;

		cur=(Node *)calloc(1, sizeof(Node ));
		wrap_retain(value);
		cur->data=value;

		rNode=node_getNodeFromIndex(head, index, length);
		preNode=rNode->pre;
		nextNode=rNode->next;

		preNode->next=cur;
		cur->pre=preNode;

		cur->next=nextNode;
		nextNode->pre=cur;

		wrap_release(rNode->data);
		free(rNode->userData);
		free(rNode);
	}
	
}

VanWrap *list_getValue(VanList *list,int index){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	VanWrap *value=NULL;
	Node *rNode=NULL;

	length=list->length;
	if(index<length&&index>=0){
		head=list->head;
		tail=list->tail;

		rNode=node_getNodeFromIndex(head, index, length);
		value=rNode->data;
	}
	
	return value;
}

char *list_toString(VanList *list){
	char *str=NULL;
	int len=0;
	int length1=128;

	int length=0;
	Node *head=NULL;
	Node *tail=NULL;
	Node *cur=NULL;

	length=list_length(list);
	head=list->head;
	tail=list->tail;
	cur=head->next;

	str=(char *)calloc(length1, sizeof(char ));
	str[0]='[';
	len++;
	while(cur!=tail){
		int _len=0;
		char *_str=NULL;

		_str=wrap_toString(cur->data);
		_len=strlen(_str);
		if(_len+5>=length1-len){
			str=realloc(str, length1+_len+5);
			length1=length1+_len+5;
		}
		strcpy(str+len,_str);
		len+=_len;
		if(cur->next!=tail){
			str[len]=',';
			len++;
		}

		cur=cur->next;
	}
	str[len]=']';
	str[len+1]='\0';

	return str;
}

void list_free(VanList  *list){

}


// map
VanMap *map_create(){
	VanMap *map=NULL;
	Node *head=NULL;
	Node *tail=NULL;

	map=(VanMap *)calloc(1, sizeof(VanMap ));
	head=(Node *)calloc(1, sizeof(Node ));
	tail=(Node *)calloc(1, sizeof(Node ));
	head->next=tail;
	tail->pre=head;

	map->head=head;
	map->tail=tail;

	return map;
}

int map_length(VanMap *map){
	int length=0;

	if(map){
		length=map->length;
	}

	return length;
}

void __map_setValue(VanMap *map,char *key,VanWrap *value,int outFlag){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *rNode=NULL;
	Node *preNode=NULL;
	Node *nextNode=NULL;
	Node *cur=NULL;

	int flag=-1;

	length=map->length;
	head=map->head;
	tail=map->tail;

	cur=(Node *)calloc(1, sizeof(Node ));
	wrap_retain(value);
	cur->data=value;

	rNode=node_getNodeFormKey(head,key,length,&flag);
	if(flag>-1){ // 存在

		if(rNode->data){
			if(rNode->data->type==DataType_RefData&&outFlag){
				VanWrap **ref=NULL;

				ref=(VanWrap **)rNode->data->data;
				*ref=value;
			}
			else{
				rNode->data=value;
			}
		}
		else{
			rNode->data=value;
		}
		// preNode=rNode->pre;
		// nextNode=rNode->next;

		// preNode->next=cur;
		// cur->pre=preNode;

		// cur->next=nextNode;
		// nextNode->pre=cur;

		// wrap_release(rNode->data);
		// free(rNode->userData);
		// free(rNode);
	}
	else{
		Node *last=NULL;

		// cur->userData=key;
		cur->userData=(char *)calloc(strlen(key)+1, sizeof(char ));
		strcpy((char *)cur->userData,key);
		if(length){
			last=tail->pre;

			last->next=cur;
			cur->pre=last;

			cur->next=tail;
			tail->pre=cur;
		}
		else{
			head->next=cur;
			cur->pre=head;

			cur->next=tail;
			tail->pre=cur;
		}

		length++;
	}

	map->length=length;
}

void map_setValue(VanMap *map,char *key,VanWrap *value){

	__map_setValue(map,key,value,1);

}

void map_setRefValue(VanMap *map,char *key,VanWrap *value){

	__map_setValue(map,key,value,0);

}

VanWrap *map_getValue(VanMap *map,char *key){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *rNode=NULL;
	VanWrap *value=NULL;
	VanWrap **ref=NULL;

	length=map->length;
	head=map->head;
	tail=map->tail;

	rNode=node_getNodeFormKey(head,key,length,NULL);
	if(rNode){
		value=rNode->data;
		if(value){
			if(value->type==DataType_RefData){
				ref=(VanWrap **)value->data;
				value=*ref;
			}
		}
		
	}

	return value;
}

VanWrap *map_getRefValue(VanMap *map,char *key){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *rNode=NULL;
	VanWrap *value=NULL;
	VanWrap **ref=NULL;

	length=map->length;
	head=map->head;
	tail=map->tail;

	rNode=node_getNodeFormKey(head,key,length,NULL);
	if(rNode){
		ref=&rNode->data;
		value=wrap_wrapRefData(ref);
	}

	return value;
}

int map_hasKey(VanMap *map,char *key){
	int length=0;
	Node *head=NULL;
	Node *tail=NULL;

	Node *rNode=NULL;
	int flag=0;

	length=map->length;
	head=map->head;
	tail=map->tail;

	if(length){
		rNode=node_getNodeFormKey(head,key,length,&flag);
		flag=(flag>-1?1:0);
	}
	
	return flag;
}

char **map_keyArr(VanMap *map){
	char **keyArr=NULL;
	int len=0;

	int length=0;
	Node *head=NULL;
	Node *tail=NULL;
	Node *cur=NULL;

	length=map->length;
	head=map->head;
	tail=map->tail;
	cur=head->next;

	if(length){
		keyArr=(char **)calloc(length, sizeof(char *));
		while(cur!=tail){
			char *_key=NULL;
			int _len=0;

			_key=(char *)cur->userData;
			_len=strlen(_key)+1;

			keyArr[len]=(char *)calloc(_len+1, sizeof(char ));
			strcpy(keyArr[len],_key);

			len++;
			cur=cur->next;
		}
	}

	return keyArr;
}

VanList *map_valueArr(VanMap *map){
	VanList *list=NULL;

	int length=0;
	Node *head=NULL;
	Node *tail=NULL;
	Node *cur=NULL;

	length=map->length;
	head=map->head;
	tail=map->tail;
	cur=head->next;

	list=list_create(0);
	while(cur!=tail){
		list_push(list, cur->data);
		cur=cur->next;
	}

	return list;
}

char *map_toString(VanMap *map){
	char *str=NULL;
	int len=0;
	int length1=128;

	int length=0;
	Node *head=NULL;
	Node *tail=NULL;
	Node *cur=NULL;

	length=map_length(map);
	head=map->head;
	tail=map->tail;
	cur=head->next;

	str=(char *)calloc(length1, sizeof(char ));
	str[0]='{';
	len++;
	while(cur!=tail){
		int _len1=0;
		int _len2=0;
		char *_str1=NULL;
		char *_str2=NULL;

		_str1=cur->userData;
		_len1=strlen(_str1);
		_str2=wrap_toString(cur->data);
		_len2=strlen(_str2);
		if(_len1+_len2+5>=length1-_len1-_len2){
			str=realloc(str, length1+_len1+_len2+5);
			length1=length1+_len1+_len2+5;
		}

		strcpy(str+len,_str1);
		len+=_len1;

		str[len]=':';
		len++;

		strcpy(str+len,_str2);
		len+=_len2;

		if(cur->next!=tail){
			str[len]=',';
			len++;
		}

		cur=cur->next;
	}
	str[len]='}';
	str[len+1]='\0';

	return str;
}

void map_free(VanMap *map){

}

// function
VanFunction *function_create(char *name,char **argNameArr,int *outFlagArr,int length){
	VanFunction *func=NULL;

	VanMap *dict=NULL;

	// VanList *argList=NULL;
	// VanMap *upScopeDict=NULL;
	// VanMap *globalScopeDict=NULL;
	// VanMap *inScopeDict=NULL;
	// VanList *chainScopeList=NULL;

	func=(VanFunction *)calloc(1, sizeof(VanFunction ));

	dict=map_create();

	func->name=name;
	func->dict=dict;
	func->argNameArr=argNameArr;
	func->argArrLength=length;
	func->outFlagArr=outFlagArr;

	// upScopeDict=map_create();
	// globalScopeDict=map_create();
	// inScopeDict=map_create();
	// for(int i=0;i<length;i++){
	// 	map_setValue(inScopeDict, argNameArr[i], NULL);
	// }

	// argList=list_create(0);
	// chainScopeList=list_create(0);

	// func->argList=argList;
	// func->chainScopeList=chainScopeList; 

	// func->upScopeDict=upScopeDict;
	// func->globalScopeDict=inScopeDict;
	// func->inScopeDict=inScopeDict;

	return func;
}

void function_bind(VanFunction *func,VanWrap *obj){

	if(func&&obj){
		wrap_retain(obj);
		func->bindObj=obj;
	}
}

VanWrap *function_unBind(VanFunction *func){
	VanWrap *obj=NULL;

	if(func){
		wrap_release(func->bindObj);
		obj=func->bindObj;
		func->bindObj=NULL;
	}

	return obj;
}

void function_setValue(VanFunction *func,char *key,VanWrap *value){

	if(func){
		if(func->dict){
			map_setValue(func->dict, key, value);
		}
	}
}

VanWrap *function_getValue(VanFunction *func,char *key){
	VanWrap *value=NULL;

	if(func){
		if(func->dict){
			value=map_getValue(func->dict, key);
		}
	}

	return value;
}

void function_setInScopeValue(VanFunction *func,char *name,VanWrap *value){

	if(func){
		if(func->inScopeDict){
			map_setValue(func->inScopeDict, name, value);
		}
	}
}

VanWrap *function_getInScopeValue(VanFunction *func,char *name){
	VanWrap *value=NULL;

	if(func){
		if(func->inScopeDict){
			value=map_getValue(func->inScopeDict, name);
		}
	}

	return value;
}

void function_setUpScopeValue(VanFunction *func,char *name,VanWrap *value){

	if(func){
		if(func->upScopeDict){
			map_setValue(func->upScopeDict, name, value);
		}
	}
}

VanWrap *function_getUpScopeValue(VanFunction *func,char *name){
	VanWrap *value=NULL;

	if(func){
		if(func->upScopeDict){
			value=map_getValue(func->upScopeDict, name);
		}
	}

	return value;
}

char *function_toString(VanFunction *func){
	char *str=NULL;

	char *str1="function ";
	char *str2=NULL;
	char *str3="{[code...]}";

	int len1=0;
	int len2=0;
	int len3=0;

	str2=func->name;
	len1=strlen(str1);
	if(str2){
		len2=strlen(str2);
	}
	len3=strlen(str3);

	str=(char *)calloc(len1+len2+len3+1, sizeof(char ));
	strcpy(str,str1);
	if(len2){
		strcpy(str+len1,str2);
	}
	strcpy(str+(len1+len2),str3);

	return str;
}

void function_free(VanFunction *func){

}


// class
VanClass *class_create(char *name,VanClass *base){
	VanClass *cls=NULL;

	VanMap *clsDescDict=NULL;
	VanMap *insDescDict=NULL;
	VanMap *dict=NULL;

	cls=(VanClass *)calloc(1, sizeof(VanClass ));
	clsDescDict=map_create();
	insDescDict=map_create();
	dict=map_create();

	cls->name=name;
	cls->base=base;

	cls->clsDescDict=clsDescDict;
	cls->insDescDict=insDescDict;
	cls->dict=dict;

	return cls;
}

void class_addClsField(VanClass *cls,char *name,VanWrap *value){

	if(cls){
		if(cls->clsDescDict){
			map_setValue(cls->clsDescDict, name, value); // ???
		}
	}

}

void class_addInsField(VanClass *cls,char *name,VanWrap *value){

	if(cls){
		if(cls->insDescDict){
			map_setValue(cls->insDescDict, name, value); // ???
		}
	}

}

void class_addClsWrapMethod(VanClass *cls,char *name,FieldDesc desc,int *outFlag,WrapMethod method){
	VanWrap *value=NULL;
	VanFunction *function=NULL;

	function=function_create(NULL, NULL, outFlag, 0);
	function->type=FunctionType_NativeFunction;
	function->code=method;

	value=wrap_wrapFunction(function);
	value->_fieldDesc=desc;

	class_addClsField(cls,name,value);
}

void class_addInsWrapMethod(VanClass *cls,char *name,FieldDesc desc,int *outFlag,WrapMethod method){
	VanWrap *value=NULL;
	VanFunction *function=NULL;

	function=function_create(NULL, NULL, outFlag, 0);
	function->type=FunctionType_NativeFunction;
	function->code=method;

	value=wrap_wrapFunction(function);
	value->_fieldDesc=desc;

	class_addInsField(cls,name,value);
}

void class_setValue(VanClass *cls,char *key,VanWrap *value){
	VanClass *base=NULL;

	VanMap *dict=NULL;
	VanMap *clsDescDict=NULL;

	int flag=0;

	base=cls;
	dict=base->dict;
	while(base){
		clsDescDict=base->clsDescDict;

		flag=map_hasKey(clsDescDict, key);
		if(flag){ // 成功
			map_setValue(dict, key, value);
			break;
		}
		base=base->base;
	}

}

VanWrap *class_getValue(VanClass *cls,char *key,int isSuper){
	VanWrap *value=NULL;
	VanClass *base=NULL;

	VanMap *dict=NULL;
	VanMap *clsDescDict=NULL;

	int flag=0;

	base=cls;
	if(!isSuper){
		while(base){
			clsDescDict=base->clsDescDict;
			dict=base->dict;

			flag=map_hasKey(dict, key);
			if(flag){ // ???
				value=map_getValue(dict, key);
				if(value){
					break;
				}
			}

			flag=map_hasKey(clsDescDict, key);
			if(flag){ // ???
				value=map_getValue(clsDescDict, key);
				if(value){
					break;
				}
				
			}
			base=base->base;
		}

		base=cls; // ???
	}
	else{
		base=base->base;
		while(base){
			clsDescDict=base->clsDescDict;
			dict=base->dict;

			flag=map_hasKey(dict, key);
			if(flag){ // ???
				value=map_getValue(dict, key);
				if(value){
					base=cls->base; // ???
					break;
				}
			}

			flag=map_hasKey(clsDescDict, key);
			if(flag){ // ???
				value=map_getValue(clsDescDict, key);
				if(value){
					base=cls->base; // ???
					break;
				}
				
			}
			base=base->base;
		}

		if(!value){
			base=cls;
			dict=base->dict;

			flag=map_hasKey(dict, key);
			if(flag){ // ???
				value=map_getValue(dict, key);
			}

			if(!value){
				clsDescDict=cls->clsDescDict;

				flag=map_hasKey(clsDescDict, key);
				if(flag){ // ???
					value=map_getValue(clsDescDict, key);
				}
			}
		}
	}

	if(value){
		if(value->type==DataType_Function){ // bind
			VanFunction *_fun=NULL;

			_fun=value->data;
			function_bind(_fun, wrap_wrapClass(base));
		}
	}

	return value;
}

VanClass *class_base(VanClass *cls){
	VanClass *base=NULL;

	if(cls){
		base=cls->base;
	}

	return base;
}

char *class_toString(VanClass *cls){
	char *str=NULL;

	char *str1="class ";
	char *str2=NULL;
	char *str3="{[code...]}";

	int len1=0;
	int len2=0;
	int len3=0;

	str2=cls->name;
	len1=strlen(str1);
	if(str2){
		len2=strlen(str2);
	}
	len3=strlen(str3);

	str=(char *)calloc(len1+len2+len3+1, sizeof(char ));
	strcpy(str,str1);
	if(len2){
		strcpy(str+len1,str2);
	}
	strcpy(str+(len1+len2),str3);

	return str;
}

void class_free(VanClass *cls){

}

// instance
VanWrap *instance_create(VanClass *cls,VanList *argList){
	VanWrap *wrap=NULL;

	VanInstance *ins=NULL;
	VanMap *dict=NULL;

	if(cls->type==ClassType_NativeClass){
		VanWrap *_value=NULL;
		VanFunction *_func=NULL;
		WrapMethod _method=NULL;

		_value=map_getValue(cls->clsDescDict, "__new__");
		if(_value->type==DataType_Function){
			_func=_value->data;
			if(_func->type==FunctionType_NativeFunction){
				_method=_func->code;
				wrap=_method(NULL,argList);
			}
		}
	}
	else{
		ins=(VanInstance *)calloc(1, sizeof(VanInstance ));
		dict=map_create();

		ins->cls=cls;
		ins->dict=dict;

		wrap=wrap_wrapInstance(ins);
	}

	return wrap;
}

void instance_setValue(VanInstance *ins,char *key,VanWrap *value){
	VanClass *cls=NULL;

	VanMap *dict=NULL;
	VanMap *insDescDict=NULL;

	int flag=0;

	cls=ins->cls;
	dict=ins->dict;
	while(cls){
		insDescDict=cls->insDescDict;

		flag=map_hasKey(insDescDict, key);
		if(flag){ // 成功
			map_setValue(dict, key, value);
			break;
		}
		cls=cls->base;
	}
}

VanWrap *instance_getValue(VanInstance *ins,char *key,int isSuper){
	VanWrap *value=NULL;
	VanClass *cls=NULL;

	VanMap *dict=NULL;
	VanMap *insDescDict=NULL;

	int flag=0;

	cls=ins->cls;
	dict=ins->dict;

	if(!isSuper){
		flag=map_hasKey(dict, key);
		if(flag){ // ???
			value=map_getValue(dict, key);
		}

		if(!value){
			while(cls){
				insDescDict=cls->insDescDict;

				flag=map_hasKey(insDescDict, key);
				if(flag){ // ???
					value=map_getValue(insDescDict, key);
					if(value){
						break;
					}
					
				}
				cls=cls->base;
			}
		}
	}
	else{
		cls=cls->base;
		while(cls){
			insDescDict=cls->insDescDict;

			flag=map_hasKey(insDescDict, key);
			if(flag){ // ???
				value=map_getValue(insDescDict, key);
				if(value){
					break;
				}
				
			}
			cls=cls->base;
		}

		if(!value){
			flag=map_hasKey(dict, key);
			if(flag){ // ???
				value=map_getValue(dict, key);
			}
		}

		if(!value){
			cls=ins->cls;
			insDescDict=cls->insDescDict;

			flag=map_hasKey(insDescDict, key);
			if(flag){ // ???
				value=map_getValue(insDescDict, key);
			}
		}
	}

	if(value){
		if(value->type==DataType_Function){ // bind
			VanFunction *_fun=NULL;

			_fun=value->data;
			function_bind(_fun, wrap_wrapInstance(ins));
		}
	}

	return value;
}

VanClass *instance_class(VanInstance *ins){
	VanClass *cls=NULL;

	if(ins){
		cls=ins->cls;
	}

	return cls;
}

char *instance_toString(VanInstance *ins){
	char *str=NULL;

	char *str1="object ";
	char *str2=NULL;
	char *str3="{[code...]}";

	int len1=0;
	int len2=0;
	int len3=0;

	str2=ins->cls->name;
	len1=strlen(str1);
	if(str2){
		len2=strlen(str2);
	}
	len3=strlen(str3);

	str=(char *)calloc(len1+len2+len3+1, sizeof(char ));
	strcpy(str,str1);
	if(len2){
		strcpy(str+len1,str2);
	}
	strcpy(str+(len1+len2),str3);

	return str;
}

void instance_free(VanInstance *ins){

}

// wrap
VanWrap *wrap_wrapNull(){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Null;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapTrue(){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_True;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapFalse(){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_False;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapByte(signed char c){
	return wrap_wrapLongLong(c);
}

VanWrap *wrap_wrapUByte(unsigned char c){
	return wrap_wrapLongLong(c);
}

VanWrap *wrap_wrapShort(short s){
	return wrap_wrapLongLong(s);
}

VanWrap *wrap_wrapUShort(unsigned short s){
	return wrap_wrapLongLong(s);
}

VanWrap *wrap_wrapInt(int i){
	return wrap_wrapLongLong(i);
}

VanWrap *wrap_wrapUInt(unsigned int i){
	return wrap_wrapLongLong(i);
}

VanWrap *wrap_wrapLong(long l){
	return wrap_wrapLongLong(l);
}

VanWrap *wrap_wrapLongLong(long long l){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Int;
	wrap->_referNum=1;
	wrap->value.l=l;

	return wrap;
}

VanWrap *wrap_wrapFloat(float d){
	return wrap_wrapDouble(d);
}

VanWrap *wrap_wrapDouble(double d){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Float;
	wrap->_referNum=1;
	wrap->value.d=d;

	return wrap;
}

VanWrap *wrap_wrapNativeFunction(VanNativeFunction *function){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_NativeFunction;
	wrap->data=function;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapOpaquePointer(void *data){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_OpaquePointer;
	wrap->data=data;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapString(VanString *str){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_String;
	wrap->data=str;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapList(VanList *list){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_List;
	wrap->data=list;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapMap(VanMap *map){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Map;
	wrap->data=map;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapFunction(VanFunction *func){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Function;
	wrap->data=func;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapClass(VanClass *cls){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Class;
	wrap->data=cls;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapInstance(VanInstance *ins){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_Instance;
	wrap->data=ins;
	wrap->_referNum=1;

	return wrap;
}

VanWrap *wrap_wrapRefData(VanWrap **data){
	VanWrap *wrap=NULL;

	wrap=(VanWrap *)calloc(1,sizeof(VanWrap ));

	wrap->type=DataType_RefData;
	wrap->data=data;
	wrap->_referNum=1;

	return wrap;
}

void wrap_retain(VanWrap *wrap){

	if(wrap){
		wrap->_referNum++;
	}

}

// 循环引用 ??????
void wrap_release(VanWrap *wrap){

	if(wrap){
		wrap->_referNum--;
		if(!wrap->_referNum){
			if(wrap->type==DataType_String){
				string_free(wrap->data);
			}
			else if(wrap->type==DataType_List){
				list_free(wrap->data);
			}
			else if(wrap->type==DataType_Map){
				map_free(wrap->data);
			}
			else if(wrap->type==DataType_Function){
				function_free(wrap->data);
			}
			else if(wrap->type==DataType_Class){
				class_free(wrap->data);
			}
			else if(wrap->type==DataType_Instance){
				instance_free(wrap->data);
			}
		}
	}
}

char *wrap_toString(VanWrap * wrap){
	char *str=NULL;

	if(wrap){
		if(wrap->type==DataType_Null){
			int _len=0;
			char *_str="null";

			_len=strlen(_str)+1;
			str=(char *)calloc(_len, sizeof(char ));
			strcpy(str,_str);
		}
		else if(wrap->type==DataType_True){
			int _len=0;
			char *_str="true";

			_len=strlen(_str)+1;
			str=(char *)calloc(_len, sizeof(char ));
			strcpy(str,_str);
		}
		else if(wrap->type==DataType_False){
			int _len=0;
			char *_str="false";

			_len=strlen(_str)+1;
			str=(char *)calloc(_len, sizeof(char ));
			strcpy(str,_str);
		}
		else if(wrap->type==DataType_Int){
			str=(char *)calloc(30, sizeof(char ));
			sprintf(str, "%lld",wrap->value.l);
		}
		else if(wrap->type==DataType_Float){
			str=(char *)calloc(30, sizeof(char ));
			str=gcvt(wrap->value.d, 30, str);
		}
		else if(wrap->type==DataType_String){
			str=string_toString(wrap->data);
		}
		else if(wrap->type==DataType_List){
			str=list_toString(wrap->data);
		}
		else if(wrap->type==DataType_Map){
			str=map_toString(wrap->data);
		}
		else if(wrap->type==DataType_Function){
			str=function_toString(wrap->data);
		}
		else if(wrap->type==DataType_Class){
			str=class_toString(wrap->data);
		}
		else if(wrap->type==DataType_Instance){
			str=instance_toString(wrap->data);
		}
		else if(wrap->type==DataType_NativeFunction){
			int _len=0;
			char *_str="NativeFunction";

			_len=strlen(_str)+1;
			str=(char *)calloc(_len, sizeof(char ));
			strcpy(str,_str);
		}
		else if(wrap->type==DataType_NativeData){
			int _len=0;
			char *_str="NativeData";

			_len=strlen(_str)+1;
			str=(char *)calloc(_len, sizeof(char ));
			strcpy(str,_str);
		}
		else if(wrap->type==DataType_OpaquePointer){
			int _len=0;
			char *_str="OpaquePointer";

			_len=strlen(_str)+1;
			str=(char *)calloc(_len, sizeof(char ));
			strcpy(str,_str);
		}
	}

	return str;
}

static Node *node_getNodeFromIndex(Node *head,int index,int length){
	Node *rNode=NULL;
	Node *cur=NULL;

	cur=head;
	for(int i=0;i<length;i++){
		cur=cur->next;
		if(index==i){
			rNode=cur;
			break;
		}
	}

	return rNode;
}

static Node *node_getNodeFormKey(Node *head,char *key,int length,int *flag){
	Node *rNode=NULL;
	Node *cur=NULL;
	int _flag=-1;

	cur=head;
	for(int i=0;i<length;i++){
		cur=cur->next;
		if(strcmp(key,(char *)cur->userData)==0){
			rNode=cur;
			_flag=i;
			break;
		}
	}

	if(flag){
		*flag=_flag;
	}

	return rNode;
}





