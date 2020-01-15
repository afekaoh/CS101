/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/
#include <stdlib.h>
#include <stdio.h>
#include "entry.h"

struct Entry {
	int key;
	int value;
};

/// \brief the function initialize a new Entry struct
void* initEntry(int key, int value) {
	Entry* data = malloc(sizeof(Entry));
	if (data == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	data->key = key;
	data->value = value;
	return data;
}

///freeing the allocated memory
void destroyEntry(void* entry) {
	free(entry);
}

/**
* \input Entry* entry1
* \input Entry* entry2
* \return 1 if entry1 is bigger -1 if smaller and 0 if equal to entry2
* \brief compering the keys of the element
*/
int comperEntry(void* entry1, void* entry2) {
	Entry* e1 = (Entry*) entry1;
	Entry* e2 = (Entry*) entry2;
	return e1->key > e2->key ? 1 : e1->key == e2->key ? 0 : -1;
}

///\return the value of the entry
int getValue(void* entry) {
	Entry* e = (Entry*) entry;
	return e->value;
}

/// prints the entry
void printEntry(void* entry) {
	Entry* e = (Entry*) entry;
	printf("[%d:%d]", e->key, e->value);
}
