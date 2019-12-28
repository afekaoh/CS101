#include "element.h"
#include <stdio.h>

void printElement(Element e) {
	printf("%c\n", e.value);
}

void scanElement(Element *e) {
	scanf(" %c", &e->value);
}
