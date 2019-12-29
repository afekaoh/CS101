#include <stdio.h>
#include <string.h>
#include "headers/stuck.h"
#include "headers/ass5.h"

void manageStack() {
	Stack *stack = initStack();
	if (stack == NULL) {
		printf("Sorry, not today\n");
		return;
	}
	printf("Welcome to the elements stack!\n");
	int input;
	do {
		Element element;
		printf("Please choose option from the menu:\n\t1 : push\n\t2 : top\n\t3 : pop\n\t4 : print stack\n\tother : exit\n");
		scanf("%d", &input);
		if (input == 1) {
			printf("Enter element to push: ");
			scanElement(&element);
			push(stack, element);
		} else if (input == 2) {
			if (!isEmpty(stack)) {
				printf("top\n");
				printElement(top(stack));
				printf("\n");
			} else {
				printf("can't top! stack is empty!\n");
			}
		} else if (input == 3) {
			if (!isEmpty(stack)) {
				printf("pop\n");
				printElement(pop(stack));
				printf("\n");
			} else {
				printf("can't pop! stack is empty!\n");
			}
		} else if (input == 4) {
			printStack(stack);
		}
	} while (1 <= input && input <= 4);
	destroy(stack);
}

void manageStrings() {
	char *strings[] = {
			"(87234872364)",
			"{}",
			"{999967(})",
			"{{{[[[((()))]]]}}}",
			"[[[]]]((([][{{()}}])))[]"
	};
	int testLen = sizeof(strings) / sizeof(char *);
	for (int i = 0; i < testLen; i++) {
		char *str = strings[i];
		printf("%s\n", isLegalString(str) ? "Legal" : "Illegal");
	}
}

int main() {
	manageStrings();
//	manageStack();
	return 0;
}
