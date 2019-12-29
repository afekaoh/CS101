#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


typedef enum {
	SIMPLE = 1,
	SQUARE, CURLY, TRIANGLE
} parentheses;


int whichOpen(char parentheses) {
	switch (parentheses) {
		case '(':
			return SIMPLE;
		case '[':
			return SQUARE;
		case '{':
			return CURLY;
		case '<':
			return TRIANGLE;
		default:
			return 0;
	}
}

int whichClose(char parentheses) {
	switch (parentheses) {
		case ')':
			return SIMPLE;
		case ']':
			return SQUARE;
		case '}':
			return CURLY;
		case '>':
			return TRIANGLE;
		default:
			return 0;
	}
}

int isMatch(char element, char top) {
	return whichOpen(top) == whichClose(element);
}

int isLegalString(char str[]) {
	int legal = 1;
	Stack *stack = initStack();
	if (stack == NULL) {
		printf("Sorry, not today\n");
		exit(42);
	}
	Element e;
	while (*str && legal) {
		e.c = *str;
		if (whichOpen(e.c))
			push(stack, e);
		if (whichClose(e.c))
			legal = isStackEmpty(stack) ? 0 : isMatch(e.c, pop(stack).c);
		str++;
	}
	if (legal)
		legal = isStackEmpty(stack);
	destroyStack(stack);
	return legal;
}