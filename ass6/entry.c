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

void destroyEntry(void* entry) {
	free(entry);
}


int compareEntry(void* entry1, void* entry2) {
	Entry* e1 = (Entry*) entry1;
	Entry* e2 = (Entry*) entry2;
	return e1->key > e2->key ? 1 : e1->key == e2->key ? 0 : -1;
}

int getValue(void* entry) {
	Entry* e = (Entry*) entry;
	return e->value;
}

void printEntry(void* entry) {
	Entry* e = (Entry*) entry;
	printf("[%d:%d]", e->key, e->value);
}
