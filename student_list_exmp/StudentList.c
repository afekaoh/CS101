#include "StudentList.h"
#include "StudentItem.h"
#include "student.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct StudentList {
	StudentItem *head, *last;
};

void freeStudentList(StudentList *list) {
	freeStudentItem(list->head); // cool :)
	free(list);
}

StudentList *newStudentList() {
	StudentList *sl = (StudentList *) malloc(sizeof(StudentList));
	sl->head = NULL;
	sl->last = sl->head; // in our case equivalent to sl->last = NULL
	return sl;
}


void add(StudentList *list, Student *newStudent) {
	StudentItem *item = createStudentItem(newStudent);
	if (list->head == NULL) {
		list->head = item;
		list->last = item;
	} else {
		setNext(list->last, item);
		list->last = item;
	}
}

void *newIterator(StudentList *list) {
	return list->head;
}

void printStudentList(StudentList *list) {
	for_each(list, printStudent);
	printf("\n");
}

void for_each(StudentList *list, Apply apply) {
	StudentItem *iterator = newIterator(list);
	while (iterator != NULL) {
		apply(getData(iterator));
		iterator = getNext(iterator);
	}
}

void removeStudent(StudentList *list, Student *student) {
	StudentItem *iterator = newIterator(list);
	StudentItem *previous = NULL;
// if this is the first item
	if (list->head && equal(getData(list->head), student)) {
		list->head = getNext(iterator);
		if (list->head == NULL) list->last = list->head;
		setNext(iterator, NULL); // why? :)
		freeStudentItem(iterator);
		return;
	}
	while (iterator != NULL && !equal(getData(iterator), student)) {
		previous = iterator;
		iterator = getNext(iterator);
	}
	
	if (iterator == NULL)
		return;
	
	else if (equal(getData(iterator), student)) {
		setNext(previous, getNext(iterator));
		setNext(iterator, NULL);
		if (iterator == list->last)
			list->last = previous;
		freeStudentItem(iterator);
	}
}

void bubbleSort(StudentList *list, Comparator compare) {
	StudentItem *i;
	int switched = 1;
	while (switched) {
		switched = 0;
		i = newIterator(list);
		while (getNext(i) != NULL) {
			if (compare(getData(i), getData(getNext(i))) > 0) {
				switched = 1;
				switchStudentItems(i, getNext(i));
			}
			i = getNext(i);
		}
	}
}