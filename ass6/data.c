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

Data* initData() {
	Data* data = malloc(sizeof(Data));
	if (data == NULL) {
		printf("memory error:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	return data;
}

void setData(Data* data, int index, int value) {
	data->value = value;
	data->index = index;
}

void destroyData(Data* data) {
	free(data);
}

int getIndex(Data* data) {
	return data->index;
}

int getValue(Data* data) {
	return data->value;
}

void printData(Data* data) {
	printf("[%d:%d]", data->index, data->value);
}
