#include <string.h>
#include <stdio.h>
#include "ass4.h"
#include <ctype.h>
#include <math.h>

const char EMPTY = ' ';
const char CAPTURE = 'x';
const char PROMOTION = '=';
const char CHECK = '+';
const char MATE = '#';
const char FIRST_COL = 'A';
const char WHITE_PAWN = 'P';
const char WHITE_ROOK = 'R';
const char WHITE_KNIGHT = 'N';
const char WHITE_BISHOP = 'B';
const char WHITE_QUEEN = 'Q';
const char WHITE_KING = 'K';
const char BLACK_PAWN = 'p';
const char BLACK_ROOK = 'r';
const char BLACK_KNIGHT = 'n';
const char BLACK_BISHOP = 'b';
const char BLACK_QUEEN = 'q';
const char BLACK_KING = 'k';
char const delim[] = "/";

typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, toPromote;
	int iSrc, jSrc, iDest, jDest;
	int isWhite, isCapture, isPromotion, isCheck, isMate, isLegal;
} Move;


void printMove(Move *move) {
	printf("src: '%c', %c:%d, %c:%d; ", move->srcPiece, move->srcCol, move->jSrc, move->srcRow, move->iSrc);
	printf("dest: '%c', %c:%d, %c:%d;", move->destPiece, move->destCol, move->jDest, move->destRow, move->iDest);
	if (move->isCapture)
		printf(" %c", CAPTURE);
	if (move->isPromotion) {
		printf(" %c", move->toPromote);
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
	int j = 0;
	while (*tempRow) {
		int spaces = 0;
		if (isdigit(*tempRow)) {
			spaces = toDigit(*tempRow);
			while (spaces) {
				boardRow[j++] = EMPTY;
				spaces--;
			}
		} else
			boardRow[j++] = *tempRow;
		tempRow++;
	}
}

void createBoard(char board[][SIZE], char fen[]) {
	int i = 0;
	char *fenRow = strtok(fen, delim);
	while (fenRow != NULL) {
		creatRow(board[i++], fenRow);
		fenRow = strtok(NULL, delim);
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
		printRow(board[i], SIZE - i);
	}
	printSpacers();
	printColumns();
}

char pieceColor(char piece, int isWhite) {
	if (isWhite)
		return piece;
	else
		return tolower(piece);
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
			move->toPromote = pieceColor(*(index + 1), move->isWhite);
		}
		index++;
		counter++;
	}
}

int toIndex(char position) {
	if (isdigit(position))
		return SIZE - toDigit(position);
	else
		return (int) fmax(position - 'a', 0);

}

char *setDest(Move *move, char *index) {
	while (!isdigit(*index))
		index--;
	index--;
	move->destCol = index[0];
	move->destRow = index[1];
	return --index;     //returning the position of the previous element
}

void parseMove(char pgn[], Move *move) {
	int len = (int) strlen(pgn);
	//setting a pointer to the last position of the array
	char *lastPos = &(pgn[len - 1]);

	lastPos = setDest(move, lastPos);
	parseFlags(move, pgn);

	/*
	 * after parseFlags and setDest we only need to check the first optional part of the pgn
	 * of the form [Piece][col][row]
	 */
	if (isupper(pgn[0])) {
		//the pgn is of the form (Piece)[col][row]
		move->srcPiece = pieceColor(pgn[0], move->isWhite);
		if (move->isCapture)
			lastPos--;
		if (islower(*lastPos))
			move->srcCol = *lastPos;
		else if (isdigit(*lastPos)) {
			move->srcRow = *(lastPos--);    //taking the pointer one step back
			if (islower(*lastPos))
				move->srcCol = *lastPos;
		}
	} else {
		//the piece is a pawn
		move->srcPiece = pieceColor(WHITE_PAWN, move->isWhite);
		if (move->isCapture)
			//the pgn is of the form (col)(x)(col)(row)
			move->srcCol = pgn[0];
	}
	move->iSrc = toIndex(move->srcRow);
	move->jSrc = toIndex(move->srcCol);
	move->iDest = toIndex(move->destRow);
	move->jDest = toIndex(move->destCol);
}


char *findPieceLoc(char board[][SIZE], Move *move, char toLook) {
	char *pos = NULL;
	int i = move->iDest;
	int j = move->jDest;
	char piece = move->srcPiece;
	if (islower(piece) == islower(board[i][j]))
		return NULL;
	if (toLook) {
		if (isdigit(toLook)) {
			//we have the row
		} else {
			//we have the column
		}
	} else {
		//we have neither
		if (move->isWhite) {
			if (piece == WHITE_PAWN) {
				if (move->isCapture) {
					if (board[i - 1][j - 1] == piece)
						return &board[i - 1][j - 1];
					if (board[i - 1][j + 1] == piece)
						return &board[i - 1][j + 1];
					return NULL;
				}
				else{
					if(board[i-1][j]==piece)
						return &board[i-1][j];
					if(i==4)
						if(board[i-2][j]==piece)
							return &board[i-2][j];
					return NULL;
				}
			}
		}
	}
	return pos;

}

void updateMove(char board[][SIZE], Move *move) {
	move->destPiece = board[move->iDest][move->jDest];
	char toLook = 0;
	if (!move->srcRow || !move->srcCol) {
		if (move->srcRow)
			toLook = move->srcRow;

		else if (move->srcRow)
			toLook = move->srcCol;

		char *pos = findPieceLoc(board, move, toLook);
	}
}


int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	Move move = {};
	move.isWhite = isWhiteTurn;
	parseMove(pgn, &move);
	updateMove(board, &move);
	printMove(&move);
//	printf("%s, %d\n", pgn, isWhiteTurn);
	return 0;
}