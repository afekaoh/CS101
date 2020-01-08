#ifndef HOMEWORK_STUDENTLIST_H
#define HOMEWORK_STUDENTLIST_H

#include "student.h"
#include "StudentItem.h"

typedef struct StudentList StudentList;


typedef int (*Comparator)(Student *, Student *);

typedef void (*Apply)();

void for_each(StudentList *list, Apply);

void freeStudentList(StudentList *list);

StudentList *newStudentList();

void add(StudentList *list, Student *newStudent);

StudentItem *newIterator(StudentList *list);

void printStudentList(StudentList *list);

void removeStudent(StudentList *list, Student *student);

void bubbleSort(StudentList *list, Comparator);

#endif //HOMEWORK_STUDENTLIST_H
