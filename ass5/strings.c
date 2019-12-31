/*******************
Adam Shay Shapira
*316044809
*01
*ass05
*******************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/// enum declaration for the type of parentheses the function deal with
typedef enum {
	NONE, SIMPLE, SQUARE, CURLY, TRIANGLE
} parentheses;

/************************************************************************************************
* \input ch - char from the string
* \return if the char is an opening parenthesis the function returns its type and 0 if it's not
************************************************************************************************/
int whichOpen(char ch) {
	switch (ch) {
		case '(':
			return SIMPLE;
		case '[':
			return SQUARE;
		case '{':
			return CURLY;
		case '<':
			return TRIANGLE;
		default:
			return NONE;
	}
}

/***********************************************************************************************
* \input ch - char from the string
* \return if the char is an closing parenthesis the function returns its type and 0 if it's not
************************************************************************************************/
int whichClose(char ch) {
	switch (ch) {
		case ')':
			return SIMPLE;
		case ']':
			return SQUARE;
		case '}':
			return CURLY;
		case '>':
			return TRIANGLE;
		default:
			return NONE;
	}
}

/// the function gets 2 chars and checking if they are matching parentheses
int isMatch(char element, char top) {
	return whichOpen(top) == whichClose(element);
}

/***********************************************************************************
* \input str[] - a string
* \return 1 if it's legal and 0 if not
* \brief the functions gets a string and checks if its valid parentheses wise,
* 		meaning for every opening parenthesis there is a closing one in the revers
* 		order of appearance
************************************************************************************/
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