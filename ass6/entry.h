#ifndef HOMEWORK_ENTRY_H
#define HOMEWORK_ENTRY_H


typedef struct Entry Entry;

void* initEntry(int key, int value);

void destroyEntry(void* entry);

int getValue(void* entry);

void printEntry(void* entry);

int comperEntry(void* e1, void* e2);

#endif //HOMEWORK_ENTRY_H
