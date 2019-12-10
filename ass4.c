#include <string.h>
#include <stdio.h>
#include "ass4.h"
#include <ctype.h>
typedef struct {
    // some constants that i
    char FIRST_COL;
    char space;
    char delim[];

}MakeBoard;
 MakeBoard const consts={'A', ' ', "/"};

int toDigit(char piece) {
    if ('0' < piece && piece < '9')
        return piece - '0';
    return 0;
}

void rowBoard(char boardRow[], char tempRow[]) {
int i=0;
    while(*tempRow){
    int spaces=0;
    if(toDigit(*tempRow)) {
        spaces = toDigit(*tempRow);
        while (spaces) {
            boardRow[i++] = consts.space;
            spaces--;
        }
    }
    else
        boardRow[i]=*tempRow;
    i++;
    tempRow++;
}
    boardRow[i]='\0';
}

void createBoard(char board[][SIZE], char fen[]) {
    int i = 0;

    char tempBorad[SIZE][SIZE] = {};
    printf("%s\n", fen);
    int spaces = 0;
    char *fenRow = strtok(fen, consts.delim);
    while (fenRow != NULL) {
        strcpy(tempBorad[i++],fenRow);
        fenRow = strtok(NULL, consts.delim);
    }
   for(i=0;i<SIZE;i++){
       rowBoard(board[i],tempBorad[i]);
   }
}

void printColumns() {
    char column;
    column = consts.FIRST_COL;
    printf("* |");
    for (int i = 0; i < SIZE; i++) {
        if (i) {
            printf("%c",consts.space);
        }
        printf("%c", column);
        column++;
    }
    printf("| *\n");
}

void printSpacers() {
    printf("* -");
    for (int i = 0; i < SIZE; i++) {
        printf("--");
    }
    printf(" *\n");
}

void printRow(char row[], int rowIdx) {
    printf("%d ", rowIdx);
    for (int i=0;i<SIZE;i++) {
        {
            printf("|%c", row[i]);
        }
    }
    printf("| %d\n", rowIdx);
}

void printBoard(char board[][SIZE]) {
    printColumns();
    printSpacers();

        for (int i = 0; i <SIZE; ++i) {
                printRow(board[i],i+1);
            }
    printColumns();
    printSpacers();
}

void makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {


}