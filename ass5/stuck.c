#include "stuck.h"
#include "element.h"
#include <stdlib.h>
#include <stdio.h>

Stack *initStack() {
	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL)
		return NULL;
	stack->size = 0;
	stack->topIndex = -1;
	stack->content = (Element *) malloc(stack->size * sizeof(Element *));
	if (stack->content == NULL) {
		free(stack);
		return NULL;
	}
	return stack;
}

void destroy(Stack *stack) {
	free(stack->content);
	free(stack);
}

void push(Stack *stack, Element element) {
	Element *temp = realloc(stack->content, ++stack->size * sizeof(Element *));
	if (temp == NULL) {
		printf("something went wrong");
		destroy(stack);
		exit(42);
	}
	stack->content = temp;
	stack->content[++stack->topIndex] = element;
}

Element pop(Stack *stack) {
	
	Element tempElement = stack->content[stack->topIndex--];
	Element *temp = realloc(stack->content, --stack->size * sizeof(Element *));
	if (temp == NULL) {
		printf("something went wrong");
		destroy(stack);
		exit(42);
	}
	return tempElement;
}

Element top(Stack *stack) {
	return stack->content[stack->topIndex];
}

int isEmpty(Stack *stack) {
	if (getCapacity(stack) == stack->size)
		return 1;
	return 0;
}

int isFull(Stack *stack) {
	if (getCapacity(stack) == 0)
		return 1;
	return 0;
}

int getCapacity(Stack *stack) {
	return stack->size - (getNumOfElements(stack));
}

int getNumOfElements(Stack *stack) {
	return stack->topIndex + 1;
}

void printStack(Stack *stack) {
	int size = getNumOfElements(stack);
	if (isEmpty(stack)) {
		printf("Empty");
		return;
	}
	for (int i = size - 1; i >= 0; i--) {
		printf("%d. ", size - i);
		printElement(stack->content[i]);
	}
}