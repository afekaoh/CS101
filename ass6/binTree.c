/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/
#include "binTree.h"
#include <stdlib.h>
#include "data.h"
#include <stdio.h>
#include <assert.h>

struct BinTree {
	Ptr data;
	BinTree* left;
	BinTree* right;
	BinTreeFunctions functions;
};

///initialize the tree and allocate memory to him
BinTree* initBinTree(BinTreeFunctions functions) {
	BinTree* root = malloc(sizeof(BinTree));
	if (root == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	root->data = NULL;
	root->left = NULL;
	root->right = NULL;
	root->functions = functions;
	return root;
}

///freeing all the allocated memory
void destroyBinTree(BinTree* root) {
	if (root == NULL)
		return;
	root->functions.destroy(root->data);
	destroyBinTree(root->left);
	destroyBinTree(root->right);
	free(root);
}

///\return if the tree have no children
static int isChildless(BinTree* root) {
	assert(root != NULL);
	return root->left == NULL && root->right == NULL;
}

/**
* \input Ptr data - the data of the leaf
* \brief the function creates a leaf, allocates memory to him, initialize it's fields and returns it
* \note this function is parallel to initBinTree but with data input
**/
static BinTree* createLeaf(Ptr data, BinTreeFunctions functions) {
	assert(data != NULL);
	BinTree* leaf = malloc(sizeof(BinTree));
	if (leaf == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	leaf->functions = functions;
	leaf->data = data;
	leaf->right = NULL;
	leaf->left = NULL;
	return leaf;
}

///the function compute the size of the tree
int sizeOfBinTree(BinTree* root) {
	if (root == NULL)
		return 0;
	//if the data is NULL then we dont count the leaf in the tree
	return (root->data != NULL) + sizeOfBinTree(root->left) + sizeOfBinTree(root->right);
}

///the function gets a pointer to BinTree pointer and a Data and add it to the tree or its subtree
static Result addLeaf(BinTree** root, Ptr data, BinTreeFunctions functions) {
	assert(data != NULL);
	if (*root == NULL) {
		//there is open space here
		*root = createLeaf(data, functions);
		return *root == NULL ? MEM_ERROR : SUCCESS;
	} else
		//add the data to it's own subtree
		return addToBinTree(*root, data);
}

/**
* \input root - the tree to add the data to
* \input data - the data to add
* \brief the function gets data and a tree, try to add the data to the tree and
* 		return if succeeded, failed or got a memory error
**/
Result addToBinTree(BinTree* root, Ptr data) {
	assert(data != NULL);
	
	if (root == NULL)
		return FAILURE;
	
	if (root->data == NULL) {
		root->data = data;
		return SUCCESS;
	}
	
	int comper = root->functions.compare(root->data, data);
	if (comper == 0) {
		root->functions.destroy(root->data);
		root->data = data;
		return SUCCESS;
	}
	
	return addLeaf(comper > 0 ? &root->left : &root->right, data, root->functions);
}

/**
* \input root - the tree we search in
* \input Ptr data - the data we search for
* \brief the function return the data given that it finds or null if not
**/
Ptr findInBinTree(BinTree* root, Ptr data) {
	if (root == NULL || root->data == NULL)
		return NULL;
	int comper = root->functions.compare(root->data, data);
	if (comper == 0)
		return root->data;
	return findInBinTree(comper > 0 ? root->left : root->right, data);
}

///the function find the leaf with the maximum index
static BinTree* findMax(BinTree* root) {
	if (root == NULL)
		return NULL;
	BinTree* iterator = root;
	while (iterator->right != NULL)
		//the largest index in the tree is the most right leaf
		iterator = iterator->right;
	return iterator;
}

///switching the data between 2 leaves
static void switchData(BinTree* leaf1, BinTree* leaf2) {
	Ptr temp = leaf1->data;
	leaf1->data = leaf2->data;
	leaf2->data = temp;
}

///the function find for a given tree his parent tree
static BinTree* findParent(BinTree* root, BinTree* leaf) {
	if (root == NULL || isChildless(root))
		//if we fot to a leaf
		return NULL;
	
	if (root->left == leaf || root->right == leaf)
		//if root points to the leaf
		return root;
	
	//keep searching
	return findParent(root->functions.compare(root->data, leaf->data) > 0 ? root->left : root->right, leaf);
}

/**
*\input BinTree* root - the tree that we want to remove the data from
*\input Ptr data - the data to be removed
*\return if it succeeded to remove or not
*\brief the function gets a tree, find the node before him in inOrder form,
* 		replace them and remove the unwanted one
*/
static Result recursiveRemove(BinTree* root, Ptr data) {
	BinTree* temp = findMax(root->left);
	BinTree* parent = findParent(root, temp);
	switchData(root, temp);
	return removeFromBinTree(&temp, parent, data);
}

/**
* \input BinTree* parent - the tree that points to the toRemove
* \input BinTree* toRemove  - the node we want to be remove
* \input BinTree* child - the tree that toRemove points to
* \return succession
* \note the function assumes that toRemove have at most 1 child
**/
static Result removeLeaf(BinTree* parent, BinTree* toRemove, BinTree* child) {
	// remove the toRemove from the tree
	parent->left == toRemove ? (parent->left = child) : (parent->right = child);
	// remove the child tree from the toRemove tree
	toRemove->left == child ? (toRemove->left = NULL) : (toRemove->right = NULL);
	
	destroyBinTree(toRemove);
	return SUCCESS;
}

/**
* \input BinTree** rootPtr - a pointer to a root
* \input parent - the parent of the root (if the root it the main root of the tree his parent is NULL)
* \input Ptr data - the data we want to remove
* \return if it succeeded or failed (couldn't find) to remove the data
*/
Result removeFromBinTree(BinTree** rootPtr, BinTree* parent, Ptr data) {
	BinTree* root = *rootPtr;
	if (root == NULL)
		return FAILURE;
	
	//finding the node to be removed
	int comper = root->functions.compare(root->data, data);
	if (comper > 0)
		return removeFromBinTree(&root->left, root, data);
	if (comper < 0)
		return removeFromBinTree(&root->right, root, data);
	if (parent == NULL) {
		//if the node has no parent it's the root of the tree
		if (isChildless(root)) {
			//it's lonely tree
			destroyBinTree(root);
			*rootPtr = NULL;
			return SUCCESS;
		} else
			return recursiveRemove(root, data);
	}
	
	//case of at most 1 child
	if (root->left == NULL)
		return removeLeaf(parent, root, root->right);
	if (root->right == NULL)
		return removeLeaf(parent, root, root->left);
	
	//the tree has 2 children
	return recursiveRemove(root, data);
}

///the function prints the tree in order of it's indices
void print_BinTree_In_Order(BinTree* root) {
	if (root == NULL)
		return;
	if (root->data == NULL)
		return;
	
	print_BinTree_In_Order(root->left);
	root->functions.print(root->data);
	print_BinTree_In_Order(root->right);
}
