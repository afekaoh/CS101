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
	Data* data;
	BinTree* left;
	BinTree* right;
};


BinTree* initBinTree() {
	BinTree* root = malloc(sizeof(BinTree));
	if (root == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	root->data = NULL;
	root->left = NULL;
	root->right = NULL;
	return root;
}

void destroyBinTree(BinTree* root) {
	if (root == NULL)
		return;
	destroyData(root->data);
	destroyBinTree(root->left);
	destroyBinTree(root->right);
	free(root);
}

static int isLeaf(BinTree* root) {
	assert(root != NULL);
	return root->left == NULL && root->right == NULL;
}

static BinTree* createLeaf(Data* data) {
	BinTree* leaf = malloc(sizeof(BinTree));
	if (leaf == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	leaf->data = data;
	leaf->right = NULL;
	leaf->left = NULL;
	return leaf;
}

int sizeOfBinTree(BinTree* root) {
	if (root == NULL)
		return 0;
	
	return (root->data != NULL) + sizeOfBinTree(root->left) + sizeOfBinTree(root->right);
}

Result addToBinTree(BinTree* root, Data* newData) {
	if (root == NULL)
		return FAILURE;
	
	if (root->data == NULL) {
		root->data = newData;
		return SUCCESS;
	}
	if (getIndex(root->data) == getIndex(newData)) {
		destroyData(root->data);
		root->data = newData;
		return SUCCESS;
	}
	
	if (getIndex(root->data) > getIndex(newData)) {
		if (root->left == NULL) {
			root->left = createLeaf(newData);
			return root->left == NULL ? MEM_ERROR : SUCCESS;
		} else
			return addToBinTree(root->left, newData);
	}
	
	if (getIndex(root->data) < getIndex(newData)) {
		if (root->right == NULL) {
			root->right = createLeaf(newData);
			return root->right == NULL ? MEM_ERROR : SUCCESS;
		} else
			return addToBinTree(root->right, newData);
	}
	return FAILURE;
}

BinTree* findInBinTree(BinTree* root, int key) {
	if (root != NULL) {
		if (root->data == NULL)
			return NULL;
		if (getIndex(root->data) == key)
			return root;
		if (getIndex(root->data) > key)
			return findInBinTree(root->left, key);
		if (getIndex(root->data) < key)
			return findInBinTree(root->right, key);
	}
	return NULL;
}

static BinTree* findMax(BinTree* root) {
	if (root == NULL)
		return NULL;
	BinTree* iterator = root;
	while (iterator->right != NULL)
		iterator = iterator->right;
	return iterator;
}

static BinTree* findMin(BinTree* root) {
	if (root == NULL)
		return NULL;
	BinTree* iterator = root;
	while (iterator->left != NULL)
		iterator = iterator->left;
	return iterator;
}

static void switchData(BinTree* leaf1, BinTree* leaf2) {
	Data* temp = leaf1->data;
	leaf1->data = leaf2->data;
	leaf2->data = temp;
}

static BinTree* findParent(BinTree* root, BinTree* leaf) {
	if (root == NULL || isLeaf(root))
		return NULL;
	if (root->left == leaf || root->right == leaf)
		return root;
	if (getIndex(root->data) > getIndex(leaf->data))
		return findParent(root->left, leaf);
	if (getIndex(root->data) < getIndex(leaf->data))
		return findParent(root->right, leaf);
	return NULL;
}

Result removeFromBinTree(BinTree** head, BinTree* parent, int key) {
	BinTree* root = *head;
	if (root == NULL)
		return FAILURE;
	
	if (getIndex(root->data) > key)
		return removeFromBinTree(&(root->left), root, key);
	
	if (getIndex(root->data) < key)
		return removeFromBinTree(&(root->right), root, key);
	
	if (getIndex(root->data) == key) {
		if (parent == NULL) {
			destroyBinTree(root);
			*head = NULL;
			return SUCCESS;
		}
		if (root->left == NULL) {
			if (parent->right == root)
				parent->right = root->right;
			else
				parent->left = root->right;
			
			root->right = NULL;
			destroyBinTree(root);
			return SUCCESS;
		}
		if (root->right == NULL) {
			if (parent->right == root)
				parent->right = root->left;
			else
				parent->left = root->left;
			
			root->left = NULL;
			destroyBinTree(root);
			return SUCCESS;
		}
		
		BinTree* temp = findMax(root->left);
		switchData(root, temp);
		return removeFromBinTree(&temp, findParent(root, temp), key);
	}
	return FAILURE;
}

void print_BinTree_In_Order(BinTree* root) {
	if (root == NULL)
		return;
	if (root->data == NULL)
		return;
	print_BinTree_In_Order(root->left);
	printData(root->data);
	print_BinTree_In_Order(root->right);
}

BinTree* createBinTreeFromArrays(int keys[], int values[], int size) {
	BinTree* root = initBinTree();
	if (root == NULL)
		return NULL;
	for (int i = 0; i < size; i++) {
		Data* data = initData();
		if (data == NULL) {
			destroyBinTree(root);
			return NULL;
		}
		setData(data, keys[i], values[i]);
		if (addToBinTree(root, data) == MEM_ERROR) {
			destroyData(data);
			destroyBinTree(root);
			return NULL;
		}
	}
	return root;
}

Data* getData(BinTree* root) {
	if (root == NULL)
		return NULL;
	return root->data;
}
