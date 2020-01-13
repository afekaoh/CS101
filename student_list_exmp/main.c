#define SIZE 10

#include "StudentList.h"
#include "StudentItem.h"
#include "student.h"
#include <stdio.h>


int main() {
	char str[3];
	str[0] = 'S';
	str[2] = '\0';
	StudentList* list = newStudentList();
	for (int i = 0; i < SIZE; i++) {
		str[1] = (char) ('0' + i);
		add(list, newStudent(str, 20 + i % 5, 100 - (float) i));
	}
	for_each(list, printStudent);
	printf("\n");
	Student* toRemove = newStudent("S7", 22, (float) 93.0);
	removeStudent(list, toRemove);
	freeStudent(toRemove);
	printf("\n");
	printStudentList(list);
	bubbleSort(list, ageComparator);
	printStudentList(list);
	bubbleSort(list, nameComparator);
	printStudentList(list);
	freeStudentList(list);
	return 0;
}