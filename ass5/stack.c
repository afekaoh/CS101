/*******************
Adam Shay Shapira
*316044809
*01
*ass05
*******************/

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/***********************************************************************************************
* \brief the function initialize the stack: allocating memory and setting up all it's variables
* and returns a pointer to the stuck or NULL if it couldn't initialize it
***********************************************************************************************/
Stack *initStack() {
	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL) {
		printf("heap memory error couldn't allocate memory in: %s, function: %s, line: %d\n", __FILE__, __func__,
		       __LINE__);
		return NULL;
	}
	stack->size = 1;
	stack->topIndex = -1;
	stack->content = malloc(stack->size * sizeof(Element));
	if (stack->content == NULL) {
		printf("heap memory error couldn't allocate memory in: %s, function: %s, line: %d\n", __FILE__, __func__,
		       __LINE__);
		destroyStack(stack);
	}
	return stack;
}

/***************************************************************************************************
* \input *stack - a pointer to a stack struct
* \brief the functions freeing all the memory allocating to the stack and sets the pointers to NULL
***************************************************************************************************/
void destroyStack(Stack *stack) {
	if (stack != NULL) {
		free(stack->content);
		stack->content = NULL;
		free(stack);
		stack = NULL;
	}
}

///returns how mach empty space there is in the stack
int emptySpace(Stack *stack) {
	return stack->size - lenOfStack(stack);
}

/************************************************************************************************************
* \input *stack - a pointer to a stack struct
* \input element - an element to push into the stack
* \brief the function gets an element and a stack to push it into, the function add the element to the stack
* 		 and increase the size of the stack in case we filled the stack by doubling its capacity
************************************************************************************************************/
void push(Stack *stack, Element element) {
	stack->content[++stack->topIndex] = element;
	
	if (emptySpace(stack) == 0) {
		stack->size *= 2;
		Element *temp = realloc(stack->content, stack->size * sizeof(Element));
		if (temp == NULL) {
			printf("heap memory error couldn't allocate memory in: %s, function: %s, line: %d\n", __FILE__, __func__,
			       __LINE__);
			//returning the size to it's real value
			stack->size /= 2;
		} else
			stack->content = temp;
	}
}

/************************************************************************************************
* \input: *stack - a pointer to a stack struct;
* \return: element - the element struct that we popped from stack
* \brief: the function takes out an element from the stack and returns it.
* 		if after the removal we have too much empty space (more then half of the space is empty)
* 		the function decrease the capacity of the stack by halving it
************************************************************************************************/
Element pop(Stack *stack) {
	Element popElement = stack->content[stack->topIndex--];
	
	if (emptySpace(stack) > (stack->size / 2)) {
		stack->size /= 2;
		// leaving just 1 empty spot in the stack
		assert(stack->size != 0);
		Element *temp = realloc(stack->content, stack->size * sizeof(Element));
		if (temp == NULL) {
			printf("heap memory error couldn't allocate memory in: %s, function: %s, line: %d\n", __FILE__, __func__,
			       __LINE__);
			//returning the stack to it's real value
			stack->size *= 2;
		} else
			stack->content = temp;
	}
	return popElement;
}

/// returns the top element of the stack without changing the stack
Element top(Stack *stack) {
	return stack->content[stack->topIndex];
}

int isStackEmpty(Stack *stack) {
	//if len of stack is 0 return 1 and 0 otherwise
	return !lenOfStack(stack);
}

///returns how much memory is allocated to the content array
int capacityOfStack(Stack *stack) {
	return stack->size;
}

///returns the number of element is the stack by checking where top index is pointing
int lenOfStack(Stack *stack) {
	return stack->topIndex + 1;
}

///prints the stack in LIFO order element by element
void printStack(Stack *stack) {
	for (int i = stack->topIndex; i >= 0; i--) {
		printf("%d: ", i + 1);
		printElement(stack->content[i]);
		printf("\n");
	}
}