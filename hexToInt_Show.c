#include <stdio.h>

int main(){
char a='a',b='b';
int hexToInt,hexA,hexB;
printf("the letter a is: %c the letter b is: %c\n",a,b);
printf("the ascii value of a is: %d and of b is: %d\n",a,b);
hexToInt=a-10;
hexA=a-a-10;
hexB=b-hexToInt;
printf("the diff between 'a' to a is: %d\n",hexToInt);
printf("the value of a in hex is: %d and the value of b is: %d\n",a-(a-10),b-(a-10));

    return 0;
}