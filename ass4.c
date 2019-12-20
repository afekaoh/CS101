/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass04
 *******************/

#include <string.h>
#include <stdio.h>
#include "ass4.h"
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

//number of the different pieces in chess
#define NUM_OF_PIECES 6

//some const deceleration for later use

// FEN & Board characters
const char EMPTY = ' ';
const char CAPTURE = 'x';
const char PROMOTION = '=';
const char CHECK = '+';
const char MATE = '#';
const char FIRST_COL = 'A';

//pieces name characters
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
char PIECES[NUM_OF_PIECES] = {'P', 'R', 'N', 'B', 'Q', 'K'};

// FEN separator for strtok()
char const DELIM[] = "/";


typedef enum {
	//enum declaration for better readability
			FALSE, TRUE
} Boolean;

// Move logical representation
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, toPromote;
	int iSrc, jSrc, iDest, jDest;
	Boolean isSrcWhite, isDestWhite, isCapture, isPromotion, isCheck, isLegal;
} Move;

void makeStep(char [][SIZE], Move *);

Boolean findPiece(char board[][SIZE], Move *move, int checkCheck);

int toDigit(char digit) {
	//function that gets character of digit and return its value
	assert('0' <= digit && digit <= '9');
	return digit - '0';
}

/**********************************************************************************
* Function Name: creatRow
* Input: char boardRow[] - a row in the board
 * 		 char *fenRow - a pointer to string which is a row in the FEN
* Output: None
* Function Operation: the function reads the string and converts him to the row in the
 * 					array char by char and adding spaces if needed
**********************************************************************************/
void creatRow(char boardRow[], char *fenRow) {
	int i = 0;
	while (*fenRow) {
		int spaces = 0;
		if (isdigit(*fenRow)) {
			spaces = toDigit(*fenRow);
			while (spaces) {
				boardRow[i++] = EMPTY;
				spaces--;
			}
		} else
			boardRow[i++] = *fenRow;
		fenRow++;
	}
}

/**********************************************************************************
* Function Name: creatRow
* Input: char boardRow[][] - 2D in the order of SIZExSIZE which represent a chess board
 * 		 char fen[] - a string which represent a state of the board
* Output: None
* Function Operation: the function reads the string and converts him to the board
 * 					row by row by using create row function
**********************************************************************************/
void createBoard(char board[][SIZE], char fen[]) {
	int i = 0;
	char *fenRow = strtok(fen, DELIM);
	while (fenRow != NULL) {
		creatRow(board[i++], fenRow);
		fenRow = strtok(NULL, DELIM);
	}

}


void printColumns() {
	//prints the first and last rows in the board which shows the columns letter in the board
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
	//prints separators for styling purposes
	printf("* -");
	for (int i = 0; i < SIZE; i++) {
		printf("--");
	}
	printf(" *\n");
}

void printRow(char row[], int rowIdx) {
	//prints a row in the board and its number
	printf("%d ", rowIdx);
	for (int i = 0; i < SIZE; i++) {
		{
			printf("|%c", row[i]);
		}
	}
	printf("| %d\n", rowIdx);
}

/**********************************************************************************
* Function Name: printBoard
* Input: char boardRow[][] - 2D in the order of SIZExSIZE which represent a chess board
* Output: None
* Function Operation: printing the board row by row
**********************************************************************************/
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
	/*the function gets a piece and a color (1 for White and 0 for Black)
	and returns the piece in the same color (big letter for White and small for Black)
	 */
	if (isWhite)
		return piece;
	else
		return tolower(piece);
}

