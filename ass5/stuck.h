#ifndef __STUCK_H
#define __STUCK_H

#include "element.h"

typedef struct {
	Element *content;
	int size;
	int topIndex;
} Stack;

Stack *initStack();

void destroy(Stack *stack);

void push(Stack *stack, Element element);

Element pop(Stack *stack);

Element top(Stack *stack);

int isEmpty(Stack *stack);

int isFull(Stack *stack);

int getCapacity(Stack *stack);

int getNumOfElements(Stack *stack);

void printStack(Stack *stack);

#endif
