/*******************
Adam Shay Shapira
*316044809
*01
*ass05
*******************/

#include <string.h>
#include <stdio.h>
#include "stack.h"


/// enum declaration for the type of brackets
enum BRACKETS {
	NONE, SIMPLE, SQUARE, CURLY, TRIANGLE
};

/****************************************************************************************
* \input ch - char
* \return if the char is a right bracket the function returns its type and 0 if it's not
****************************************************************************************/
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

/****************************************************************************************
* \input ch - char
* \return if the char is a left bracket the function returns its type and 0 if it's not
****************************************************************************************/
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

/// the function gets 2 chars and checking if they are matching brackets
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
	int isLegal = 1;
	Stack *stack = initStack();
	if (stack == NULL) {
		printf("there was a problem couldn't check for legality\n");
		isLegal = 0;
	}
	Element e;
	while (*str && isLegal) {
		e.c = *str;
		if (whichOpen(e.c))
			push(stack, e);
		if (whichClose(e.c))
			isLegal = isStackEmpty(stack) ? 0 : isMatch(e.c, pop(stack).c);
		str++;
	}
	if (isLegal)
		//the stack should be empty now for legal strings
		isLegal = isStackEmpty(stack);
	//destroyStack(stack);
	return isLegal;
}