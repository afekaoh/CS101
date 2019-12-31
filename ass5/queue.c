/*******************
Adam Shay Shapira
*316044809
*01
*ass05
*******************/

#include "queue.h"
#include <stdlib.h>

/************************************************************************************************
* \brief the function initialize the queue: allocating memory and setting up all it's variables
* and returns a pointer to the stuck or NULL if it couldn't initialize it
************************************************************************************************/
Queue *initQueue() {
	Queue *queue = malloc(sizeof(Queue));
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
	while (lenOfStack(from))
		push(to, pop(from));
}

/***************************************************************************************************
* \input *queue - a pointer to a queue struct
* \input element - an element to add into the queue
* \brief the function gets an element and a queue to push it into and adds the element to the queue
***************************************************************************************************/
void enqueue(Queue *queue, Element element) {
	if (isQueueEmpty(queue))
		push(queue->s1, element);
	else {
		spill(queue->s1, queue->s2);
		push(queue->s2, element);
		spill(queue->s2, queue->s1);
	}
}

/**************************************************************************
* \input: *queue - a pointer to a stack struct;
* \return: element - the element struct that we removed from the queue
* \brief: the function takes out an element from the queue and returns it.
**************************************************************************/
Element dequeue(Queue *queue) {
	return pop(queue->s1);
}

/// returns the first element of the queue without changing the queue
Element peek(Queue *queue) {
	return top(queue->s1);
}

///returns 1 if the queue is empty and 0 if not
int isQueueEmpty(Queue *queue) {
	return isStackEmpty(queue->s1);
}

///returns the number of element is the queue
int lenOfQueue(Queue *queue) {
	return lenOfStack(queue->s1);
}

///returns the potential capacity of the queue
int capacityOfQueue(Queue *queue) {
	return capacityOfStack(queue->s1);
}

