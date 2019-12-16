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
	int isSrcWhite, isDestWhite, isCapture, isPromotion, isCheck, isMate, isLegal;
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
			move->toPromote = pieceColor(*(index + 1), move->isSrcWhite);
		}
		index++;
		counter++;
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
	if (strcasecmp(pgn, "aaa") == 0)
		return 2;
	if (strcasecmp(pgn, "bbb") == 0)
		return 3;
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
		move->srcPiece = pieceColor(pgn[0], move->isSrcWhite);
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
		move->srcPiece = pieceColor(WHITE_PAWN, move->isSrcWhite);
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

char *findPawn(char board[][SIZE], Move *move, char piece) {
	int pawnMove = (int) pow(-1, move->isSrcWhite);  //return -1 if pawn is black and 1 if is white
	int i = move->iDest;
	int j = move->jDest;
	if (move->isCapture) {
		if (board[i][j] != EMPTY) {
			if (!move->isDestWhite != !move->isSrcWhite)
				if (board[i - pawnMove][move->jSrc] == piece){
					move->isLegal=1;
					return &(board[i - pawnMove][move->jSrc]);
				}
			return NULL;
		}
		return NULL;
	}
	if (board[i - pawnMove][j] == piece) {
		return &(board[i - pawnMove][j]);
	}
	if (board[i - pawnMove][j] != EMPTY)
		return NULL;
	if ((move->isSrcWhite && i == 4) || (!move->isSrcWhite && i == 3))
		if (board[i - 2 * pawnMove][j] == piece) {
			move->isLegal=1;
			return &(board[i - 2 * pawnMove][j]);
		}
	return NULL;
}

char *checkColumn(char *dest, char const *start, char const *end, char piece) {
	char *check = dest - SIZE;
	while (check >= start) {
		if (*check == EMPTY) {
			check -= SIZE;
			continue;
		}
		if (piece == *check) {
			return check;
		}
		break;
	}
	check = dest + SIZE;
	while (check <= end) {
		if (*check == EMPTY) {
			check += SIZE;
			continue;
		}
		if (piece == *check) {
			return check;
		}
		break;
	}
	return NULL;
}

int setSrcIndex(Move *move, int i, int j) {
	move->iSrc = i;
	move->jSrc = j;
	return 1;
}

char *checkRow(char *dest, const char *start, const char *end, char piece) {
	char *tempPos = dest - 1;
	while (tempPos >= start) {
		if (*tempPos == EMPTY) {
			tempPos--;
			continue;
		}
		if (piece == *tempPos)
			return tempPos;
		break;
	}
	tempPos = dest + 1;
	while (tempPos <= end) {
		if (*tempPos == EMPTY) {
			tempPos++;
			continue;
		}
		if (piece == *tempPos)
			return tempPos;
		break;
	}
	return NULL;
}

char *findRook(char board[][SIZE], char *dest, Move *move, char piece) {
	char *src = NULL;
	if ((move->isLegal = canStep(move))) {
		src = checkColumn(dest, &board[0][move->jDest], &board[SIZE][move->jDest], piece);
		if (src == NULL)
			src = checkRow(dest, &board[move->iDest][0], &board[move->iDest][SIZE], piece);

	}
	return src;
}

char *checkMainDiagonal(char *dest, const char *start, const char *end, char piece) {
	char *check = ((dest - SIZE) - 1);
	while (check >= start) {
		if (*check == EMPTY) {
			check = ((check - SIZE) - 1);
			continue;
		}
		if (piece == *check) {
			return check;
		}
		break;
	}
	check = ((dest + SIZE) + 1);
	while (check <= end) {
		if (*check == EMPTY) {
			check = ((check + SIZE) + 1);
			continue;
		}
		if (piece == *check) {
			return check;
		}
		break;
	}
	return NULL;
}

char *checkSecDiagonal(char *dest, const char *start, const char *end, char piece) {
	char *check = ((dest + SIZE) - 1);
	while (check >= start) {
		if (*check == EMPTY) {
			check = ((check + SIZE) - 1);
			continue;
		}
		if (piece == *check) {
			return check;
		}
		break;
	}
	check = ((dest - SIZE) + 1);
	while (check <= end) {
		if (*check == EMPTY) {
			check = ((check - SIZE) + 1);
			continue;
		}
		if (piece == *check) {
			return check;
		}
		break;
	}
	return NULL;
}

