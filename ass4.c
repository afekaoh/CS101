#include <string.h>
#include <stdio.h>
#include "ass4.h"
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define NUM_OF_PIECES 6

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
char PIECES[NUM_OF_PIECES] = {'P', 'R', 'N', 'B', 'Q', 'K'};

typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, toPromote;
	int iSrc, jSrc, iDest, jDest;
	int isSrcWhite, isDestWhite, isCapture, isPromotion, isCheck, isMate, isLegal;
} Move;

int findKnight(char[][SIZE], Move *, int);

void makeStep(char [][SIZE], Move *);

int findPiece(char board[][SIZE], Move *move, int flag);

int toDigit(char piece) {
	assert('0' <= piece && piece <= '9');
	return piece - '0';
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

char setColor(char piece, int isWhite) {
	if (isWhite)
		return piece;
	else
		return tolower(piece);
}

void parseFlags(Move *move, char *index) {
	while (*index) {
		if (*index == CAPTURE)
			move->isCapture = 1;
		if (*index == MATE)
			move->isMate = 1;
		if (*index == CHECK)
			move->isCheck = 1;
		if (*index == PROMOTION) {
			move->isPromotion = 1;
			move->toPromote = setColor(*(index + 1), move->isSrcWhite);
		}
		index++;
	}
}

int toIndex(char position) {
	return isdigit(position) ? SIZE - toDigit(position) : (int) fmax(position - 'a', -1);
}

char *setDest(Move *move, char *index) {
	while (!isdigit(*index))
		index--;
	index--;
	move->destCol = index[0];
	move->destRow = index[1];
	return --index;     //returning the position of the previous element
}

int parseMove(char pgn[], Move *move) {
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
	return 1;
}

int canStep(Move *move) {
	if (move->isCapture) {
		if (move->destPiece != EMPTY)
			if (!move->isDestWhite != !move->isSrcWhite)
				return 1;
		return 0;
	}
	if (move->destPiece == EMPTY)
		return 1;
	return 0;
}

char checkColor(char piece) {
	if (isupper(piece))
		return 1;

	return 0;
}

int isPawn(char piece) {
	return piece == WHITE_PAWN || piece == BLACK_PAWN;
}

int isRook(char piece) {
	return piece == WHITE_ROOK || piece == BLACK_ROOK;
}

int isBishop(char piece) {
	return piece == BLACK_BISHOP || piece == WHITE_BISHOP;
}

int isQueen(char piece) {
	return piece == WHITE_QUEEN || piece == BLACK_QUEEN;
}

int isKing(char piece) {
	return piece == WHITE_KING || piece == BLACK_KING;
}

int isKnight(char piece) {
	return piece == WHITE_KNIGHT || piece == BLACK_KNIGHT;
}

int setMove(Move *move, int i, int j) {
	if (move->isPromotion)
		move->srcPiece = setColor(move->toPromote, move->isSrcWhite);
	move->iSrc = i;
	move->jSrc = j;
	move->isLegal = 1;
	return 1;
}

void findKingOnBoard(char board[][SIZE], int kingColor, int *iKing, int *jKing) {
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {

			if (board[i][j] == setColor(WHITE_KING, kingColor)) {
				(*iKing) = i;
				(*jKing) = j;
				return;
			}

		}
	}
}

void copyBoard(char board[][SIZE], int kingColor, char tempBoard[][SIZE]) {

	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			tempBoard[i][j] = board[i][j];
		}

	}
}

int checkOtherCheck(Move const *move, int iOther, int jOther, Move *checkKing, char tempBoard[][SIZE]) {
	if (findPiece(tempBoard, checkKing, 1)) {
		if (move->isCheck || move->isMate) {
			return 1;
		}
		return 0;
	}
	return 1;
}

