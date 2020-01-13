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

static int isLeaf(BinTree* root) {
	return root->right == NULL && root->left == NULL ? 1 : 0;
}

int sizeOfBinTree(BinTree* root) {
	if (root == NULL)
		return 0;
	if (isLeaf(root))
		return 1;
	return sizeOfBinTree(root->left) + sizeOfBinTree(root->right);
}

Result addToBinTree(BinTree* root, Data* d) {
	if (root == NULL)
		return FAILURE;
	if (root->data == NULL)
		root->data = d;
	
	if (getIndex(root->data) == getIndex(d)) {
		destroyData(root->data);
		root->data = d;
		return SUCCESS;
	}
	
	if (getIndex(root->data) > getIndex(d)) {
		if (root->left == NULL) {
			root->left = createLeaf(d);
			return root->left == NULL ? MEM_ERROR : SUCCESS;
		} else
			return addToBinTree(root->left, d);
	}
	
	if (getIndex(root->data) < getIndex(d)) {
		if (root->right == NULL) {
			root->right = createLeaf(d);
			return root->right == NULL ? MEM_ERROR : SUCCESS;
		} else
			return addToBinTree(root->right, d);
	}
	return FAILURE;
}

BinTree* findInBinTree(BinTree* root, int key) {
	if (root != NULL) {
		if (getIndex(root->data) == key)
			return root;
		if (getIndex(root->data) < key)
			return findInBinTree(root->left, key);
		if (getIndex(root->data) > key)
			return findInBinTree(root->right, key);
	}
	return NULL;
}

Result removeFromBinTree(BinTree* root, int key) {
	return SUCCESS;
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
	return root->data;
}
