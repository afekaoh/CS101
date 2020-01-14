/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/
#include "dictionary.h"
#include "binTree.h"
#include "globals.h"
#include <stdlib.h>
#include "data.h"
#include <stdio.h>

struct Dictionary {
	BinTree* binTree;
};

///allocating memory and initialize the dictionary
Dictionary* initDictionary() {
	BinTreeFunctions functions = {(void*) destroyData, (void*) comperData, (void*) printData};
	Dictionary* d = malloc(sizeof(Dictionary));
	if (d == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	d->binTree = initBinTree(functions);
	if (d->binTree == NULL) {
		free(d);
		return NULL;
	}
	return d;
}

///freeing all the allocated memory
void destroyDictionary(Dictionary* d) {
	if (d == NULL)
		return;
	destroyBinTree(d->binTree);
	free(d);
}

///returns how many element there is in the dictionary
int sizeOfDictionary(Dictionary* d) {
	return sizeOfBinTree(d->binTree);
}

///adds a new element to the dictionary
Result putInDictionary(Dictionary* d, int key, int value) {
	Data* newData = initData(key, value);
	if (newData == NULL)
		return MEM_ERROR;
	return addToBinTree(d->binTree, newData);
}

///\return the value of a given key and 0 if couldn't find it
int getFromDictionary(Dictionary* d, int key) {
	Data* temp = initData(key, 0);
	Data* data = findInBinTree(d->binTree, temp);
	if (data == NULL) {
		data = temp;
	}
	int value = getValue(data);
	destroyData(temp);
	return value;
}

///remove an element from the dictionary
Result removeFromDictionary(Dictionary* d, int key) {
	Data* data = initData(key, 0);
	if (data == NULL)
		return MEM_ERROR;
	Result result = removeFromBinTree(&d->binTree, NULL, data);
	destroyData(data);
	return result;
}

///prints the dictionary in by the order of its keys
void printDictionary(Dictionary* d) {
	printf("{");
	if (d->binTree != NULL) {
		print_BinTree_In_Order(d->binTree);
	}
	printf("}\n");
}

///creat a new dictionary from a given arrays of keys and values the function assumes valid input
Dictionary* createDictionaryFromArrays(int keys[], int values[], int size) {
	BinTreeFunctions functions = {(void*) destroyData, (void*) comperData, (void*) printData};
	Dictionary* d = malloc(sizeof(Dictionary));
	if (d == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	d->binTree = initBinTree(functions);
	if (d->binTree == NULL) {
		free(d);
		return NULL;
	}
	for (int i = 0; i < size; i++) {
		Data* data = initData(keys[i], values[i]);
		if (data == NULL) {
			destroyBinTree(d->binTree);
			free(d);
			return NULL;
		}
		addToBinTree(d->binTree, data);
	}
	return d;
}