int isValidMove(char board[][SIZE], Move *move, int i, int j, int kingColor) {
	int iKing = 0, jKing = 0, iOther = 0, jOther = 0;
	Move checkKing = {};
	checkKing.srcPiece = move->srcPiece;
	checkKing.iSrc = i;
	checkKing.jSrc = j;
	checkKing.iDest = move->iDest;
	checkKing.jDest = move->jDest;
	checkKing.isDestWhite = kingColor;
	char tempBoard[SIZE][SIZE];
	copyBoard(board, kingColor, tempBoard);
	makeStep(tempBoard, &checkKing);
	findKingOnBoard(tempBoard,kingColor,&checkKing.iDest,&checkKing.jDest);

	for (int index = 0; index < NUM_OF_PIECES; ++index) {
		checkKing.srcPiece = setColor(PIECES[index], !kingColor);
		if (findPiece(tempBoard, &checkKing, 1))
			return 0;
	}
	findKingOnBoard(tempBoard,!kingColor,&checkKing.iDest,&checkKing.jDest);
	for (int index = 0; index <NUM_OF_PIECES;++index) {
		checkKing.srcPiece=setColor(PIECES[index],kingColor);
		if (!checkOtherCheck(move, iOther, jOther, &checkKing, tempBoard))
			return 0;
	}

	return setMove(move, i, j);
}


int findPawn(char board[][SIZE], Move *move, int flag) {
	int pawnMove = (int) pow(-1, move->isSrcWhite);
	int i = move->iDest;
	int j = move->jDest;
	char piece = move->srcPiece;
	if (move->isCapture) {
		if (board[i][j] != EMPTY) {
			if (board[i - pawnMove][move->jSrc] == piece) {
				if (flag)
					return 1;
				move->isLegal = isValidMove(board, move, i - pawnMove, move->jSrc, move->isSrcWhite);
				return 1;
			}
		}
		return 0;
	}
	if (board[i - pawnMove][j] == piece) {
		if (flag)
			return 1;
		move->isLegal = isValidMove(board, move, i - pawnMove, j, move->isSrcWhite);
		return 1;
	}
	if ((move->isSrcWhite && (board[SIZE - 4][j]==piece)) || (!move->isSrcWhite && (board[3][j]==piece)))
		if (board[i - pawnMove][j] == EMPTY) {
			if (flag)
				return 1;
			move->isLegal = isValidMove(board, move, i - (2 * pawnMove), j, move->isSrcWhite);
			return 1;
		}
	return 0;
}

