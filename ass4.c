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
	return isdigit(position) ? SIZE - toDigit(position) : (int) fmax(position - 'a', 0);

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


int findPawn(char board[][SIZE], Move *move, int i, int j, char piece) {
	int pawnMove = (int) pow(-1, move->isSrcWhite);  //return -1 if pawn is black and 1 if is white
	if (move->isCapture) {
		if (move->destPiece != EMPTY) {
			if (board[i - pawnMove][j - pawnMove] == piece) {
				move->iSrc = i - pawnMove;
				move->jSrc = j - pawnMove;
				return 1;
			}
			if (board[i - pawnMove][j + pawnMove] == piece) {
				move->iSrc = i - pawnMove;
				move->jSrc = j + pawnMove;
				return 1;
			}
			return 0;
		}
		return 0;
	}
	if (move->destPiece == EMPTY) {
		if (board[i - pawnMove][j] == piece) {
			move->iSrc = i - pawnMove;
			move->jSrc = j;
			return 1;
		}
		if ((move->isSrcWhite && i == 4) || (!move->isSrcWhite && i == 3))
			if (board[i - 2 * pawnMove][j] == piece) {
				move->iSrc = i - 2 * pawnMove;
				move->jSrc = j;
				return 1;
			}
	}
	return 0;
}

int checkColumn(char board[][SIZE], Move *move, int i, int j, char piece) {
	for (int k = i - 1; k >= 0; k--) {
		if (board[k][j] == EMPTY)
			continue;
		if (piece == board[k][j]) {
			move->iSrc = k;
			move->jSrc = j;
			return 1;
		}
		break;
	}
	for (int k = i + 1; k < SIZE; k++) {
		if (board[k][j] == EMPTY)
			continue;
		if (piece == board[k][j]) {
			move->iSrc = k;
			move->jSrc = j;
			return 1;
		}
		break;
	}
	return 0;
}

int setSrcIndex(Move *move, int i, int j) {
	move->iSrc = i;
	move->jSrc = j;
	return 1;
}

int checkRow(char board[][SIZE], Move *move, int i, int j, char piece) {
	for (int k = j - 1; k >= 0; k--) {
		if (board[i][k] == EMPTY)
			continue;
		if (piece == board[i][k]) {
			return setSrcIndex(move, i, k);
		}
		break;
	}
	for (int k = j + 1; k < SIZE; k++) {
		if (board[i][k] == EMPTY)
			continue;
		if (piece == board[i][k]) {
			return setSrcIndex(move, i, k);
		}
		break;
	}
	return 0;
}

int findRook(char board[][SIZE], Move *move, int i, int j, char piece, char dest) {
	if (dest == EMPTY) {
		if (!move->isCapture)
			if (checkColumn(board, move, i, j, piece) || checkRow(board, move, i, j, piece))
				return 1;
		return 0;
	}
	if (!move->isDestWhite != !move->isSrcWhite && move->isCapture)
		if (checkColumn(board, move, i, j, piece) || checkRow(board, move, i, j, piece))
			return 1;
	return 0;
}

int checkMainDiagonal(char board[][SIZE], Move *move, int i, int j, char piece) {
	int k, l;
	for (k = i - 1, l = j - 1; k >= 0 && l >= 0; k--, l--) {
		if (board[k][l] == EMPTY)
			continue;
		if (piece == board[k][l]) {
			return setSrcIndex(move, k, l);
		}
		break;
	}
	for (k = i + 1, l = j + 1; k < SIZE && l < SIZE; k++, l++) {
		if (board[k][l] == EMPTY)
			continue;
		if (piece == board[k][l]) {
			return setSrcIndex(move, k, l);
		}
		break;
	}
	return 0;
}

int checkSecDiagonal(char board[][SIZE], Move *move, int i, int j, char piece) {
	int k, l;
	for (k = i + 1, l = j - 1; k < SIZE && l >= 0; k++, l--) {
		if (board[k][l] == EMPTY)
			continue;
		if (piece == board[k][l]) {
			return setSrcIndex(move, k, l);
		}
		break;
	}
	for (k = i - 1, l = j + 1; k >= 0 && l < SIZE; k--, l++) {
		if (board[k][l] == EMPTY)
			continue;
		if (piece == board[k][l]) {
			return setSrcIndex(move, k, l);
		}
		break;
	}
	return 0;
}

int findBishop(char board[][SIZE], Move *move, int i, int j, char piece, char dest) {
	if (dest == EMPTY) {
		if (!move->isCapture)
			if (checkMainDiagonal(board, move, i, j, piece) || checkSecDiagonal(board, move, i, j, piece))
				return 1;
		return 0;
	}
	if (!move->isDestWhite != !move->isSrcWhite && move->isCapture)
		if (checkMainDiagonal(board, move, i, j, piece) || checkSecDiagonal(board, move, i, j, piece))
			return 1;
	return 0;
}

