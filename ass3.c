/*******************
* Adam Shay Shapira
* 316044809
* 01
* ass03
********************/
#include <math.h>
#include <stdio.h>
#include "ass3.h"
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
	//is negative to know when the power is negative
	int isNeg = 0;
	double result = 1.00;
	//edge cases
	if (power == 0)
		return 1;
	// 0^-y y>0 is infinity
	if (power < 0 && base == 0)
		// power<0 then x^y is 1/x^-y which is 1/(0^y)
	    return 1.00/base;
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
	counter++;
	int isNeg = 0;
	double result;
	if (power == 0)
		return 1;
    if (power < 0 && base == 0)
        return 1.00/base;
	if (power == 1 || base == 0)
		return base;

	if (power < 0) {
		power *= -1;
		isNeg = 1;
	}
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
    counter++;
	int isNeg = 0;
	double isOdd = 0;
	double result;
	if (power == 0)
		return 1;
    if (power < 0 && base == 0)
        return 1.00/base;
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
		//if arr[i]>max then max =arr[i]
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

	//there is no number with 0 digits
	int setSize=1;
	//edge case of matrix full only with 0's
	if(max!=0)
        //the log will return number of digit -1
        setSize= (int) log10(max) + 1;
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

void createTable(int arr[], int table[], int size) {
	/*******************************************************************************
* Function name: createTable
* Input: int arr[] an array of integers with elements between o-size
\*        int table[] an empty array of integers
*        int size the size of the arrays
* Output: none
* Function Operation: the function gets 2 1D arrays and creates a table of arr
*                     so that in each element of table (table[i]) there is how many
*                     element of arr are the same value
*******************************************************************************/
	for (int i = 0; i < size; ++i) {
		table[arr[i]]++;
	}

}

int isPermutation(int arr1[], int size1, int arr2[], int size2) {
	/*******************************************************************************
   * Function name: isPermutation
   * Input: int arr1[] an array of integers
   *        int size1 the size of the first array
   *        int arr2[] an array of integers
   *        int size1 the size of the second array
   * Output: int (1 or 0)
   * Function Operation: the function gets 2 1D arrays and checks if they
   *                    permutation (the same array in different order)
   *                    of each other by creating tables of the values and comper them
   *******************************************************************************/
	//the arrays have to be the same size and can't be of size 0
	if (size1 != size2 || size1 == 0)
		return 0;

	//from here i know that size1=size 2 so for convenience purposes i will use size1 for the size of both arrays.

	int tableArr1[size1], tableArr2[size2], isEqual = 0;
	//initialized the new arrays with all 0
	for (int i = 0; i < size1; i++) {
		tableArr1[i] = 0;
		tableArr2[i] = 0;
	}
	createTable(arr1, tableArr1, size1);
	createTable(arr2, tableArr2, size2);
	for (int i = 0; i < size1; i++) {
		//checking if the tables are equal if the arrays were permutation of each other they should be identical
		if (tableArr1[i] == tableArr2[i])
			isEqual++;
	}
	//if they were identical then every loop the counter went up and it should be the same value of size
	isEqual = isEqual == size1 ? 1 : 0;
	return isEqual;
}
