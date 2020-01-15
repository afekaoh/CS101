#ifndef HOMEWORK_BINTREE_H
#define HOMEWORK_BINTREE_H

#include "globals.h"
#include "data.h"

///implantation of a generic binary search tree with indexed data
typedef struct BinTree BinTree;

typedef struct {
	void (* destroy)(Type);
	
	int (* compare)(Type, Type);
	
	void (* print)(Type);
} BinTreeFunctions;


BinTree* initBinTree(BinTreeFunctions functions);

void destroyBinTree(BinTree* root);

int sizeOfBinTree(BinTree* root);

Result addToBinTree(BinTree* root, Type data);

Type findInBinTree(BinTree* root, Type data);

Result removeFromBinTree(BinTree** rootPtr, BinTree* parent, Type data);

void print_BinTree_In_Order(BinTree* root);

#endif //HOMEWORK_BINTREE_H
