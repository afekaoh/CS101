#ifndef HOMEWORK_DATA_H
#define HOMEWORK_DATA_H


typedef struct Data Data;

Data* initData(int key, int value);

void destroyData(Data*);

int getValue(Data*);

void printData(Data*);

int comperData(Data*, Data*);

#endif //HOMEWORK_DATA_H
