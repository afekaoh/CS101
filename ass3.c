/*******************
* Adam Shay Shapira
* 316044809
* 01
* ass03
********************/
#include <math.h>
#include <stdio.h>

int counter;

double iterPow(double base, int power) {
    /*******************************************************************************
    * Function name: iterPow
    * Input: double base (valid value every number) - number to raise to the power
    *        int power (valid input every integer) - the exponent
    * Output: double (value every number)
    * Function Operation: the function gets a base and an exponent and return the value
    *                     of the base to the power of power in an iterative way
    *******************************************************************************/
    //constant that equals to infinity by the definition of IEEE
    const double inf = 1.00 / 0.00;
    //is negative to know when the power is negative
    int isNeg = 0;
    double result = 1.00;
    //edge cases
    if (power == 0)
        return 1;
    // 0^-y y>0 is infinity
    if (power < 0 && base == 0)
        return inf;
    //x^1==x and 0^x=0 (x>0)
    if (power == 1 || base == 0)
        return base;
    if (power < 0) {
        power = power * -1;
        isNeg = 1;
    }
    for (int i = 0; i < power; ++i) {
        counter++;
        result *= base;
    }
    if (isNeg)
        //x to the power of negative number y is 1/(x^-y)
        return (1.00 / result);
    return result;
}

double recPow(double base, int power) {
    /*******************************************************************************
    * Function name: recPow
    * Input: double base (valid value every number) - number to raise to the power
    *        int power (valid input every integer) - the exponent
    * Output: double (value every number)
    * Function Operation: the function gets a base and an exponent and return the value
    *                     of the base to the power of power in an naive recursive way
    *                     of x^y=x*x^(y-1)
    *******************************************************************************/
    const double inf = 1.00 / 0.00;
    int isNeg = 0;
    double result;
    if (power == 0)
        return 1;
    if ((power < 0) && (base == 0))
        return inf;
    if (power == 1 || base == 0)
        return base;

    if (power < 0) {
        power *= -1;
        isNeg = 1;
    }
    counter++;
    result = base * recPow(base, power - 1);
    if (isNeg)
        return (1.00 / result);
    return result;

}

double recEffiPow(double base, int power) {
    /*******************************************************************************
    * Function name: recPow
    * Input: double base (valid value every number) - number to raise to the power
    *        int power (valid input every integer) - the exponent
    * Output: double (value every number)
    * Function Operation: the function gets a base and an exponent and return the val
    *                     of the base to the power of power in an efficient recursive way
    *                     of x^y=x^(y/2)*x^(y/2)
    *******************************************************************************/
    const double inf = 1.00 / 0.00;
    int isNeg = 0;
    double isOdd = 0;
    double result;
    if (power == 0)
        return 1;
    if (power < 0 && base == 0)
        return inf;
    if (power == 1 || base == 0)
        return base;
    if (power < 0) {
        power *= -1;
        isNeg = 1;
    }
    //if y is odd return
    if (power % 2) {
        //putting one x to the side to make the power even
        isOdd = base;
        power--;
    } else
        isOdd = 1;
    counter++;
    //computing x^(y/2)
    result = recEffiPow(base, power / 2);
    if (isNeg)
        return (1.00 / (isOdd * result * result));
    //computing x^(y/2)*x^(y/2) and in the case of odd y multiply in the x we put aside
    return (isOdd * result * result);


}


int maxRow(int arr[], int size) {
    /*******************************************************************************
    * Function name: maxRow
    * Input: int arr[] an array of integers
    *        int size the size of the array
    * Output: integer
    * Function Operation: the function gets an 1D array and returning the the biggest number
    *                     in the array
    *******************************************************************************/
    int max = 0;
    for (int i = 0; i < size; ++i) {
        max = max > arr[i] ? max : arr[i];
    }

    return max;
}

void printRow(int arr[], int size, int max) {
    /*******************************************************************************
    * Function name: printRow
    * Input: int arr[] an array of integers
    *        int size the size of the array
    *        int max the biggest number of the array
    * Output: none
    * Function Operation: the function gets a 1D array and returning prints the array
    *                     in an evenly distributed way by adding a zeros to make all the
    *                     numbers the same size by the factor of max
    *******************************************************************************/
    //the log will return number of digit -1
    int setSize = (int) log10(max) + 1;
    for (int i = 0; i < size; ++i) {
        printf("%0*d", setSize, arr[i]);
        //adding a space after every number but the last element
        if (i != size - 1)
            printf(" ");
    }
    printf("\n");
}

void printArr2D(int arr[][MAX], int row, int column) {
    /*******************************************************************************
    * Function name: printArr2D
    * Input: int arr[][] an array of integers
    *        int row numbers of rows in the array
    *        int column number of columns in the array
    * Output: none
    * Function Operation: the function gets a 2D array and returning prints the array
    *                     in an evenly distributed way by adding a zeros to make all the
    *                     numbers the same size by the factor of its biggest number
    *******************************************************************************/
    //an array of all the max numbers in every row
    int maxInRow[row], max;
    for (int i = 0; i < row; ++i) {
        maxInRow[i] = maxRow(arr[i], column);
    }
    //computing the biggest number in the array
    max = maxRow(maxInRow, row);
    //print every row
    for (int i = 0; i < row; ++i) {
        printRow(arr[i], column, max);
    }

}

void swap(int arr[], int i, int j) {
    /*******************************************************************************
    * Function name: swap
    * Input: int arr[] an array of integers
    *        int i a valid index in the array
    *        int j a valid index in the array
    * Output: none
    * Function Operation: the function gets an array and to indexes and swap the elements
    *                     of the array in those indexes
    *******************************************************************************/
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;


}

void sort(int arr[], int size) {
    /*******************************************************************************
    * Function name: sort
    * Input: int arr[] an array of integers
    *        int size the size of the array
    * Output: none
    * Function Operation: the function gets an array and sorts him in accending order
    *******************************************************************************/
    int isSwap = 0;
    do {
        //flag to check if swap has been done
        isSwap = 0;
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                //if the next element is smaller then the current one swap them
                swap(arr, i, i + 1);
                isSwap = 1;
            }
        }
        //if there wasn't a swap it's mean that the array is sorted
    } while (isSwap);
}


int isPermutation(int arr1[], int size1, int arr2[], int size2) {
    /*******************************************************************************
   * Function name: isPermutation
   * Input: int arr1[] an array of integers
   *        int size1 the size of the first array
   *        int arr2[] an array of integers
   *        int size1 the size of the second array
   * Output: int (1 or 0)
   * Function Operation: the function gets 2 1D arrays and checkes if they
   *                    permutation (the same array in different order)
   *                    of each other
   *******************************************************************************/
    int sortedArr1[size1], sortedArr2[size2], isEqual = 0;
    //the arrays have to be the same size and cant be of size 0
    if (size1 != size2 || size1 == 0 || size2 == 0)
        return 0;
    //duplicate the arrays so the sort function won't change the original arrays
    for (int i = 0; i < size1; i++) {
        sortedArr1[i] = arr1[i];
        sortedArr2[i] = arr2[i];
    }
    //sorting the arrays
    sort(sortedArr1, size1);
    sort(sortedArr2, size2);
    for (int i = 0; i < size1; i++) {
        //once the arrays are sorted if they were permutation of each other they should be identical
        if (sortedArr1[i] == sortedArr2[i])
            //
            isEqual++;
    }
    //if they were identical then every loop the counter went up and it should be the same value of size
    isEqual = isEqual == size1 ? 1 : 0;
    return isEqual;
}
