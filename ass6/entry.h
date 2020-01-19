#ifndef HOMEWORK_ENTRY_H
#define HOMEWORK_ENTRY_H
/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass06
*******************/

typedef struct Entry Entry;

/**
* \input int key - the key of the entry
* \input int value - the value of the entry
* \return the functions initialize a new entry allocate memory to him and return a pointer to him
*/
void* initEntry(int key, int value);

///freeing the allocated memory
void destroyEntry(void* entry);

///the functions returns the value of the given entry
int getValue(void* entry);

///the function prints the entry in a specific format of "[key:value]"
void printEntry(void* entry);

/**
* \input e1 - pointer to entry
* \unput e2 - pointer to entry
* \return  int (1,0,-1)
* \brief the function compares two entries by key and return if e1 is bigger then e2 smaller or equal
*/
int compareEntry(void* entry1, void* entry2);

#endif //HOMEWORK_ENTRY_H
