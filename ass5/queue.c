#include "queue.h"
#include <stdlib.h>

Queue *initQueue() {
	Queue *queue = malloc(sizeof(Queue));
	queue->s1 = initStack();
	if (queue->s1 == NULL) {
		free(queue);
		return NULL;
	}
	queue->s2 = initStack();
	if (queue->s2 == NULL) {
		free(queue->s1);
		free(queue);
		return NULL;
	}
	return queue;
}

void destroyQueue(Queue *queue) {
	free(queue->s1);
	free(queue->s2);
	free(queue);
	queue->s1 = NULL;
	queue->s2 = NULL;
	queue = NULL;
}

void spill(Stack *from, Stack *to) {
	while (lenOfStack(from))
		push(to, pop(from));
}


void enqueue(Queue *queue, Element element) {
	if (isQueueEmpty(queue))
		push(queue->s1, element);
	else {
		spill(queue->s1, queue->s2);
		push(queue->s2, element);
		spill(queue->s2, queue->s1);
	}
}


Element dequeue(Queue *queue) {
	return pop(queue->s1);
}

Element peek(Queue *queue) {
	return top(queue->s1);
}

int isQueueEmpty(Queue *queue) {
	return isStackEmpty(queue->s1);
}

int lenOfQueue(Queue *queue) {
	return lenOfStack(queue->s1);
}

int capacityOfQueue(Queue *queue) {
	return capacityOfStack(queue->s1);
}

