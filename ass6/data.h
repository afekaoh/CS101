#ifndef HOMEWORK_DATA_H
#define HOMEWORK_DATA_H
typedef struct Data Data;

Data* initData();

void setData(Data*, int index, int value);

void destroyData(Data*);

int getIndex(Data*);

int getValue(Data*);

void printData(Data*);

#endif //HOMEWORK_DATA_H
