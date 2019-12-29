#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *initStack() {
	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL)
		return NULL;
	stack->size = 1;
	stack->topIndex = -1;
	stack->content = malloc(stack->size * sizeof(Element));
	if (stack->content == NULL) {
		free(stack);
		return NULL;
	}
	return stack;
}

void destroyStack(Stack *stack) {
	free(stack->content);
	stack->content = NULL;
	free(stack);
	stack = NULL;
}

void push(Stack *stack, Element element) {
	stack->content[++stack->topIndex] = element;
	
	if (lenOfStack(stack) > capacityOfStack(stack) / 2) {
		stack->size *= 2;
		Element *temp = realloc(stack->content, stack->size * sizeof(Element));
		if (temp == NULL) {
			printf("something went wrong in %s in line %d", __FILE__, __LINE__);
			destroyStack(stack);
			exit(42);
		}
		stack->content = temp;
	}
}

Element pop(Stack *stack) {
	Element tempElement = stack->content[stack->topIndex--];
	if (lenOfStack(stack) < capacityOfStack(stack) / 2) {
		stack->size = (stack->size / 2) + (stack->size % 2);
		Element *temp = realloc(stack->content, stack->size * sizeof(Element));
		if (temp == NULL) {
			printf("something went wrong in %s in line %d", __FILE__, __LINE__);
			destroyStack(stack);
			exit(42);
		}
	}
	return tempElement;
}

Element top(Stack *stack) {
	return stack->content[stack->topIndex];
}

int isStackEmpty(Stack *stack) {
	return lenOfStack(stack) == 0 ? 1 : 0;
}

int capacityOfStack(Stack *stack) {
	return stack->size;
}

int lenOfStack(Stack *stack) {
	return stack->topIndex + 1;
}

void printStack(Stack *stack) {
	if (isStackEmpty(stack)) {
		printf("Empty\n");
		return;
	}
	for (int i = stack->topIndex; i >= 0; i--) {
		printf("%d: ", lenOfStack(stack) - i);
		printElement(stack->content[i]);
		printf("\n");
	}
}