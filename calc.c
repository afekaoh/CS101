#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define SIZE 21
const char DELIM[] = {'+', '-', '/', '*'};

int toDigit(char digit) {
	return digit - '0';
}

double toNumber(char const *string, int *flag) {
	int num = 0;
	while (*string) {
		if (!isdigit(*string)) {
			*flag = 0;
			return 0;
		}
		num *= 10;
		num += toDigit(*string);
		string++;
	}
	return num;
}

char getOperator(char *string) {
	char operator = 0;
	char *ptr = string;
	while (*ptr) {
		if (!isdigit(*ptr)) {
			operator = *ptr;
			break;
		}
		ptr++;
	}
	return operator;
}

void getNums(int i, double *nums, char *string, int *flag) {
	char *number = strtok(string, DELIM);
	while (number != NULL) {
		nums[i++] = toNumber(number, flag);
		number = strtok(NULL, DELIM);
	}
}

int main() {
	int i = 0;
	double sum = 0;
	double nums[2] = {};
	char operator = 0;
	char string[SIZE];
	
	scanf("%s", string);
	while (strcmp(string, "exit") != 0) {
		int flag = 1;
		operator = getOperator(string);
		getNums(i, nums, string, &flag);
		switch (operator) {
			case '+':
				sum = nums[0] + nums[1];
				break;
			case '-':
				sum = nums[0] - nums[1];
				break;
			case '*':
				sum = nums[0] * nums[1];
				break;
			case '/': {
				if (nums[1] == 0) {
					flag = 0;
					break;
				}
				sum = nums[0] / nums[1];
				break;
			}
			default:
				flag = 0;
		}
		flag ? printf("%g%c%g=%g\n", nums[0], operator, nums[1], sum) : puts("illegal");
		scanf("%s", string);
	}
	return 0;
}