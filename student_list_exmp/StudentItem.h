#ifndef HOMEWORK_STUDENTITEM_H
#define HOMEWORK_STUDENTITEM_H

#include "student.h"

typedef struct StudentItem StudentItem;

StudentItem* createStudentItem(Student*);

void freeStudentItem(StudentItem* item);

void switchStudentItems(StudentItem* a, StudentItem* b);

void setNext(StudentItem* std1, StudentItem* std2);

void setData(StudentItem* std1, Student* std2);

StudentItem* getNext(StudentItem* std1);

Student* getData(StudentItem* std1);

#endif //HOMEWORK_STUDENTITEM_H
