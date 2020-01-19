/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include "binTree.h"
#include "result.h"
#include "entry.h"


struct Dictionary {
	BinTree* BinTree;
};

///the function for the BinTree
static const BinTreeFunctions ENTRY_FUNCTIONS = {destroyEntry, compareEntry, printEntry};

///allocating memory and initialize the dictionary
Dictionary* initDictionary() {
	Dictionary* d = malloc(sizeof(Dictionary));
	if (d == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	d->BinTree = initBinTree(ENTRY_FUNCTIONS);
	if (d->BinTree == NULL) {
		free(d);
		return NULL;
	}
	return d;
}

///freeing all the allocated memory
void destroyDictionary(Dictionary* d) {
	if (d == NULL)
		return;
	destroyBinTree(d->BinTree);
	free(d);
}

///returns how many entries there is in the dictionary
int sizeOfDictionary(Dictionary* d) {
	return sizeOfBinTree(d->BinTree);
}

///adds a new entry to the dictionary
Result putInDictionary(Dictionary* d, int key, int value) {
	Entry* entry = initEntry(key, value);
	if (entry == NULL)
		return MEM_ERROR;
	return addToBinTree(d->BinTree, entry);
}

///\return the value of a given key and 0 if couldn't find it
int getFromDictionary(Dictionary* d, int key) {
	Entry* temp = initEntry(key, 0);
	if (temp == NULL)
		return FAILURE;
	Entry* entry = findInBinTree(d->BinTree, temp);
	if (entry == NULL) {
		entry = temp;
	}
	int value = getValue(entry);
	destroyEntry(temp);
	return value;
}

///remove an entry from the dictionary
Result removeFromDictionary(Dictionary* d, int key) {
	Entry* entry = initEntry(key, 0);
	if (entry == NULL)
		return MEM_ERROR;
	Result result = removeFromBinTree(&d->BinTree, NULL, entry);
	destroyEntry(entry);
	return result;
}

///prints the dictionary in by the order of its keys
void printDictionary(Dictionary* d) {
	printf("{");
	if (d->BinTree != NULL) {
		print_BinTree_In_Order(d->BinTree);
	}
	printf("}");
}

///creat a new dictionary from a given arrays of keys and values. the function assumes valid input
Dictionary* createDictionaryFromArrays(int keys[], int values[], int size) {
	Dictionary* d = malloc(sizeof(Dictionary));
	if (d == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	d->BinTree = initBinTree(ENTRY_FUNCTIONS);
	if (d->BinTree == NULL) {
		free(d);
		return NULL;
	}
	for (int i = 0; i < size; i++) {
		Entry* entry = initEntry(keys[i], values[i]);
		if (entry == NULL) {
			destroyBinTree(d->BinTree);
			free(d);
			return NULL;
		}
		addToBinTree(d->BinTree, entry);
	}
	return d;
}
