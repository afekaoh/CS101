#ifndef HOMEWORK_STUDENT_H
#define HOMEWORK_STUDENT_H
typedef struct Student Student;

Student* newStudent(const char* name, int age, float average);

void printStudent(Student* ps);

void freeStudent(Student* ps);

int ageComparator(Student* a, Student* b);

int nameComparator(Student* a, Student* b);

int equal(Student* s1, Student* s2);

char* getName(Student* s1);

#endif //HOMEWORK_STUDENT_H