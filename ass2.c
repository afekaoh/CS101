/*************
 *Adam Shapira
 *316044809
 *01
 *ass02
 **************/
#include <stdio.h>
enum Boolean {FALSE , TRUE };
void draw() {
    /*
     get's a number x from the user and print a square with side of  size 2x+1
     with diagonals and special characters for corners and the middle
     */
    int drawSize;
    printf("Enter a number: ");
    scanf("%d", &drawSize);
    //computing the size of the sides
    drawSize = 2 * drawSize + 1;
    for (int i = 0; i < drawSize; ++i) {
        for (int j = 0; j < drawSize; ++j) {
            // middle
            if (i == j && i == ((drawSize) / 2))
                printf("X");
                //first or last row
            else if ((i == 0) || (i == drawSize - 1))
                //first or last column
                if ((j == 0) || (j == drawSize - 1))
                    printf("+");
                    //regular rows
                else
                    printf("-");
                //regular columns
            else if ((j == 0) || (j == drawSize - 1))
                printf("|");
                // left to right diagonal
            else if (i == j)
                printf("\\");
                // right to left diagonal
            else if (i == (drawSize - 1 - j))
                printf("/");
            else
                printf(" ");

        }
        printf("\n");
    }
}
void isEvenLength() {
    // the function gets a sequence of characters from the user and print if the length is odd or even
    char evenCheck;
    printf("Enter text: ");
    scanf(" %c",&evenCheck);
    int counter=1;
    while(evenCheck!='\n')
    {
        scanf("%c",&evenCheck);
        counter++;
    }
    printf("Your text's length is ");
    // checking the oddity of the number with bitwise operator and
    if (counter & 1) {
        printf("even\n");
    } else {
        printf("odd\n");
    }
}
void identifyText() {
    // the function gets a sequence of letters from the user and checking the relation between the letters.

    char firstLetter, secondLetter;
    //declaring flags to check the differents states of the sequence
    enum Boolean isIncrease=FALSE,isDecrease=FALSE,isMixed=FALSE,isValid=TRUE;
    printf("Enter text: ");
    // getting the first letter
    scanf(" %c",&firstLetter);
    // getting the second letter
    scanf("%c",&secondLetter);
    //if the user put only 1 valid letter
    if(secondLetter=='\n')
    {
        printf((firstLetter >= 'a') && (firstLetter <= 'z') ? "your text is constant\n" : "your text is invalid\n");
        return;
    }
    while(secondLetter!='\n'){
        //checking the validity of the input
        if(isValid&&((firstLetter>='a')&&(firstLetter<='z'))&&((secondLetter>='a')&&(secondLetter<='z')))
        {
            if (!isMixed&&(secondLetter > firstLetter))
                isIncrease = TRUE;
            else if (!isMixed&&(secondLetter<firstLetter))
                isDecrease = TRUE;
            //if both of the increase and the decrease flags are on the sequence is mixed
            if (isDecrease && isIncrease)
            {
                isIncrease = FALSE;
                isDecrease = FALSE;
                isMixed = TRUE;
            }
        }
            //not valid
        else {
            isValid=FALSE;
        }
        //shifting the second letter to the first and getting a new one
        firstLetter=secondLetter;
        scanf("%c",&secondLetter);

    }

    if (isValid) {
        printf("your text is ");
        if (isIncrease)
            printf("increasing\n");
        else if (isDecrease)
            printf("decreasing\n");
        else if (isMixed)
            printf("mixed\n");
            // if it's valid and neither increasing nor decreasing nor mixed it must be constant
        else
            printf("constant\n");
    }
    else
        printf("your text is invalid\n");
}
void hexToDec() {
    // the function gets a reversed number in base 16 and print its decimal value

    //const to covert digit to number
    int const DIG_INT='0'-0;
    //const to covert big letter to number in HEX base
    int const BIG_INT='A'-10;
    //const to covert small letter to number in HEX base
    int const SML_INT='a'-10;
    enum Boolean toPrint=TRUE;
    int toInt,answer=0,power=1;
    char digit;
    printf("Enter a reversed number in base 16: ");
    scanf(" %c",&digit);
    while(digit!='\n'){
        //converting the chars to ints
        if(digit>='0'&&digit<='9')
            toInt=digit-DIG_INT;
        else if(digit>='a'&&digit<='f')
            toInt=digit-SML_INT;
        else if(digit>='A'&&digit<='F')
            toInt=digit-BIG_INT;
        else {
            printf("Error! %c is not a valid digit in base 16\n",digit);
            //is there is at least 1 not valid characters the whole number should not be printed
            toPrint=FALSE;
            scanf("%c",&digit);
            continue;
        }

        // calculating the correct value of the digit base on it's placement in the number
        toInt*=power;
        answer+=toInt;
        power=power*16;
        scanf("%c",&digit);
    }
    if(toPrint)
        printf("%d\n",answer);
}
void baseToDec() {
    //the function gets a base and a reversed number in the same base from the user and prints its decimal value

    // the difference between a digit in base 10 and its ascii value
    const int HEX_TO_INT = '0' - 0;
    int toInt, answer = 0, power = 1,base;
    enum Boolean toPrint = TRUE;
    char digit;
    printf("Enter a base (2-10): ");
    scanf("%d", &base);
    printf("Enter a reversed number in base %d: ",base);
    scanf(" %c", &digit);
    while (digit != '\n')
    {
        // converting the char to int using the constant the we calculated before
        toInt = digit - HEX_TO_INT;
        // checking the validity of the digit
        if (toInt < 0 || toInt >= base)
        {
            printf("Error! %c is not a valid digit in base %d\n", digit, base);
            toPrint = FALSE;
            scanf("%c", &digit);
            continue;
        }
        toInt *= power;
        answer += toInt;
        power = power * base;
        scanf("%c",&digit);
    }
    if (toPrint)
        printf("%d\n", answer);
}
void bitCount() {
    // the function gets an integer and print all the active bits in his signed binary value

    //calculate the numbers of bits in an integer of 32 bit
    const int INT_SIZE= sizeof(int) * 8;
    printf("Enter a number: ");
    int numToTest,countBit=0;
    unsigned int tester=1;
    scanf("%d",&numToTest);
    for (int i = 0; i < INT_SIZE; ++i) {
        // the bit is equals to 1
        if(numToTest&tester)
            countBit++;
        //shifting to test the next bit
        tester=tester<<(unsigned)1;
    }
    printf("The bit count of %d is %d\n",numToTest,countBit);
}

int main() {
    int choice;
    do {
        printf("Choose an option:\n1: Draw\n2: Even or Odd\n3: Text type\n4: Hex to Dec\n5: Base to Dec\n6: Count bits\n0: Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                draw();
                break;
            case 2:
                isEvenLength();
                break;
            case 3:
                identifyText();
                break;
            case 4:
                hexToDec();
                break;
            case 5:
                baseToDec();
                break;
            case 6:
                bitCount();
            case 0:
                break;
            default:
                printf("Wrong option!\n");
        }
    } while (choice != 0);
    return 0;
}