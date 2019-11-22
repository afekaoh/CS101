#include <stdio.h>

int main(){
char a='a',b='b';
int hexToInt;
printf("the letter a is: %c the letter b is: %c\n",a,b);
printf("the ascii value of a is: %d and of b is: %d\n",a,b);
hexToInt=a-10;
a-=hexToInt;
b-=hexToInt;
printf("the value of a in hex is: %d and the value of b is: %d\n",a,b);

    return 0;
}