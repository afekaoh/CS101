#ifndef HOMEWORK_BINTREE_H
#define HOMEWORK_BINTREE_H
/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/

/**********************************************************************************************************************
* this library is different from the regular implantation of BST as it's not a data structure but only a way to access
* the data, it's means that the responsibility to the data and it's validity it's on the user itself.
**********************************************************************************************************************/

#include "result.h"

typedef void* Type;

typedef struct BinTree BinTree;

///the functions that the tree needs to operate
typedef struct BinTreeFunctions {
	void (* destroy)(Type);
	
	int (* compare)(Type, Type);
	
	void (* print)(Type);
} BinTreeFunctions;

/*****************************************************************************************
* \input BinTreeFunctions functions - set of the generic functions of the data
* \brief the function create a pointer to binary tree allocates memory and sets its fields
*****************************************************************************************/
BinTree* initBinTree(BinTreeFunctions functions);

///freeing all the allocated memory
void destroyBinTree(BinTree* root);

///the function compute the size of the tree
int sizeOfBinTree(BinTree* root);

/*******************************************************************************
* \input BinTree* root - the tree to add the data to
* \input Type data - a pointer to the data to add
* \brief the function gets data and a tree, try to add the data to the tree and
* 		return if succeeded, failed or got a memory error
*******************************************************************************/
Result addToBinTree(BinTree* root, Type data);

/************************************************************************
* \input BinTree* root - the tree we search in
* \input Type data - the data we searching for
* \brief the function return the data given that it finds or null if not
************************************************************************/
Type findInBinTree(BinTree* root, Type data);

/******************************************************************************************************
* \input BinTree** rootPtr - a pointer to a root
* \input parent - the parent of the root (if the root it the main root of the tree his parent is NULL)
* \input Type data -a pointer to an equal data to the data we want to remove
* \return if it succeeded or failed (couldn't find) to remove the data
******************************************************************************************************/
Result removeFromBinTree(BinTree** rootPtr, BinTree* parent, Type data);

///the function prints the tree in InOrder sets by the compare function
void print_BinTree_In_Order(BinTree* root);

///the function prints the tree in PreOrder sets by the compare function
void print_BinTree_Pre_Order(BinTree* root);

///the function prints the tree in PostOrder sets by the compare function
void print_BinTree_Post_Order(BinTree* root);

#endif //HOMEWORK_BINTREE_H
