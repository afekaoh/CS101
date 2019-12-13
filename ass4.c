#include <string.h>
#include <stdio.h>
#include "ass4.h"
#include <ctype.h>
#include <assert.h>
#include <math.h>

enum consts {
	FIRST_COL = 'A',
	EMPTY = ' ',
	MATE = '#',
	CHECK = '+',
	PROMOTION = '=',
	CAPTURE = 'x',
	PAWN = 'P'
/*
 * some definition
 * L = big letter
 * l = small letter
 * n = number
 *
 */
};
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, toPromote;
	int iSrc, jSrc, iDest, jDest, captureLoc;
	int isWhite, isCapture, isPromotion, isCheck, isMate, isLegal;
} Move;


void printMove(Move *move) {
	printf("src: %c, %c:%d, %c:%d; ", move->srcPiece, move->srcCol, move->jSrc, move->srcRow, move->iSrc);
	printf("dest: %c, %c:%d, %c:%d;", move->destPiece, move->destCol, move->jDest, move->destRow, move->iDest);
	if (move->isCapture)
		printf(" %c", CAPTURE);
	if (move->isPromotion) {
		printf(" %c%c", PROMOTION, move->toPromote);
	}
	if (move->isMate)
		printf(" %c;", MATE);
	else if (move->isCheck)
		printf(" %c;", CHECK);
	printf("\n");
}

int toDigit(char piece) {
	if ('0' < piece && piece <= '9')
		return piece - '0';
	return 0;
}

void creatRow(char boardRow[], char *tempRow) {
	int i = 0;
	while (*tempRow) {
		int spaces = 0;
		if (toDigit(*tempRow)) {
			spaces = toDigit(*tempRow);
			while (spaces) {
				boardRow[i++] = EMPTY;
				spaces--;
			}
		} else
			boardRow[i++] = *tempRow;
		tempRow++;
	}
	boardRow[i] = '\0';
}

void createBoard(char board[][SIZE], char fen[]) {
	int i = 0;
	char *fenRow = strtok(fen, "/");
	while (fenRow != NULL) {
		creatRow(board[i++], fenRow);
		fenRow = strtok(NULL, "/");
	}

}

void printColumns() {
	char column;
	column = FIRST_COL;
	printf("* |");
	for (int i = 0; i < SIZE; i++) {
		if (i) {
			printf("%c", EMPTY);
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
	for (int i = 0; i < SIZE; i++) {
		{
			printf("|%c", row[i]);
		}
	}
	printf("| %d\n", rowIdx);
}

void printBoard(char board[][SIZE]) {
	printColumns();
	printSpacers();

	for (int i = 0; i < SIZE; ++i) {
		printRow(board[i], i + 1);
	}
	printSpacers();
	printColumns();
}

void parseFlags(Move *move, char *index) {
	int counter = 0;
	while (*index) {
		if (*index == CAPTURE)
			move->isCapture = counter;
		if (*index == MATE)
			move->isMate = counter;
		if (*index == CHECK)
			move->isCheck = counter;
		if (*index == PROMOTION) {
			move->isPromotion = counter;
			move->toPromote = *(index + 1);
		}
		index++;
		counter++;
	}
}

char pieceColor(char piece, int isWhite) {
	if (isWhite)
		return piece;
	else
		return tolower(piece);
}

int toIndex(char location, int isWhite) {
	if (isdigit(location))
		return toDigit(location) - 1;
	else
		return (int) fmax(location - 'a', 0);

}

char* setDest(Move *move, char *index) {
	while (!isdigit(*index))
		index--;
	index--;
	move->destCol = index[0];
	move->destRow = index[1];
	return --index;     //returning the position of the previous element
}

void parseMove(char pgn[], int isWhiteTurn, Move *move) {
	move->isWhite = isWhiteTurn;
	int len = (int) strlen(pgn);
	char *lastPos = &(pgn[len - 1]);

	lastPos = setDest(move, lastPos);
	parseFlags(move, pgn);

	if (isupper(pgn[0])) {
		move->srcPiece = pieceColor(pgn[0], isWhiteTurn);
		if (move->isCapture)
			lastPos--;
		if (islower(*lastPos))
				move->srcCol = *lastPos;
		} else {
			move->srcRow = *(lastPos--);    //taking the pointer one step back
			if (islower(*lastPos))
				move->srcCol = *lastPos;
	}
	else {
		//the pgn is of the form [col][x](col)(row)
		move->srcPiece = pieceColor(PAWN, isWhiteTurn);
		if (move->isCapture)
			//the pgn is of the form (col)(x)(col)(row)
			move->srcCol = pgn[0];
	}
	move->iSrc = toIndex(move->srcRow, isWhiteTurn);
	move->jSrc = toIndex(move->srcCol, isWhiteTurn);
	move->iDest = toIndex(move->destRow, isWhiteTurn);
	move->jDest = toIndex(move->destCol, isWhiteTurn);
}
void makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	Move move = {};
	parseMove(pgn, isWhiteTurn, &move);

	printMove(&move);
//	printf("%s, %d\n", pgn, isWhiteTurn);

}