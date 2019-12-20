#include <string.h>
#include <stdio.h>
#include "ass4.h"
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
//number of the different pieces in chess
#define NUM_OF_PIECES 6

//some const decleration for later use
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
char const DELIM[] = "/";
char PIECES[NUM_OF_PIECES] = {'P', 'R', 'N', 'B', 'Q', 'K'};

typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, toPromote;
	int iSrc, jSrc, iDest, jDest;
	int isSrcWhite, isDestWhite, isCapture, isPromotion, isCheck, isLegal;
} Move;

void makeStep(char [][SIZE], Move *);

int findPiece(char board[][SIZE], Move *move, int checkCheck);

int toDigit(char digit) {
	//function that gets character of digit and return its value
	assert('0' <= digit && digit <= '9');
	return digit - '0';
}

void creatRow(char boardRow[], char *tempRow) {
	//todo
	int i = 0;
	while (*tempRow) {
		int spaces = 0;
		if (isdigit(*tempRow)) {
			spaces = toDigit(*tempRow);
			while (spaces) {
				boardRow[i++] = EMPTY;
				spaces--;
			}
		} else
			boardRow[i++] = *tempRow;
		tempRow++;
	}
}

void createBoard(char board[][SIZE], char fen[]) {
	//todo
	int i = 0;
	char *fenRow = strtok(fen, DELIM);
	while (fenRow != NULL) {
		creatRow(board[i++], fenRow);
		fenRow = strtok(NULL, DELIM);
	}

}

void printColumns() {
	//todo
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
	//todo
	printf("* -");
	for (int i = 0; i < SIZE; i++) {
		printf("--");
	}
	printf(" *\n");
}

void printRow(char row[], int rowIdx) {
	//todo
	printf("%d ", rowIdx);
	for (int i = 0; i < SIZE; i++) {
		{
			printf("|%c", row[i]);
		}
	}
	printf("| %d\n", rowIdx);
}

void printBoard(char board[][SIZE]) {
	//todo
	printColumns();
	printSpacers();

	for (int i = 0; i < SIZE; ++i) {
		printRow(board[i], SIZE - i);
	}
	printSpacers();
	printColumns();
}

char setColor(char piece, int isWhite) {
	//todo
	if (isWhite)
		return piece;
	else
		return tolower(piece);
}

void parseFlags(Move *move, char *index) {
	//todo
	while (*index) {
		if (*index == CAPTURE)
			move->isCapture = 1;
		if (*index == CHECK || *index == MATE)
			move->isCheck = 1;
		if (*index == PROMOTION) {
			move->isPromotion = 1;
			move->toPromote = setColor(*(index + 1), move->isSrcWhite);
		}
		index++;
	}
}

int toIndex(char position) {
	//todo
	//the function gets a char position (can be a row or a column) and returns its value on the board
	return isdigit(position) ? SIZE - toDigit(position) : (int) fmax(position - 'a', -1);
}

char *setDest(Move *move, char *index) {
	//todo
	while (!isdigit(*index))
		index--;
	index--;
	move->destCol = index[0];
	move->destRow = index[1];
	return --index;     //returning the position of the previous element
}

void parseMove(char pgn[], Move *move) {
	//todo
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
		move->srcPiece = setColor(pgn[0], move->isSrcWhite);
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
		move->srcPiece = setColor(WHITE_PAWN, move->isSrcWhite);
		if (move->isCapture)
			//the pgn is of the form (col)(x)(col)(row)
			move->srcCol = pgn[0];
	}
	move->iSrc = toIndex(move->srcRow);
	move->jSrc = toIndex(move->srcCol);
	move->iDest = toIndex(move->destRow);
	move->jDest = toIndex(move->destCol);
}

int canStep(Move *move) {
	if (move->isCapture) {
		if (move->destPiece != EMPTY)
			if (!move->isDestWhite != !move->isSrcWhite)
				//false if both the src and dest is the same color and true if they are different
				return 1;
		return 0;
	}
	if (move->destPiece == EMPTY)//and isCapture is false
		return 1;
	return 0;
}

int isPawn(char piece) {
	//return if the piece is Pawn
	return piece == WHITE_PAWN || piece == BLACK_PAWN;
}