char *findBishop(char board[][SIZE], char *dest, Move *move, char piece) {
	char *src = NULL;
	if ((move->isLegal = canStep(move))) {
		src = checkMainDiagonal(dest, &board[0][0], &board[SIZE - 1][SIZE - 1], piece);
		if (src == NULL)
			src = checkSecDiagonal(dest, &board[0][0], &board[SIZE - 1][SIZE - 1], piece);
	}
	return src;
}

char *
checkAll(char board[][SIZE], Move *move, int leftBorder, int rightBorder, int topBorder, int bottomBorder, char piece) {
	char *src = NULL;
	char *dest = &board[move->iDest][move->jDest];
	src = checkColumn(dest, &board[topBorder][move->jDest], &board[bottomBorder][move->jDest], piece);
	if (src == NULL)
		src = checkRow(dest, &board[move->iDest][leftBorder], &board[move->iDest][rightBorder], piece);
	if (src == NULL)
		src = checkMainDiagonal(dest, &board[topBorder][leftBorder], &board[bottomBorder][rightBorder], piece);
	if (src == NULL)
		src = checkSecDiagonal(dest, &board[bottomBorder][leftBorder], &board[topBorder][rightBorder], piece);
	return src;
}

char *findQueen(char board[][SIZE], Move *move, int leftBorder, int rightBorder, int topBorder, int bottomBorder,
                char piece) {
	if ((move->isLegal = canStep(move)))
		return checkAll(board, move, leftBorder, rightBorder, topBorder, bottomBorder, piece);
}

char *findKing(char board[][SIZE], Move *move, char piece) {
	int top = (int) fmax(0, move->iDest - 1);
	int bottom = (int) fmin(SIZE - 1, move->iDest + 1);
	int right = (int) fmin(SIZE - 1, move->jDest + 1);
	int left = (int) fmax(0, move->jDest - 1);
	if ((move->isLegal = canStep(move)))
		return checkAll(board, move, left, right, top, bottom, piece);
}

int isValidKnight(const char *src, const char *dest) {
	if ((src + (2 * SIZE) + 1) == dest)
		return 1;
	if ((src + (2 * SIZE) - 1) == dest)
		return 1;
	if ((src + (SIZE) + 2) == dest)
		return 1;
	if ((src + (SIZE) - 2) == dest)
		return 1;
	if ((src - (2 * SIZE) + 1) == dest)
		return 1;
	if ((src - (2 * SIZE) + 1) == dest)
		return 1;
	if ((src - (SIZE) + 2) == dest)
		return 1;
	if ((src - (SIZE) - 2) == dest)
		return 1;
	return 0;
}

char *checkKnight(char *dest, Move *move, char piece) {
	int horizontalBoundary = move->iDest;
	int verticalBorder = move->jDest;
	if (horizontalBoundary - 2 >= 0 && verticalBorder - 2 >= 0) {
		if (*(dest - (SIZE) - 2) == piece)
			return dest - (SIZE) - 2;
		if (*(dest - (2 * SIZE) - 1) == piece)
			return dest - (2 * SIZE) - 1;
	}
	if (horizontalBoundary - 2 >= 0 && verticalBorder + 2 < SIZE) {
		if (*(dest - (2 * SIZE) + 1) == piece)
			return dest - (2 * SIZE) - 1;
		if (*(dest - (SIZE) + 2) == piece)
			return dest - (SIZE) + 2;
	}
	if (horizontalBoundary + 2 < SIZE && verticalBorder + 2 < SIZE) {
		if (*(dest + (2 * SIZE) + 1) == piece)
			return dest + (2 * SIZE) + 1;
		if (*(dest + (SIZE) + 2) == piece)
			return dest + (SIZE) + 2;
	}
	if (horizontalBoundary + 2 < SIZE && verticalBorder - 2 >= 0) {
		if (*(dest + (2 * SIZE) - 1) == piece)
			return dest + (2 * SIZE) - 1;
		if (*(dest + (SIZE) - 2) == piece)
			return dest + (SIZE) - 2;
	}
	return 0;
}