/**********************************************************************************
* Function Name: parseFlags
* Input: Move *move - a pointer to Move struct
 * 		*index - pointer to a string of PGN
* Output: None
* Function Operation: the function reads the string and turnings on the
corresponding flags in the Move struct
**********************************************************************************/
void parseFlags(Move *move, char *index) {
	while (*index) {
		if (*index == CAPTURE)
			move->isCapture = TRUE;
		if (*index == CHECK || *index == MATE)
			move->isCheck = TRUE;
		if (*index == PROMOTION) {
			move->isPromotion = TRUE;
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

/**********************************************************************************
* Function Name: setDest
* Input: Move *move - a pointer to Move struct
 * 		char *index - pointer to a string of PGN
* Output: char *
* Function Operation: the function reads the string and puts the data in the Move struct
**********************************************************************************/
char *setDest(Move *move, char *index) {
	while (!isdigit(*index))    //if we reading from the end we bound to gets a number representing the dest row
		index--;
	index--;
	move->destCol = index[0];
	move->destRow = index[1];   //the char before the row number is the dest column
	return --index;     //returning the position of the previous element
}

void parseMove(char pgn[], Move *move) {
	//the function gets a pgn string and a pointer to Move struct parse the pgn and puting the data in the Move struct
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
	//the function gets a pointer to Move struct and checking the validity of the move it represent
	if (move->isCapture) {
		if (move->destPiece != EMPTY)
			if (!move->isDestWhite != !move->isSrcWhite)
				//false if both the src and dest is the same color and true if they are different
				return TRUE;
		return FALSE;
	}
	if (move->destPiece == EMPTY)   //and isCapture is false
		return TRUE;
	return FALSE;
}

Boolean isPawn(char piece) {
	//return if the piece is Pawn
	return piece == WHITE_PAWN || piece == BLACK_PAWN;
}

Boolean isRook(char piece) {
	//return if the piece is Rook
	return piece == WHITE_ROOK || piece == BLACK_ROOK;
}

Boolean isBishop(char piece) {
	//return if the piece is Bishop
	return piece == BLACK_BISHOP || piece == WHITE_BISHOP;
}

Boolean isQueen(char piece) {
	//return if the piece is Queen
	return piece == WHITE_QUEEN || piece == BLACK_QUEEN;
}

Boolean isKing(char piece) {
	//return if the piece is King
	return piece == WHITE_KING || piece == BLACK_KING;
}

Boolean isKnight(char piece) {
	//return if the piece is Knight
	return piece == WHITE_KNIGHT || piece == BLACK_KNIGHT;
}

/**********************************************************************************
* Function Name: setDest
* Input: Move *move - a pointer to Move struct
 * 		int row - destenation row location
 * 		int column - dest coloumn location
* Output: None
* Function Operation: the function reads the string and puts the
**********************************************************************************/
Boolean setMove(Move *move, int row, int column, Boolean legal) {
	//todo
	if (move->isPromotion)
		move->srcPiece = setColor(move->toPromote, move->isSrcWhite);
	move->iSrc = row;
	move->jSrc = column;
	move->isLegal = legal;
	return TRUE;
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

Boolean isCheck(char tempBoard[][SIZE], int kingColor, Move *move) {
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
						return TRUE;
				if (move->jDest < SIZE - 1)
					if (tempBoard[move->iDest + pawnMove][move->jDest + 1] == setColor(WHITE_PAWN, !kingColor))
						return TRUE;
			}
		} else if (findPiece(tempBoard, move, TRUE))
			return TRUE;
	}
	return FALSE;
}

void makeTempMove(char const board[][SIZE], Move const *move, int i, int j, Move *tempMove, char tempBoard[][SIZE]) {
	//todo
	tempMove->srcPiece = move->srcPiece;
	setMove(tempMove, i, j, FALSE);
	tempMove->iDest = move->iDest;
	tempMove->jDest = move->jDest;
	tempMove->isDestWhite = move->isSrcWhite;
	copyBoard(board, tempBoard);
	makeStep(tempBoard, tempMove);
}

Boolean isValidMove(char board[][SIZE], Move *move, int i, int j) {
	//todo
	Move tempMove = {};
	char tempBoard[SIZE][SIZE];
	int kingColor = move->isSrcWhite;
	makeTempMove(board, move, i, j, &tempMove, tempBoard);

	if (isCheck(tempBoard, kingColor, &tempMove))
		//if there is check on the played side
		return FALSE;

	if (isCheck(tempBoard, !kingColor, &tempMove))
		//if there is check on the opponent side
		if (!move->isCheck)
			return FALSE;

	return setMove(move, i, j, TRUE);
}

Boolean findPawn(char board[][SIZE], Move *move) {
	//todo
	int pawnMove = (int) pow(-1, move->isSrcWhite);
	int i = move->iDest;
	int j = move->jDest;
	char piece = move->srcPiece;
	if (move->isCapture) {
		if (board[i][j] != EMPTY) {
			if (board[i - pawnMove][move->jSrc] == piece) {
				move->isLegal = isValidMove(board, move, i - pawnMove, move->jSrc);
				return TRUE;
			}
		}
		return FALSE;
	}
	if (board[i - pawnMove][j] == piece) {
		move->isLegal = isValidMove(board, move, i - pawnMove, j);
		return TRUE;
	}
	if ((move->isSrcWhite && (board[SIZE - 2][j] == piece)) || (!move->isSrcWhite && (board[1][j] == piece)))
		if (board[i - pawnMove][j] == EMPTY) {
			move->isLegal = isValidMove(board, move, i - (2 * pawnMove), j);
			return TRUE;
		}
	return FALSE;
}

Boolean findInCol(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	for (int i = move->iDest - 1; i >= 0; --i) {
		if (board[i][move->jDest] == EMPTY)
			continue;
		if (board[i][move->jDest] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, i, move->jDest))
				return TRUE;
		}
		break;
	}
	for (int i = move->iDest + 1; i < SIZE; ++i) {
		if (board[i][move->jDest] == EMPTY)
			continue;
		if (board[i][move->jDest] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, i, move->jDest))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

