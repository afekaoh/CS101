#include <math.h>
#include <stdio.h>
int counter;

double iterPow(double base, int power){
    const double ONE=1.00;
    counter=0;
    //is negative to know when the power is negative
    int isNeg=0;
    double result=1.00;
    if (power == 0)
        return 1;
    if(power == 1)
        return base;
    if(power<0&&base==0)
        return infinity();
    if(base==0)
        return 0;
    if(power<0){
        power=power*-1;
        isNeg=1;
    }
    for (int i = 0; i <power ; ++i) {
        counter++;
        result*=base;

    }
    if(isNeg)
        return (ONE / result);
    return result;
}

int main(){
    int y;
    double x=1;
    for (int i = -5; i <6 ; ++i) {
        x=1.5;
        printf("the base is:%f, the exp is:%d, the result is:%f, and the counter is:%d\n",x,i,iterPow(x,i),counter);
    }



}