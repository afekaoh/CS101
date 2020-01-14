#ifndef HOMEWORK_BINTREE_H
#define HOMEWORK_BINTREE_H

#include "globals.h"
#include "data.h"

///implantation of a generic binary search tree with indexed data
typedef struct BinTree BinTree;
typedef void* Ptr;

typedef struct {
	void (* destroy)(Ptr);
	
	int (* compare)(Ptr, Ptr);
	
	void (* print)(Ptr);
} BinTreeFunctions;


BinTree* initBinTree(BinTreeFunctions functions);

void destroyBinTree(BinTree* root);

int sizeOfBinTree(BinTree* root);

Result addToBinTree(BinTree* root, Data* data);

Data* findInBinTree(BinTree* root, Data* data);

Result removeFromBinTree(BinTree** rootPtr, BinTree* parent, Data* data);

void print_BinTree_In_Order(BinTree* root);

#endif //HOMEWORK_BINTREE_H
