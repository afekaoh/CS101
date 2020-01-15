#ifndef HOMEWORK_ENTRY_H
#define HOMEWORK_ENTRY_H


typedef struct Entry Entry;

Entry* initEntry(int key, int value);

void destroyEntry(Entry*);

int getValue(Entry*);

void printEntry(Entry*);

int comperEntry(Entry*, Entry*);

#endif //HOMEWORK_ENTRY_H
