#ifndef HOMEWORK_BINTREE_H
#define HOMEWORK_BINTREE_H

#include "globals.h"
#include "data.h"

typedef struct BinTree BinTree;

BinTree* initBinTree();

void destroyBinTree(BinTree* head);

int sizeOfBinTree(BinTree* head);

Result addToBinTree(BinTree* head, Data*);

BinTree* findInBinTree(BinTree* head, int key);

Result removeFromBinTree(BinTree* head, int key);

void print_BinTree_In_Order(BinTree* root);

Data* getData(BinTree* head);

BinTree* createBinTreeFromArrays(int keys[], int values[], int size);

#endif //HOMEWORK_BINTREE_H