Boolean findInRow(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	for (int j = move->jDest - 1; j >= 0; j--) {
		if (board[move->iDest][j] == EMPTY)
			continue;
		if (board[move->iDest][j] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, move->iDest, j))
				return TRUE;
		}
		break;
	}
	for (int j = move->jDest + 1; j < SIZE; ++j) {
		if (board[move->iDest][j] == EMPTY)
			continue;
		if (board[move->iDest][j] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, move->iDest, j))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

Boolean findRook(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	if (findInCol(board, move, checkCheck))
		return TRUE;
	if (findInRow(board, move, checkCheck))
		return TRUE;
	return FALSE;
}

Boolean findInSecDiag(char board[][SIZE], Move *move, int checkCheck) {
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
				return TRUE;
			if (isValidMove(board, move, i, j))
				return TRUE;
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
				return TRUE;
			if (isValidMove(board, move, i, j))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

Boolean findInMainDiag(char board[][SIZE], Move *move, int checkCheck) {
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
				return TRUE;
			if (isValidMove(board, move, i, j))
				return TRUE;
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
				return TRUE;
			if (isValidMove(board, move, i, j))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

Boolean findBishop(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	if (findInMainDiag(board, move, checkCheck))
		return TRUE;
	if (findInSecDiag(board, move, checkCheck))
		return TRUE;
	return FALSE;
}

Boolean findQueen(char board[][SIZE], Move *move, int checkCheck) {
	//todo
	if (findInRow(board, move, checkCheck))
		return TRUE;
	if (findInCol(board, move, checkCheck))
		return TRUE;
	if (findInMainDiag(board, move, checkCheck))
		return TRUE;
	if (findInSecDiag(board, move, checkCheck))
		return TRUE;
	return FALSE;
}

Boolean findKing(char board[][SIZE], Move *move, int checkCheck) {
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
					return TRUE;
				if (isValidMove(board, move, i, j))
					return TRUE;
			}
		}
	}
	return FALSE;
}

Boolean findKnight(char board[][SIZE], Move *move, int checkCheck) {
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
				if (isValidMove(board, move, i, j))
					return TRUE;
			} else
				return TRUE;
		}
	}
	return FALSE;
}

void makeStep(char board[][SIZE], Move *move) {
	//todo
	board[move->iSrc][move->jSrc] = EMPTY;
	board[move->iDest][move->jDest] = move->srcPiece;
}

Boolean findPiece(char board[][SIZE], Move *move, int checkCheck) {
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
	return FALSE;
}

Boolean isClearCol(char board[][SIZE], Move *move, int i, int j, int sign) {
	//todo
	i += sign;
	while (i != move->iDest) {
		if (board[i][j] != EMPTY)
			return FALSE;
		i += sign;
	}
	if (i == move->iDest && j == move->jDest)
		return TRUE;
	return FALSE;
}

Boolean isClearRow(char board[][SIZE], Move *move, int i, int j, int sign) {
	//todo
	j += sign;
	while (j != move->jDest) {
		if (board[i][j] != EMPTY)
			return FALSE;
		j += sign;
	}
	if (j == move->jDest && i == move->iDest)
		return TRUE;
	return FALSE;
}

Boolean isClearDiag(char board[][SIZE], Move *move, int i, int j, int iSign, int jSign) {
	//todo
	i += iSign;
	j += jSign;
	while ((j != move->jDest) && (i != move->iDest)) {
		if (board[i][j] != EMPTY)
			return FALSE;
		i += iSign;
		j += jSign;
	}
	if (i == move->iDest && j == move->jDest)
		return TRUE;
	return FALSE;
}

Boolean isClearKnight(int i, int j, Move *move) {
	//todo
	if (abs(move->iDest - i) == 2 && abs(move->jDest - j) == 1)
		return TRUE;
	if (abs(move->iDest - i) == 1 && abs(move->jDest - j) == 2)
		return TRUE;
	return FALSE;
}

Boolean canMove(char board[][SIZE], Move *move, int iSrc, int jSrc) {
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
	return FALSE;
}

void updateMove(char board[][SIZE], Move *move) {
	//todo
	Boolean checkCheck = FALSE;
	move->destPiece = board[move->iDest][move->jDest];
	move->isDestWhite = isupper(move->destPiece);
	char piece = move->srcPiece;
	if (canStep(move)) {
		if (!(move->srcRow && move->srcCol)) {
			if (move->srcRow && !isPawn(piece)) {
				for (int j = 0; j < SIZE; ++j) {
					if (board[move->iSrc][j] == piece)
						if (canMove(board, move, move->iSrc, j))
							if (isValidMove(board, move, move->iSrc, j))
								break;
				}
			} else if (move->srcCol && !isPawn(piece)) {
				for (int i = 0; i < SIZE; ++i) {
					if (board[i][move->jSrc] == piece)
						if (canMove(board, move, i, move->jSrc))
							if (isValidMove(board, move, i, move->jSrc))
								break;
				}
			} else
				findPiece(board, move, checkCheck);
		} else
			isValidMove(board, move, move->iSrc, move->jSrc);
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