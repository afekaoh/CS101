/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/
#include "binTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct BinTree {
	Type data;
	BinTree* left;
	BinTree* right;
	BinTreeFunctions functions;
};


/**
* \input BinTreeFunctions functions - set of the generic functions of the data
* \brief the function create a pointer to binary tree allocates memory and sets its fields
*/
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

///the function compute the size of the tree
int sizeOfBinTree(BinTree* root) {
	if (root == NULL)
		return 0;
	//if the data is NULL then we dont count the leaf in the tree
	return (root->data != NULL) + sizeOfBinTree(root->left) + sizeOfBinTree(root->right);
}

///the function gets a pointer to BinTree pointer and a Entry and add it to the tree or its subtree
static Result addLeaf(BinTree** root, Type data, BinTreeFunctions functions) {
	assert(data != NULL);
	if (*root == NULL) {
		//there is free space here
		*root = initBinTree(functions);
		return *root == NULL ? MEM_ERROR : addToBinTree(*root, data);
	} else
		//add the data to it's own subtree
		return addToBinTree(*root, data);
}

/**
* \input BinTree* root - the tree to add the data to
* \input Type data - the data to add
* \brief the function gets data and a tree, try to add the data to the tree and
* 		return if succeeded, failed or got a memory error
**/
Result addToBinTree(BinTree* root, Type data) {
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
* \input BinTree* root - the tree we search in
* \input Type data - the data we searching for
* \brief the function return the data given that it finds or null if not
**/
Type findInBinTree(BinTree* root, Type data) {
	if (root == NULL || root->data == NULL)
		return NULL;
	int comper = root->functions.compare(root->data, data);
	if (comper == 0)
		return root->data;
	return findInBinTree(comper > 0 ? root->left : root->right, data);
}

///the function find the leaf with the maximum key
static BinTree* findMax(BinTree* root) {
	if (root == NULL)
		return NULL;
	BinTree* iterator = root;
	while (iterator->right != NULL)
		//the largest key in the tree is the most right leaf
		iterator = iterator->right;
	return iterator;
}

static BinTree* findMin(BinTree* root) {
	if (root == NULL)
		return NULL;
	BinTree* iterator = root;
	while (iterator->left != NULL)
		//the smallest tree is the most left leaf
		iterator = iterator->left;
	return iterator;
}

///switching the data between 2 leaves
static void switchData(BinTree* leaf1, BinTree* leaf2) {
	Type temp = leaf1->data;
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
* \input BinTree* parent - the tree that points to the toRemove
* \input BinTree* toRemove  - the node we want to be remove
* \input BinTree* child - the tree that toRemove points to
* \return succession
* \note the function assumes that toRemove have at most 1 child
**/
static Result removeLeaf(BinTree* parent, BinTree* toRemove, BinTree* child) {
	assert(parent != NULL);
	// remove the toRemove from the parent tree
	parent->left == toRemove ? (parent->left = child) : (parent->right = child);
	// remove the child tree from the toRemove tree
	toRemove->left == child ? (toRemove->left = NULL) : (toRemove->right = NULL);
	
	destroyBinTree(toRemove);
	return SUCCESS;
}

static Result removeRoot(BinTree* root) {
	if (isChildless(root)) {
		//it's lonely tree
		root->functions.destroy(root->data);
		root->data = NULL;
		return SUCCESS;
	}
	if (root->left == NULL) {
		BinTree* min = findMin(root->right);
		BinTree* minParent = findParent(root, min);
		switchData(root, min);
		return removeLeaf(minParent, min, min->right);
	} else {
		BinTree* max = findMax(root->left);
		BinTree* maxParent = findParent(root, max);
		switchData(root, max);
		return removeLeaf(maxParent, max, max->left);
	}
}

/**
* \input BinTree** rootPtr - a pointer to a root
* \input parent - the parent of the root (if the root it the main root of the tree his parent is NULL)
* \input Type data - the data we want to remove
* \return if it succeeded or failed (couldn't find) to remove the data
*/
Result removeFromBinTree(BinTree** rootPtr, BinTree* parent, Type data) {
	BinTree* root = *rootPtr;
	if (root == NULL || root->data == NULL)
		return FAILURE;
	
	//finding the node to be removed
	int comper = root->functions.compare(root->data, data);
	if (comper > 0)
		return removeFromBinTree(&root->left, root, data);
	if (comper < 0)
		return removeFromBinTree(&root->right, root, data);
	
	//comper==0
	if (parent == NULL)
		//it's the root
		return removeRoot(root);
	
	//case of at most 1 child
	if (root->left == NULL)
		return removeLeaf(parent, root, root->right);
	if (root->right == NULL)
		return removeLeaf(parent, root, root->left);
	
	//the root has 2 children
	BinTree* max = findMax(root->left);
	BinTree* maxParent = findParent(root, max);
	switchData(root, max);
	return removeLeaf(maxParent, max, max->left);
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
