// clang 
#include "van_parseUtil.h"

ASTNode *astNode_getRootNode(ASTNode *node){
	ASTNode *rootNode=NULL;

	if(node){
		rootNode=node;
		while(rootNode->parent){
			rootNode=rootNode->parent;
		}
	}

	return rootNode;
}

ASTNode *astNode_findLoseNode(ASTNode *node){
	ASTNode *loseNode=NULL;
	ASTNode **arr=NULL;

	if(node){
		arr=(ASTNode **)node->childArr;
		if(node->op>ASTOp_BinaryStart&&
			node->op<ASTOp_BinaryEnd){
			if(arr){
				for(int i=0;i<node->childLength;i++){
					if(!arr[i]){
						loseNode=node;
						break;
					}
				}
			}
		}

		if(!loseNode){
			for(int i=0;i<node->childLength;i++){
				loseNode=astNode_findLoseNode(arr[i]);
				if(loseNode){
					break;
				}
			}
		}
	}

	return loseNode;
}

ASTNode *astNode_findBinaryGreaterNode(ASTNode *node,ASTOp op){
	ASTNode *greaterNode=NULL;

	if(node){
		if(node->op>ASTOp_BinaryStart&&
			node->op<ASTOp_BinaryEnd){
			if(op%10<node->op%10){
				greaterNode=node;
			}
		}

		if(!greaterNode){
			greaterNode=astNode_findBinaryGreaterNode(node->parent,op);
		}
	}

	return greaterNode;
}

ASTNode *astNode_mergeNode(ASTNode *leftNode,ASTNode *rightNode){
	ASTNode *loseNode=NULL;
	ASTNode *greaterNode=NULL;
	ASTOp op1=0;
	ASTOp op2=0;

	if(!leftNode){
		return rightNode;
	}

	loseNode=astNode_findLoseNode(leftNode);
	op1=loseNode->op;
	op2=rightNode->op;

	if(op2>ASTOp_BinaryStart&&
		op2<ASTOp_BinaryEnd&&
		op2%10>=op1%10){ // op2<=op1

		greaterNode=astNode_findBinaryGreaterNode(loseNode,op2);

		loseNode->childArr[1]=rightNode->childArr[0];
		((ASTNode *)loseNode->childArr[1])->parent=loseNode;
		if(greaterNode){
			rightNode->childArr[0]=greaterNode->childArr[1];
			((ASTNode *)rightNode->childArr[0])->parent=rightNode;

			greaterNode->childArr[1]=rightNode;
			rightNode->parent=greaterNode;
		}
		else{
			
			rightNode->childArr[0]=leftNode;
			leftNode->parent=rightNode;

			leftNode=rightNode;
		}
	}
	else{ // op2>op1 || ???
		loseNode->childArr[1]=rightNode;
		rightNode->parent=loseNode;
	}

	return leftNode;
}

int astNode_all(ASTNode *node,FlagHander hander){
	int length=0;
	ASTNode **nodeArr=NULL;
	int flag=0;

	flag=hander(node);
	if(flag){
		length=node->childLength;
		nodeArr=node->childArr;
		for(int i=0;i<length;i++){
			flag=astNode_all(nodeArr[i], hander);
			if(!flag){
				break;
			}
		}
	}

	return flag;
}

int astNode_has(ASTNode *node,FlagHander hander){
	int length=0;
	ASTNode **nodeArr=NULL;
	int flag=0;

	flag=hander(node);
	if(!flag){
		length=node->childLength;
		nodeArr=node->childArr;
		for(int i=0;i<length;i++){
			flag=astNode_all(nodeArr[i], hander);
			if(flag){
				break;
			}
		}
	}

	return flag;
}

// class --> new
int astNode_hasNewParent(ASTNode *node){
	int flag=0;

	ASTNode *parent=NULL;

	parent=node->parent;
	while(parent){
		if(parent->op==ASTOp_New){
			flag=1;
			break;
		}
		else if(parent->op==ASTOp_Prior){
			parent=parent->parent;
			continue;
		}
		else{
			break;
		}
	}

	return flag;
}

// new -->class
int astNode_hasClassChild(ASTNode *node){
	int flag=0;

	ASTNode **nodeArr=NULL;

	nodeArr=node->childArr;
	while(nodeArr){
		if(nodeArr[0]->op==ASTOp_Call){
			if(nodeArr[0]->childArr[0]->wrap->type==DataType_Class){
				flag=1;
			}
			break;
		}
		else if(nodeArr[0]->op==ASTOp_Prior){
			nodeArr=nodeArr[0]->childArr;
			continue;
		}
		else{
			break;
		}
	}

	return flag;
}

// 递归线性展开
void astNode_vist(ASTNode *node,VistHandler handler){
	

}

void astNode_free(ASTNode *node){
	
}