int checkAll(char board[][SIZE], Move *move, int i, int j, char piece) {
	return checkColumn(board, move, i, j, piece) || checkRow(board, move, i, j, piece) ||
	       checkMainDiagonal(board, move, i, j, piece) ||
	       checkSecDiagonal(board, move, i, j, piece);
}

int findQueen(char board[][SIZE], Move *move, int i, int j, char piece, char dest) {
	if (dest == EMPTY) {
		if (!move->isCapture)
			if (checkAll(board, move, i, j, piece))
				return 1;
		return 0;
	}
	if (!move->isDestWhite != !move->isSrcWhite && move->isCapture)
		if (checkAll(board, move, i, j, piece))
			return 1;
	return 0;
}

int checkKing(char board[][SIZE], Move *move, int i, int j, char piece) {
	for (int k = i - 1; k >= 0 && k <= i + 1; k++) {
		for (int l = j - 1; l >= 0 && l <= j + 1; l++) {
			if (board[k][l] == piece)
				return setSrcIndex(move, k, l);
		}
	}
	return 0;
}

int findKing(char board[][SIZE], Move *move, int i, int j, char piece, char dest) {
	if (dest == EMPTY) {
		if (!move->isCapture)
			if (checkKing(board, move, i, j, piece))
				return 1;
		return 0;
	}
	if (!move->isDestWhite != !move->isSrcWhite && move->isCapture)
		if (checkKing(board, move, i, j, piece))
			return 1;
	return 0;
}

int checkKnight(char board[][SIZE], Move *move, int i, int j, char piece) {
	if (i - 2 >= 0 && j - 2 >= 0) {
		if (board[i - 2][j - 1] == piece)
			return setSrcIndex(move, i - 2, j - 1);
		if (board[i - 1][j - 2] == piece)
			return setSrcIndex(move, i - 1, j - 2);
	}
	if (i - 2 >= 0 && j + 2 < SIZE) {
		if (board[i - 2][j + 1] == piece)
			return setSrcIndex(move, i - 2, j + 1);
		if (board[i - 1][j + 2] == piece)
			return setSrcIndex(move, i - 1, j + 2);
	}
	if (i + 2 <SIZE && j + 2 <SIZE) {
		if (board[i + 2][j + 1] == piece)
			return setSrcIndex(move, i + 2, j + 1);
		if (board[i + 1][j + 2] == piece)
			return setSrcIndex(move, i + 1, j + 2);
	}
	if (i + 2 < SIZE && j - 2 >= 0) {
		if (board[i + 2][j - 1] == piece)
			return setSrcIndex(move, i + 2, j - 1);
		if (board[i + 1][j - 2] == piece)
			return setSrcIndex(move, i + 1, j - 2);
	}
	return 0;
}

int findKnight(char board[][SIZE], Move *move, int i, int j, char piece, char dest) {
	if (dest == EMPTY) {
		if (!move->isCapture)
			if (checkKnight(board, move, i, j, piece))
				return 1;
		return 0;
	}
	if (!move->isDestWhite != !move->isSrcWhite && move->isCapture)
		if (checkKnight(board, move, i, j, piece))
			return 1;
	return 0;
}

int findPiecePos(char board[][SIZE], Move *move, char toLook) {
	int i = move->iDest;
	int j = move->jDest;
	char piece = move->srcPiece;
	char dest = move->destPiece;
	if (toLook) {
		if (isdigit(toLook)) {
			//we have the row
		} else {
			//we have the column
		}
	} else {
		//we have neither
		if (piece == WHITE_PAWN || piece == BLACK_PAWN)
			return findPawn(board, move, i, j, piece);
		if (piece == WHITE_ROOK || piece == BLACK_ROOK)
			return findRook(board, move, i, j, piece, dest);
		if (piece == BLACK_BISHOP || piece == WHITE_BISHOP)
			return findBishop(board, move, i, j, piece, dest);
		if (piece == WHITE_QUEEN || piece == BLACK_QUEEN)
			return findQueen(board, move, i, j, piece, dest);
		if (piece == WHITE_KING || piece == BLACK_KING)
			return findKing(board, move, i, j, piece, dest);
		if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT) {
			return findKnight(board, move, i, j, piece, dest);
		}
		return 0;
	}

}

void updateMove(char board[][SIZE], Move *move) {
	move->destPiece = board[move->iDest][move->jDest];
	move->isDestWhite = isupper(move->destPiece);
	char toLook = 0;
	if (!(move->srcRow && move->srcCol)) {
		if (move->srcRow)
			toLook = move->srcRow;

		else if (move->srcRow)
			toLook = move->srcCol;

		move->isLegal = findPiecePos(board, move, toLook);
		if (move->isLegal) {
			board[move->iSrc][move->jSrc] = EMPTY;
			board[move->iDest][move->jDest] = move->srcPiece;
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
	}
	else if (parseMove(pgn, &move) == 3) {
		board[0][4] = EMPTY;
		board[0][0] = EMPTY;
		board[0][2] = BLACK_KING;
		board[0][3] = BLACK_ROOK;
	}
	updateMove(board, &move);
	return move.isLegal;

}