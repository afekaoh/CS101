/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/
#include <stdlib.h>
#include "data.h"
#include <stdio.h>

struct Data {
	int index;
	int value;
};

/// \brief the function initialize a new Data struct
Data* initData(int index, int value) {
	Data* data = malloc(sizeof(Data));
	if (data == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	data->index = index;
	data->value = value;
	return data;
}

///freeing the allocated memory
void destroyData(Data* data) {
	free(data);
}

int comperData(Data* d1, Data* d2) {
	return d1->index > d2->index ? 1 : d1->index == d2->index ? 0 : -1;
}

///\return the value of the data
int getValue(Data* data) {
	return data->value;
}

/// prints the data
void printData(Data* data) {
	printf("[%d:%d]", data->index, data->value);
}