int isRook(char piece) {
	//return if the piece is Rook
	return piece == WHITE_ROOK || piece == BLACK_ROOK;
}

int isBishop(char piece) {
	//return if the piece is Bishop
	return piece == BLACK_BISHOP || piece == WHITE_BISHOP;
}

int isQueen(char piece) {
	//return if the piece is Queen
	return piece == WHITE_QUEEN || piece == BLACK_QUEEN;
}

int isKing(char piece) {
	//return if the piece is King
	return piece == WHITE_KING || piece == BLACK_KING;
}

int isKnight(char piece) {
	//return if the piece is Knight
	return piece == WHITE_KNIGHT || piece == BLACK_KNIGHT;
}

int setMove(Move *move, int i, int j, int legal) {
	//todo
	if (move->isPromotion)
		move->srcPiece = setColor(move->toPromote, move->isSrcWhite);
	move->iSrc = i;
	move->jSrc = j;
	move->isLegal = legal;
	return 1;
}

void findKingOnBoard(char board[][SIZE], int kingColor, Move *move) {
	//todo
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {

			if (board[i][j] == setColor(WHITE_KING, kingColor)) {
				move->iDest = i;
				move->jDest = j;
				return;
			}

		}
	}
}

void copyBoard(char const board[][SIZE], char tempBoard[][SIZE]) {
//the function gets 2 2D arrays and copy the first one to the other
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			tempBoard[i][j] = board[i][j];
		}

	}
}

int isCheck(char tempBoard[][SIZE], int kingColor, Move *move) {
	//todo
	findKingOnBoard(tempBoard, kingColor, move);
	for (int index = 0; index < NUM_OF_PIECES; ++index) {
		move->srcPiece = setColor(PIECES[index], !kingColor);
		if (move->srcPiece == setColor(WHITE_PAWN, !kingColor)) {
			//checking for pawns that can capture the king
			int pawnMove = (int) pow(-1, kingColor);
			if (move->iDest + pawnMove >= 0 && move->iDest + pawnMove < SIZE) {
				if (move->jDest > 0)
					if (tempBoard[move->iDest + pawnMove][move->jDest - 1] == setColor(WHITE_PAWN, !kingColor))
						return 1;
				if (move->jDest < SIZE - 1)
					if (tempBoard[move->iDest + pawnMove][move->jDest + 1] == setColor(WHITE_PAWN, !kingColor))
						return 1;
			}
		} else if (findPiece(tempBoard, move, 1))
			return 1;
	}
	return 0;
}

void makeTempMove(char const board[][SIZE], Move const *move, int i, int j, int kingColor, Move *tempMove,
                  char tempBoard[][SIZE]) {
	//todo
	tempMove->srcPiece = move->srcPiece;
	setMove(tempMove, i, j, 0);
	tempMove->iDest = move->iDest;
	tempMove->jDest = move->jDest;
	tempMove->isDestWhite = kingColor;
	copyBoard(board, tempBoard);
	makeStep(tempBoard, tempMove);
}

int isValidMove(char board[][SIZE], Move *move, int i, int j, int kingColor) {
	Move tempMove = {};
	char tempBoard[SIZE][SIZE];
	makeTempMove(board, move, i, j, kingColor, &tempMove, tempBoard);

	if (isCheck(tempBoard, kingColor, &tempMove))
		//if there is check on the played side
		return 0;

	if (isCheck(tempBoard, !kingColor, &tempMove))
		//if there is check on the opponent side
		if (!move->isCheck)
			return 0;

	return setMove(move, i, j, 1);
}


int findPawn(char board[][SIZE], Move *move) {
	//todo
	int pawnMove = (int) pow(-1, move->isSrcWhite);
	int i = move->iDest;
	int j = move->jDest;
	char piece = move->srcPiece;
	if (move->isCapture) {
		if (board[i][j] != EMPTY) {
			if (board[i - pawnMove][move->jSrc] == piece) {
				move->isLegal = isValidMove(board, move, i - pawnMove, move->jSrc, move->isSrcWhite);
				return 1;
			}
		}
		return 0;
	}
	if (board[i - pawnMove][j] == piece) {
		move->isLegal = isValidMove(board, move, i - pawnMove, j, move->isSrcWhite);
		return 1;
	}
	if ((move->isSrcWhite && (board[SIZE - 2][j] == piece)) || (!move->isSrcWhite && (board[1][j] == piece)))
		if (board[i - pawnMove][j] == EMPTY) {
			move->isLegal = isValidMove(board, move, i - (2 * pawnMove), j, move->isSrcWhite);
			return 1;
		}
	return 0;
}

