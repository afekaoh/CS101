#include "dictionary.h"
#include "binTree.h"
#include "globals.h"
#include <stdlib.h>
#include "data.h"
#include <stdio.h>

struct Dictionary {
	BinTree* binTree;
};

Dictionary* initDictionary() {
	Dictionary* d = malloc(sizeof(Dictionary));
	if (d == NULL)
		return NULL;
	d->binTree = initBinTree();
	if (d->binTree == NULL) {
		free(d);
		return NULL;
	}
	return d;
}

void destroyDictionary(Dictionary* d) {
	destroyBinTree(d->binTree);
	free(d);
}

int sizeOfDictionary(Dictionary* d) {
	return sizeOfBinTree(d->binTree);
}

Result putInDictionary(Dictionary* d, int key, int value) {
	Data* data = initData();
	if (data == NULL)
		return MEM_ERROR;
	setData(data, key, value);
	Result result = addToBinTree(d->binTree, data);
	return result;
}

int getFromDictionary(Dictionary* d, int key) {
	BinTree* leaf = findInBinTree(d->binTree, key);
	if (leaf == NULL)
		return FAILURE;
	return getValue(getData(leaf));
}

Result removeFromDictionary(Dictionary* d, int key) {
	Result result = removeFromBinTree(d->binTree, key);
	return result;
}

void printDictionary(Dictionary* d) {
	printf("{");
	if (d->binTree != NULL) {
		print_BinTree_In_Order(d->binTree);
	}
	printf("}\n");
}

Dictionary* createDictionaryFromArrays(int keys[], int values[], int size) {
	Dictionary* d = malloc(sizeof(Dictionary));
	if (d == NULL)
		return NULL;
	d->binTree = createBinTreeFromArrays(keys, values, size);
	if (d->binTree == NULL) {
		free(d);
		return NULL;
	}
	return d;
}
