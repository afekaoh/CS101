#include <math.h>
#include <stdio.h>
#include "ass3.h"

int counter;

double iterPow(double base, int power) {
    const double ONE = 1.00;
    //is negative to know when the power is negative
    int isNeg = 0;
    double result = 1.00;
    //edge cases
    if (power == 0)
        return 1;
    if (power == 1)
        return base;
    if(power<0&&base==0)
        return (double)1/0;
    //x to the power of negative number y is 1/(x^-y)
    if (power < 0) {
        power = power * -1;
        isNeg = 1;
    }
    for (int i = 0; i < power; ++i) {
        counter++;
        result *= base;

    }
    if (isNeg)
        return (ONE / result);
    return result;
}

double recPow(double base, int power) {
    const double ONE = 1;
    int isNeg = 0;
    double result;
    if (power == 0)
        return 1;
   if((power<0) && (base==0))
       return (double)1/0;
    if (power == 1)
        return base;

    if (power < 0) {
        power *= -1;
        isNeg = 1;
    }
    counter++;
    result = base * recPow(base, power - 1);
    if (isNeg)
        return (ONE / result);
    return result;

}

double recEffiPow(double base, int power) {
    const double ONE = 1;
    int isNeg = 0;
    double isOdd = 0;
    double result = 1;
    if (power == 0)
        return 1;
    if(power<0&&base==0)
        return (double)1/0;
    if (power == 1)
        return base;
    if (power < 0) {
        power *= -1;
        isNeg = 1;
    }
    if (power % 2) {
        isOdd = base;
        power--;
    } else
        isOdd = 1;
    counter++;
    result = recEffiPow(base, power / 2);
    if (isNeg)
        return (ONE / (isOdd * result * result));
    return (isOdd * result * result);


}


int maxRow(int arr[], int size) {
    int max = 0;
    for (int j = 0; j < size; ++j) {
        max = max > arr[j] ? max : arr[j];
    }

    return max;
}

void printRow(int arr[], int size, int max) {

    //the log will return number of digit -1
    int setSize = (int) log10(max) + 1;
    for (int i = 0; i < size; ++i) {
        printf("%0*d", setSize, arr[i]);
        if (i != size - 1)
            printf(" ");
    }
    printf("\n");
}

void printArr2D(int arr[][MAX], int row, int column) {
    int maxInRow[row], max;
    for (int i = 0; i < row; ++i) {
        maxInRow[i] = maxRow(arr[i], column);
    }
    max = maxRow(maxInRow, row);

    for (int i = 0; i < row; ++i) {
        printRow(arr[i], column, max);
    }

}
void swap(int arr[],int i,int j){
    int temp=arr[i];
    arr[i]=arr[j];
    arr[j]=temp;


}
void sort(int arr[], int size) {
    int isSwap=0;
    do{
        isSwap=0;
    for (int i=0; i < size-1 ; i++) {
        if (arr[i] > arr[i + 1]) {
            swap(arr, i, i + 1);
            isSwap = 1;
        }
    }
    }while(isSwap);
}


int isPermutation(int arr1[], int size1, int arr2[], int size2) {
    int sortedArr1[size1], sortedArr2[size2],isEqule=0;
    if(size1!=size2||size1==0||size2==0)
        return 0;

    for (int i = 0; i < size1 ; i++) {
            sortedArr1[i] = arr1[i];
            sortedArr2[i] = arr2[i];
    }
   sort(sortedArr1, size1);
   sort(sortedArr2, size2);
    for (int i = 0; i < size1; i++) {
    if(sortedArr1[i]==sortedArr2[i])
        isEqule++;
    }
    isEqule = isEqule == size1 ? 1 : 0;
return isEqule;
}
