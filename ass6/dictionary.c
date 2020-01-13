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
	Data* newData = initData();
	if (newData == NULL)
		return MEM_ERROR;
	setData(newData, key, value);
	Result result = addToBinTree(d->binTree, newData);
	return result;
}

int getFromDictionary(Dictionary* d, int key) {
	Data* data = getData(findInBinTree(d->binTree, key));
	if (data == NULL)
		return FAILURE;
	return getValue(data);
}

Result removeFromDictionary(Dictionary* d, int key) {
	return removeFromBinTree(&d->binTree, NULL, key);
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
