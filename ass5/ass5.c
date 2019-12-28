#include <string.h>
#include "stuck.h"

char const OPEN_PARENTHESES[] = "([{";
char const CLOSE_PARENTHESES[] = ")]}";

char matchParentheses(char element, char top) {
	switch (element) {
		case '}' :
			if (top == '{')
				return 1;
		case ']' :
			if (top == '[')
				return 1;
		case ')' :
			if (top == '(')
				return 1;
		default:
			return 0;
	}
}

int isLegalString(char const str[]) {
	Stack *stack = initStack();
	Element element;
	while (*str) {
		element.value = *str;
		if (strchr(OPEN_PARENTHESES, element.value) != NULL) {
			if (isEmpty(stack))
				push(stack, element);
			else {
				if (element.value <= top(stack).value)
					push(stack, element);
				else
					return 0;
			}
		}
		if (strchr(CLOSE_PARENTHESES, element.value) != NULL) {
			if (!matchParentheses(element.value, pop(stack).value))
				return 0;
		}
		str++;
	}
	int flag = isEmpty(stack);
	destroy(stack);
	return flag;
}