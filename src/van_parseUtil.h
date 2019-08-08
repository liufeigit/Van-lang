#ifndef VAN_PARSEUNTIL_H
#define VAN_PARSEUNTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "van_parse.h"

typedef int (*FlagHander)(ASTNode *node);
typedef int (*VistHandler)(ASTNode *node);

ASTNode *astNode_getRootNode(ASTNode *node);
ASTNode *astNode_mergeNode(ASTNode *leftNode,ASTNode *rightNode);
ASTNode *astNode_findBinaryLoseNode(ASTNode *node);
ASTNode *astNode_findBinaryGreaterNode(ASTNode *node,ASTOp op);

int astNode_all(ASTNode *node,FlagHander hander);
int astNode_has(ASTNode *node,FlagHander hander);

int astNode_hasNewParent(ASTNode *node);
int astNode_hasClassChild(ASTNode *node);

void astNode_vist(ASTNode *node,VistHandler handler);
void astNode_free(ASTNode *node);


#ifdef __cplusplus
}
#endif

#endif