int findInCol(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	for (int i = move->iDest - 1; i >= 0; --i) {
		if (board[i][move->jDest] == EMPTY)
			continue;
		if (board[i][move->jDest] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, i, move->jDest, move->isSrcWhite))
				return 1;
		}
		break;
	}
	for (int i = move->iDest + 1; i < SIZE; ++i) {
		if (board[i][move->jDest] == EMPTY)
			continue;
		if (board[i][move->jDest] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, i, move->jDest, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findInRow(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	for (int j = move->jDest - 1; j >= 0; j--) {
		if (board[move->iDest][j] == EMPTY)
			continue;
		if (board[move->iDest][j] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, move->iDest, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	for (int j = move->jDest + 1; j < SIZE; ++j) {
		if (board[move->iDest][j] == EMPTY)
			continue;
		if (board[move->iDest][j] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, move->iDest, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findRook(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	if (findInCol(board, move, checkCheck))
		return 1;
	if (findInRow(board, move, checkCheck))
		return 1;
	return 0;
}

int findInSecDiag(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	int i = move->iDest + 1;
	int j = move->jDest - 1;
	while (i < SIZE && j >= 0) {
		if (board[i][j] == EMPTY) {
			i++;
			j--;
			continue;
		}
		if (board[i][j] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, i, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	i = move->iDest - 1;
	j = move->jDest + 1;
	while (i >= 0 && j < SIZE) {
		if (board[i][j] == EMPTY) {
			i--;
			j++;
			continue;
		}
		if (board[i][j] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, i, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findInMainDiag(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	int i = move->iDest - 1;
	int j = move->jDest - 1;
	while (i >= 0 && j >= 0) {
		if (board[i][j] == EMPTY) {
			i--;
			j--;
			continue;
		}
		if (board[i][j] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, i, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	i = move->iDest + 1;
	j = move->jDest + 1;
	while (i < SIZE && j < SIZE) {
		if (board[i][j] == EMPTY) {
			i++;
			j++;
			continue;
		}
		if (board[i][j] == move->srcPiece) {
			if (checkCheck)
				return 1;
			if (isValidMove(board, move, i, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findBishop(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	if (findInMainDiag(board, move, checkCheck))
		return 1;
	if (findInSecDiag(board, move, checkCheck))
		return 1;
	return 0;
}

int findQueen(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	if (findInRow(board, move, checkCheck))
		return 1;
	if (findInCol(board, move, checkCheck))
		return 1;
	if (findInMainDiag(board, move, checkCheck))
		return 1;
	if (findInSecDiag(board, move, checkCheck))
		return 1;
	return 0;
}

int findKing(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	int iMove[] = {-1, 0, 1};
	int jMove[] = {-1, 0, 1};
	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			int i = move->iDest + iMove[l];
			int j = move->jDest + jMove[k];
			if (i >= SIZE || i < 0 || j >= SIZE || j < 0)
				continue;
			if (board[i][j] == move->srcPiece) {
				if (checkCheck)
					return 1;
				if (isValidMove(board, move, i, j, move->isSrcWhite))
					return 1;
			}
		}
	}
	return 0;
}

int findKnight(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	int iMove[] = {1, 2, 1, 2, -1, -2, -1, -2};
	int jMove[] = {2, 1, -2, -1, 2, 1, -2, -1};
	for (int index = 0; index < 8; ++index) {
		int i = move->iDest + iMove[index];
		int j = move->jDest + jMove[index];
		if (i >= SIZE || i < 0 || j >= SIZE || j < 0)
			continue;
		if (board[i][j] == move->srcPiece) {
			if (!checkCheck) {
				if (isValidMove(board, move, i, j, move->isSrcWhite))
					return 1;
			} else
				return 1;
		}
	}
	return 0;
}

void makeStep(char board[][SIZE], Move *move) {
	//todo
	board[move->iSrc][move->jSrc] = EMPTY;
	board[move->iDest][move->jDest] = move->srcPiece;
}

int findPiece(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	char piece = move->srcPiece;
	if (isPawn(piece))
		return findPawn(board, move);   //no need for checkCheck because the logic makes sure it won't enter here
	if (isRook(piece))
		return findRook(board, move, checkCheck);
	if (isBishop(piece))
		return findBishop(board, move, checkCheck);
	if (isQueen(piece))
		return findQueen(board, move, checkCheck);
	if (isKing(piece))
		return findKing(board, move, checkCheck);
	if (isKnight(piece))
		return findKnight(board, move, checkCheck);
	return 0;
}

int isClearCol(char board[][SIZE], Move *move, int i, int j, int sign) {
	//todo
	i += sign;
	while (i != move->iDest) {
		if (board[i][j] != EMPTY)
			return 0;
		i += sign;
	}
	if (i == move->iDest && j == move->jDest)
		return 1;
	return 0;
}

int isClearRow(char board[][SIZE], Move *move, int i, int j, int sign) {
	//todo
	j += sign;
	while (j != move->jDest) {
		if (board[i][j] != EMPTY)
			return 0;
		j += sign;
	}
	if (j == move->jDest && i == move->iDest)
		return 1;
	return 0;
}

int isClearDiag(char board[][SIZE], Move *move, int i, int j, int iSign, int jSign) {
	//todo
	i += iSign;
	j += jSign;
	while ((j != move->jDest) && (i != move->iDest)) {
		if (board[i][j] != EMPTY)
			return 0;
		i += iSign;
		j += jSign;
	}
	if (i == move->iDest && j == move->jDest)
		return 1;
	return 0;
}

int isClearKnight(int i, int j, Move *move) {
	//todo
	if (abs(move->iDest - i) == 2 && abs(move->jDest - j) == 1)
		return 1;
	if (abs(move->iDest - i) == 1 && abs(move->jDest - j) == 2)
		return 1;
	return 0;
}

int canMove(char board[][SIZE], Move *move, int iSrc, int jSrc) {
	//todo
	char piece = move->srcPiece;
	int iSign = move->iDest - iSrc > 0 ? 1 : -1;
	int jSign = move->jDest - jSrc > 0 ? 1 : -1;
	if (isRook(move->srcPiece))
		return isClearCol(board, move, iSrc, jSrc, iSign) || isClearRow(board, move, iSrc, jSrc, jSign);
	if (isBishop(piece))
		return isClearDiag(board, move, iSrc, jSrc, iSign, jSign);
	if (isQueen(piece))
		return isClearCol(board, move, iSrc, jSrc, iSign) || isClearRow(board, move, iSrc, jSrc, jSign) ||
		       isClearDiag(board, move, iSrc, jSrc, iSign, jSign);
	if (isKnight(piece))
		return isClearKnight(iSrc, jSrc, move);
	return 0;
}

void updateMove(char board[][SIZE], Move *move) {
	//todo
	int const checkCheck = 0;
	move->destPiece = board[move->iDest][move->jDest];
	move->isDestWhite = isupper(move->destPiece);
	char piece = move->srcPiece;
	if (canStep(move)) {
		if (!(move->srcRow && move->srcCol)) {
			if (move->srcRow && !isPawn(piece)) {
				for (int j = 0; j < SIZE; ++j) {
					if (board[move->iSrc][j] == piece)
						if (canMove(board, move, move->iSrc, j))
							if (isValidMove(board, move, move->iSrc, j, move->isSrcWhite))
								break;
				}
			} else if (move->srcCol && !isPawn(piece)) {
				for (int i = 0; i < SIZE; ++i) {
					if (board[i][move->jSrc] == piece)
						if (canMove(board, move, i, move->jSrc))
							if (isValidMove(board, move, i, move->jSrc, move->isSrcWhite))
								break;
				}
			} else
				findPiece(board, move, checkCheck);
		} else
			isValidMove(board, move, move->iSrc, move->jSrc, move->isSrcWhite);
		if (move->isLegal)
			makeStep(board, move);
	}
}

int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	//todo
	Move move = {};
	move.isSrcWhite = isWhiteTurn;
	parseMove(pgn, &move);
	updateMove(board, &move);
	return move.isLegal;

}