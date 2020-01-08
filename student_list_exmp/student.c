#include "student.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Student {
	char *name;
	int age;
	float average;
};

int ageComparator(Student *a, Student *b) {
	return a->age - b->age;
}

int nameComparator(Student *a, Student *b) {
	return strcmp(a->name, b->name);
}

int equal(Student *s1, Student *s2) {
// strcmp is the last check since it is O(N)...
	return s1->age == s2->age && s1->average == s2->average && strcmp(s1->name, s2->name) == 0 ? 1 : 0;
}

char *getName(Student *s) {
	return s->name;
}

Student *newStudent(const char *name, int age, float average) {
	Student *std = (Student *) malloc(sizeof(Student));
	if (std == NULL) {
		exit(42);
	}
	std->name = (char *) malloc(strlen(name) + 1);
	if (std->name == NULL) {
		free(std);
		exit(42);
	}
	strcpy(std->name, name);
	std->age = age;
	std->average = average;
	return std;
}

void printStudent(Student *ps) {
	printf("%s %d %.2f\n", ps->name, ps->age, ps->average);
}

void freeStudent(Student *ps) {
	if (ps != NULL) {
		free(ps->name);
		free(ps);
	}
}



