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

int sizeOfBinTree(BinTree* root) {
	if (root == NULL)
		return 0;
	//if the data is NULL then we dont count the leaf in the tree
	return (root->data != NULL) + sizeOfBinTree(root->left) + sizeOfBinTree(root->right);
}

///the function gets a pointer to BinTree pointer and a Entry and add it to the tree or its subtree
static Result addLeaf(BinTree** root, Type data, BinTreeFunctions functions) {
	if (*root == NULL) {
		//there is free space here
		*root = initBinTree(functions);
		return *root == NULL ? MEM_ERROR : addToBinTree(*root, data);
	} else
		//add the data to it's own subtree
		return addToBinTree(*root, data);
}


Result addToBinTree(BinTree* root, Type data) {
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


Type findInBinTree(BinTree* root, Type data) {
	if (root == NULL || root->data == NULL)
		return NULL;
	int comper = root->functions.compare(root->data, data);
	if (comper == 0)
		return root->data;
	return findInBinTree(comper > 0 ? root->left : root->right, data);
}

///the function find the leaf with the maximum data sets by the compare function
static BinTree* findMax(BinTree* root) {
	if (root == NULL)
		return NULL;
	BinTree* iterator = root;
	while (iterator->right != NULL)
		//the largest tree is the most right leaf
		iterator = iterator->right;
	return iterator;
}

///the function find the leaf with the minimum data sets by the compare function
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
		//the root can't be a parent
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
	assert(parent != NULL && toRemove != NULL);
	// remove the toRemove from the parent tree
	parent->left == toRemove ? (parent->left = child) : (parent->right = child);
	// remove the child tree from the toRemove tree
	toRemove->left == child ? (toRemove->left = NULL) : (toRemove->right = NULL);
	
	destroyBinTree(toRemove);
	return SUCCESS;
}

///the functions deals with the situation we need to remove the actual root of the tree
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


void print_BinTree_In_Order(BinTree* root) {
	if (root == NULL)
		return;
	if (root->data == NULL)
		return;
	
	print_BinTree_In_Order(root->left);
	root->functions.print(root->data);
	print_BinTree_In_Order(root->right);
}


void print_BinTree_Pre_Order(BinTree* root) {
	if (root == NULL)
		return;
	if (root->data == NULL)
		return;
	
	root->functions.print(root->data);
	print_BinTree_Pre_Order(root->left);
	print_BinTree_Pre_Order(root->right);
}

void print_BinTree_Post_Order(BinTree* root) {
	if (root == NULL)
		return;
	if (root->data == NULL)
		return;
	
	print_BinTree_Post_Order(root->left);
	print_BinTree_Post_Order(root->right);
	root->functions.print(root->data);
	
}