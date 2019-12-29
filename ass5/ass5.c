#include <string.h>
#include "headers/stuck.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	SIMPLE = 1,
	SQUARE, CURLY
} parentheses;

int whichOpen(char parentheses) {
	switch (parentheses) {
		case '(':
			return SIMPLE;
		case '[':
			return SQUARE;
		case '{':
			return CURLY;
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
		default:
			return 0;
	}
}

int isMatch(char element, char top) {
	return whichOpen(top) == whichClose(element);
}

int isLegalString(char const str[]) {
	int legal = 1;
	Stack *stack = initStack();
	if (stack == NULL) {
		printf("Sorry, not today\n");
		exit(42);
	}
	Element e;
	while (*str && legal) {
		e.value = *str;
		if (whichOpen(e.value))
			push(stack, e);
		if (whichClose(e.value))
			legal = isEmpty(stack) ? 0 : isMatch(e.value, pop(stack).value);
		str++;
	}
	if (legal)
		legal = isEmpty(stack);
	destroy(stack);
	return legal;
}