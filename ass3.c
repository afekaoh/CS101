#include <math.h>
#include <stdio.h>
int counter;

double iterPow(double base, int power){
    const double ONE=1.00;
    counter=1;
    //is negative to know when the power is negative
    int isNeg=0;
    double result=1.00;
   //edge cases
    if (power == 0)
        return 1;
    if(power == 1)
        return base;
    if(power<0&&base==0)
        return infinity();
    if(base==0)
        return 0;
    //x to the power of negative number y is 1/(x^-y)
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

double recPow(double base, int power){
    static int countRec=1;
    const double ONE=1;
    int isNeg=0;
    double result=1;
    if(power==0)
        return 1;
    if(power==1||base==0)
        return base;
    if(power<0&&base==0)
        return infinity();
    if (power<0){
        power*=-1;
        isNeg=1;
    }
    countRec++;
    result=base*recPow(base,power-1);
    counter=countRec;
    countRec=1;
    if(isNeg)
        return (ONE/result);
    return result;

}

double recEffiPow(double base,int power){
    static int countRec=1;
    const double ONE=1;
    int isNeg=0;
    double result=1;
    if(power==0)
        return 1;
    if(power==1||base==0)
        return base;
    if(power<0&&base==0)
        return infinity();
    if (power<0){
        power*=-1;
        isNeg=1;
    }
    countRec++;
    

}
int main(){
    int y;
    double x,result;
    for (int i = -5; i <6 ; ++i) {
        x=0;
        result=iterPow(x,i);
        printf("the base is:%f, the exp is:%d, the result is:%f, and the counter is:%d\n",x,i,result,counter);
        result=recPow(x,i);
        printf("the base is:%f, the exp is:%d, the result is:%f, and the counter is:%d\n",x,i,result,counter);
        printf("\n");

    }



}