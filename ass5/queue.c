/*******************
Adam Shay Shapira
*316044809
*01
*ass05
*******************/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/************************************************************************************************
* \brief the function initialize the queue: allocating memory and setting up all it's variables
* and returns a pointer to the stuck or NULL if it couldn't initialize it
************************************************************************************************/
Queue *initQueue() {
	Queue *queue = malloc(sizeof(Queue));
	if (queue == NULL) {
		printf("heap memory error couldn't allocate memory in: %s, function: %s, line: %d\n", __FILE__, __func__,
		       __LINE__);
		return NULL;
	}
	queue->s1 = initStack();
	if (queue->s1 == NULL) {
		free(queue);
		return NULL;
	}
	queue->s2 = initStack();
	if (queue->s2 == NULL) {
		destroyStack(queue->s1);
		free(queue);
		return NULL;
	}
	return queue;
}

/*******************************************************************************************************
* \input *queue - a pointer to a queue struct
* \brief the functions freeing all the memory allocating to the queue and sets all the pointers to NULL
*******************************************************************************************************/
void destroyQueue(Queue *queue) {
	destroyStack(queue->s1);
	destroyStack(queue->s2);
	free(queue);
	queue = NULL;
}

/*******************************************************************************************************
* \input from - the stack that we emptying
* \input to - the stuck that we filling up
* \brief the function gets 2 stacks and moving all the elements from the first one to the second one
*******************************************************************************************************/
void spill(Stack *from, Stack *to) {
	while (lenOfStack(from) && (capacityOfStack(to) - lenOfStack(to) != 0))
		push(to, pop(from));
}

/***************************************************************************************************
* \input *queue - a pointer to a queue struct
* \input element - an element to add into the queue
* \brief the function gets an element and a queue to push it into and adds the element to the queue
***************************************************************************************************/
void enqueue(Queue *queue, Element element) {
	push(queue->s1, element);
}

/**************************************************************************
* \input: *queue - a pointer to a stack struct;
* \return: element - the element struct that we removed from the queue
* \brief: the function takes out an element from the queue and returns it.
**************************************************************************/
Element dequeue(Queue *queue) {
	if (isStackEmpty(queue->s2))
		spill(queue->s1, queue->s2);
	return pop(queue->s2);
}

/// returns the first element of the queue without changing the queue
Element peek(Queue *queue) {
	if (isStackEmpty(queue->s2))
		spill(queue->s1, queue->s2);
	return top(queue->s2);
}

///returns 1 if the queue is empty and 0 if not
int isQueueEmpty(Queue *queue) {
	return isStackEmpty(queue->s1) && isStackEmpty(queue->s2);
}

///returns the number of element is the queue
int lenOfQueue(Queue *queue) {
	return lenOfStack(queue->s1) + lenOfStack(queue->s2);
}

///returns the potential capacity of the queue
int capacityOfQueue(Queue *queue) {
	/*
	 * capacity is how much can you add to the queue which is capacity of s1 which you add elements in
	 * and how many elements there is in the queue which is in the capacity of s1 and the len of s2
	 */
	return capacityOfStack(queue->s1)+lenOfStack(queue->s2);
}

