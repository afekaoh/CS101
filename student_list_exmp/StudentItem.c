#include "StudentItem.h"
#include "student.h"
#include <stdlib.h>
#include <stdio.h>

struct StudentItem {
	Student* data;
	struct StudentItem* next;
};

StudentItem* createStudentItem(Student* newStudent) {
	StudentItem* std = (StudentItem*) malloc(sizeof(StudentItem));
	if (std == NULL)
		return NULL;
	std->data = newStudent;
	std->next = NULL;
	return std;
}

void setNext(StudentItem* std1, StudentItem* std2) {
	std1->next = std2;
}

void setData(StudentItem* std1, Student* std2) {
	std1->data = (Student*) std2;
}

StudentItem* getNext(StudentItem* std1) {
	return std1->next;
}

Student* getData(StudentItem* std1) {
	return std1->data;
}


// how it works? :)
void freeStudentItem(StudentItem* item) {
	if (item != NULL) {
		if (item->data != NULL) {
			printf("freeing %s\n", getName(item->data));// for debug
		}
		freeStudent(item->data);
		freeStudentItem(item->next);
		free(item);
	}
}

void switchStudentItems(StudentItem* a, StudentItem* b) {
	Student* temp = a->data;
	a->data = b->data;
	b->data = temp;
}