int findInCol(char board[][SIZE], Move *move, int flag) {
	for (int i = move->iDest - 1; i >= 0; --i) {
		if (board[i][move->jDest] == EMPTY)
			continue;
		if (board[i][move->jDest] == move->srcPiece) {
			if (flag)
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
			if (flag)
				return 1;
			if (isValidMove(board, move, i, move->jDest, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findInRow(char board[][SIZE], Move *move, int flag) {
	for (int j = move->jDest - 1; j >= 0; j--) {
		if (board[move->iDest][j] == EMPTY)
			continue;
		if (board[move->iDest][j] == move->srcPiece) {
			if (flag)
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
			if (flag)
				return 1;
			if (isValidMove(board, move, move->iDest, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findRook(char board[][SIZE], Move *move, int flag) {
	if (findInCol(board, move, flag))
		return 1;
	if (findInRow(board, move, flag))
		return 1;
	return 0;
}

int findnSecDiag(char board[][SIZE], Move *move, int flag) {
	int i = move->iDest + 1;
	int j = move->jDest - 1;
	while (i < SIZE && j >= 0) {
		if (board[i][j] == EMPTY) {
			i++;
			j--;
			continue;
		}
		if (board[i][j] == move->srcPiece) {
			if (flag)
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
			if (flag)
				return 1;
			if (isValidMove(board, move, i, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}


int findnMainDiag(char board[][SIZE], Move *move, int flag) {
	int i = move->iDest - 1;
	int j = move->jDest - 1;
	while (i >= 0 && j >= 0) {
		if (board[i][j] == EMPTY) {
			i--;
			j--;
			continue;
		}
		if (board[i][j] == move->srcPiece) {
			if (flag)
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
			if (flag)
				return 1;
			if (isValidMove(board, move, i, j, move->isSrcWhite))
				return 1;
		}
		break;
	}
	return 0;
}

int findBishop(char board[][SIZE], Move *move, int flag) {
	if (findnMainDiag(board, move, flag))
		return 1;
	if (findnSecDiag(board, move, flag))
		return 1;
	return 0;
}

int findQueen(char board[][SIZE], Move *move, int flag) {
	if (findInRow(board, move, flag))
		return 1;
	if (findInCol(board, move, flag))
		return 1;
	if (findnMainDiag(board, move, flag))
		return 1;
	if (findnSecDiag(board, move, flag))
		return 1;
	return 0;
}

int findKing(char board[][SIZE], Move *move, int flag) {
	int iMove[] = {-1, 0, 1};
	int jMove[] = {-1, 0, 1};
	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			int i = move->iDest + iMove[l];
			int j = move->jDest + jMove[k];
			if (i >= SIZE || i < 0 || j >= SIZE || j < 0)
				continue;
			if (board[i][j] == move->srcPiece) {
				if (flag)
					return 1;
				if (isValidMove(board, move, i, j, move->isSrcWhite))
					return 1;
			}
		}
	}
	return 0;
}

int findKnight(char board[][SIZE], Move *move, int flag) {
	int iMove[] = {1, 2, 1, 2, -1, -2, -1, -2};
	int jMove[] = {2, 1, -2, -1, 2, 1, -2, -1};
	for (int index = 0; index < 8; ++index) {
		int i = move->iDest + iMove[index];
		int j = move->jDest + jMove[index];
		if (i >= SIZE || i < 0 || j >= SIZE || j < 0)
			continue;
		if (board[i][j] == move->srcPiece) {
			if (!flag) {
				if (isValidMove(board, move, i, j, move->isSrcWhite))
					return 1;
			} else
				return 1;
		}
	}
	return 0;
}


void makeStep(char (*board)[8], Move *move) {
	board[move->iSrc][move->jSrc] = EMPTY;
	board[move->iDest][move->jDest] = move->srcPiece;
}


int findPiece(char board[][SIZE], Move *move, int flag) {
	char piece = move->srcPiece;
	if (isPawn(piece))
		return findPawn(board, move, flag) && move->isDestWhite != checkColor(piece);
	if (isRook(piece))
		return findRook(board, move, flag);
	if (isBishop(piece))
		return findBishop(board, move, flag);
	if (isQueen(piece))
		return findQueen(board, move, flag);
	if (isKing(piece))
		return findKing(board, move, flag);
	if (isKnight(piece))
		return findKnight(board, move, flag);
	return 0;
}

int isClearRow(char board[][SIZE], Move *move, int i, int j, int sign) {
	i += sign;
	while (i != move->iDest) {
		if (board[i][j] != EMPTY)
			return 0;
		i += sign;
	}
	return 1;
}

int isClearCol(char board[][SIZE], Move *move, int i, int j, int sign) {
	j += sign;
	while (j != move->jDest) {
		if (board[i][j] != EMPTY)
			return 0;
		j += sign;
	}
	return 1;
}

int isClearDiag(char board[][SIZE], Move *move, int i, int j, int iSign, int jSign) {
	i += iSign;
	j += jSign;
	while (i != move->jDest && j != move->jDest) {
		if (board[i][j] != EMPTY)
			return 0;
		i += iSign;
		j += jSign;
	}
	return 1;
}

int isClearKnight(int i, int j, Move *move) {
	if (abs(move->iDest - i) == 2 && abs(move->jDest - j) == 1)
		return 1;
	if (abs(move->iDest - i) == 1 && abs(move->jDest - j) == 2)
		return 1;
	return 0;
}

int canMove(char board[][SIZE], Move *move, int iSrc, int jSrc) {
	char piece = move->srcPiece;
	int iSign = move->iDest - iSrc > 0 ? 1 : -1;
	int jSign = move->jDest - jSrc > 0 ? 1 : -1;
	if (isRook(move->srcPiece))
		return isClearRow(board, move, iSrc, jSrc, iSign) || isClearCol(board, move, iSrc, jSrc, jSign);
	if (isBishop(piece))
		return isClearDiag(board, move, iSrc, jSrc, iSign, jSign);
	if (isQueen(piece))
		return isClearRow(board, move, iSrc, jSrc, iSign) || isClearCol(board, move, iSrc, jSrc, jSign) ||
		       isClearDiag(board, move, iSrc, jSrc, iSign, jSign);
	if (isKnight(piece))
		return isClearKnight(iSrc, jSrc, move);
	return 0;
}

void updateMove(char board[][SIZE], Move *move) {
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
				findPiece(board, move, 0);
		} else
			isValidMove(board, move, move->iSrc, move->jSrc, move->isSrcWhite);
		if (move->isLegal)
			makeStep(board, move);
	}
}

int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	Move move = {};
	move.isSrcWhite = isWhiteTurn;
	int x = parseMove(pgn, &move);
	updateMove(board, &move);
//	printMove(&move);
	return move.isLegal;

}