char *findKnight(char board[][SIZE], Move *move, char piece) {
	char *dest = &board[move->iDest][move->jDest];
	if ((move->isLegal = canStep(move)))
		return checkKnight(dest, move, piece);
}

char *findPiece(char board[][SIZE], char *dest, Move *move) {
	char piece = move->srcPiece;
	if (piece == WHITE_PAWN || piece == BLACK_PAWN)
		return findPawn(board, move, piece);
	if (piece == WHITE_ROOK || piece == BLACK_ROOK)
		return findRook(board, dest, move, piece);
	if (piece == BLACK_BISHOP || piece == WHITE_BISHOP)
		return findBishop(board, dest, move, piece);
	if (piece == WHITE_QUEEN || piece == BLACK_QUEEN)
		return findQueen(board, move, 0, SIZE - 1, 0, SIZE - 1, piece);
	if (piece == WHITE_KING || piece == BLACK_KING)
		return findKing(board, move, piece);
	if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT) {
		return findKnight(board, move, piece);
	}
	return 0;
}


void makeStep(char *dest, char *src) {
	if (dest != NULL && src != NULL) {
		*dest = *src;
		*src = EMPTY;
	} else
		printf("promblem\n");

}

int isValidMove(char board[][SIZE], int i, int j, char *dest, char piece) {
	char *src = &board[i][j];

	//	if (piece == WHITE_PAWN || piece == BLACK_PAWN)
//		return findPawn(board, move, piece);
//	if (piece == WHITE_ROOK || piece == BLACK_ROOK)
//		return findRook(board, dest, move, piece);
//	if (piece == BLACK_BISHOP || piece == WHITE_BISHOP)
//		return findBishop(board, dest, move, piece);
//	if (piece == WHITE_QUEEN || piece == BLACK_QUEEN)
//		return findQueen(board, move, 0, SIZE - 1, 0, SIZE - 1, piece);
//	if (piece == WHITE_KING || piece == BLACK_KING)
//		return findKing(board, move, piece);
	if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT) {
		return isValidKnight(src, dest);
	}
	return 0;

}

char *findPiecePos(char board[][SIZE], Move *move, char toLook) {
	char *dest = &board[move->iDest][move->jDest];
	char piece = move->srcPiece;
	if (toLook) {
		if (isdigit(toLook)) {
			int k = toIndex(toLook);
		} else {
			int j = toIndex(toLook);
			for (int i = 0; i < SIZE; ++i) {
				if (board[i][j] == piece)
					if (isValidMove(board, i, j, dest, piece)) {
						move->isLegal = 1;
						return &board[i][j];
					}
			}
		}
	} else {
		//we have neither
		return findPiece(board, dest, move);
	}
	return NULL;
}


void updateMove(char board[][SIZE], Move *move) {
	move->destPiece = board[move->iDest][move->jDest];
	move->isDestWhite = isupper(move->destPiece);
	char toLook = 0;
	char *dest = &board[move->iDest][move->jDest];

	if (!(move->srcRow && move->srcCol)) {
		if (move->srcRow)
			toLook = move->srcRow;

		else if (move->srcCol)
			toLook = move->srcCol;

		char *src = findPiecePos(board, move, toLook);
		//		move->srcRow = toRow(move->iSrc);
//		move->srcCol = toCol(move->jSrc);
		if (move->isLegal) {
			makeStep(dest, src);
		}
	}
}


int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	Move move = {};
	move.isSrcWhite = isWhiteTurn;
	if (parseMove(pgn, &move) == 2) {
		board[7][0] = EMPTY;
		board[7][4] = EMPTY;
		board[7][2] = WHITE_KING;
		board[7][3] = WHITE_ROOK;
	} else if (parseMove(pgn, &move) == 3) {
		board[0][4] = EMPTY;
		board[0][0] = EMPTY;
		board[0][2] = BLACK_KING;
		board[0][3] = BLACK_ROOK;
	}
	updateMove(board, &move);
//	printMove(&move);
	return move.isLegal;

}