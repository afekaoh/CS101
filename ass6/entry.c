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
Entry* initEntry(int key, int value) {
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
void destroyEntry(Entry* entry) {
	free(entry);
}

/**
* \input Entry* entry1
* \input Entry* entry2
* \return 1 if entry1 is bigger -1 if smaller and 0 if equal to entry2
* \brief compering the keys of the element
*/
int comperEntry(Entry* entry1, Entry* entry2) {
	return entry1->key > entry2->key ? 1 : entry1->key == entry2->key ? 0 : -1;
}

///\return the value of the entry
int getValue(Entry* entry) {
	return entry->value;
}

/// prints the entry
void printEntry(Entry* entry) {
	printf("[%d:%d]", entry->key, entry->value);
}
