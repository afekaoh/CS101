#ifndef __ELEMENT_H
#define __ELEMENT_H

typedef struct {
	char value;
} Element;

void printElement(Element e);

void scanElement(Element *e);

#endif