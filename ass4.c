/*******************
 *Adam Shay Shapira
 *316044809
 *01
 *ass4
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

// PGN & Board characters
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

Boolean findPiece(char board[][SIZE], Move *move, Boolean checkCheck);

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

/**********************************************************************
* Function Name: createBoard
* Input:char board[][SIZE] - 2D array representing a chess board
* 		char fen[] - a string of a chess representing a board state
* Output: None
* Function Operation:the function gets a board and a state
*						 and create a board in this state
***********************************************************************/
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
	printColumns();
	printSpacers();
	
	for (int i = 0; i < SIZE; ++i) {
		printRow(board[i], SIZE - i);
	}
	printSpacers();
	printColumns();
}

char setColor(char piece, int isWhite) {
	/*the function gets a char piece and a color (1 for White and 0 for Black)
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
* Function Operation: the function reads the string with a pointer and turnings on the
*					corresponding flags in the Move struct
**********************************************************************************/
void parseFlags(Move *move, char *index) {
	while (*index) {
		if (*index == CAPTURE)
			move->isCapture = TRUE;
		if (*index == CHECK || *index == MATE)
			//the tow states are identical for this assigment purposes
			move->isCheck = TRUE;
		if (*index == PROMOTION) {
			move->isPromotion = TRUE;
			move->toPromote = setColor(*(index + 1), move->isSrcWhite);
		}
		index++;
	}
}

int toIndex(char position) {
/*
	the function gets a char position and returns its value on the board  if its a row or a column
    if it's not a row or a column it's returning -1
*/
	if (isalnum(position))
		return isdigit(position) ? SIZE - toDigit(position) : position - 'a';
	return -1;
}

/**********************************************************************************
* Function Name: setDest
* Input: Move *move - a pointer to Move struct
 * 		char *index - pointer to a string of PGN
* Output: char *
* Function Operation: the function reads the string and puts the data in the Move struct
**********************************************************************************/
char *setDest(Move *move, char *index) {
	while (!isdigit(*index)) {
		//if we reading from the end we bound to gets a number representing the dest row
		index--;
	}
	//taking another step back
	index--;
	//the char before the row number is the dest column
	move->destCol = index[0];
	move->destRow = index[1];
	//returning the position of the previous element
	return --index;
}

void parseMove(char pgn[], Move *move) {
	//the function gets a pgn string and a pointer to Move struct parse the pgn and putting the data in the Move struct
	int len = (int) strlen(pgn);
	//setting a pointer to the last position of the array
	char *lastPos = &(pgn[len - 1]);
	parseFlags(move, pgn);
	lastPos = setDest(move, lastPos);
	
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
			//could be (Piece)(col)(row) or (Piece)(row)
			move->srcRow = *(lastPos--);    //taking source row and moving the pointer one step back
			if (islower(*lastPos))
				// then it's (Piece)(col)(row)
				move->srcCol = *lastPos;
		}
	} else {
		//the source piece is a pawn
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
	return piece == WHITE_PAWN || piece == BLACK_PAWN;
}

Boolean isRook(char piece) {
	return piece == WHITE_ROOK || piece == BLACK_ROOK;
}

Boolean isBishop(char piece) {
	return piece == BLACK_BISHOP || piece == WHITE_BISHOP;
}

Boolean isQueen(char piece) {
	return piece == WHITE_QUEEN || piece == BLACK_QUEEN;
}

Boolean isKing(char piece) {
	return piece == WHITE_KING || piece == BLACK_KING;
}

Boolean isKnight(char piece) {
	return piece == WHITE_KNIGHT || piece == BLACK_KNIGHT;
}

void makeStep(char board[][SIZE], Move *move) {
	//the function gets a board and a move and makes the step that's in the move
	board[move->iSrc][move->jSrc] = EMPTY;
	board[move->iDest][move->jDest] = move->srcPiece;
}

/**********************************************************************************
* Function Name: setSource
* Input: Move *move - a pointer to Move struct
 * 		int row - destination row location
 * 		int column - dest column location
* Output: None
* Function Operation: the function reads the string and puts the
**********************************************************************************/
void setSource(Move *move, int row, int column) {
	if (move->isPromotion)
		move->srcPiece = setColor(move->toPromote, move->isSrcWhite);
	move->iSrc = row;
	move->jSrc = column;
	move->isLegal = TRUE;
}

void findKingOnBoard(char board[][SIZE], int kingColor, Move *move) {
	/*
	* the function gets a board move and a color the function find the king in the color and
	* assign its position to the move
	*/
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

void setMove(Move const *move, Move *tempMove) {
	//the function gets 2 moves pointers and update the temp from the original
	tempMove->srcPiece = move->srcPiece;
	tempMove->iDest = move->iDest;
	tempMove->jDest = move->jDest;
	tempMove->isDestWhite = move->isSrcWhite;
}

void makeTempMove(char const board[][SIZE], char tempBoard[][SIZE], Move const *move, Move *tempMove,
                  int row, int column) {
	//sets the new board and the new move
	copyBoard(board, tempBoard);
	setMove(move, tempMove);
	setSource(tempMove, row, column);
	//making the step on the new board
	makeStep(tempBoard, tempMove);
}

/*********************************************************************************
* Function Name: isCheck
* Input: char board[][SIZE] - 2D array
* 		 int kingColor - the color of the king (0 for black, 1 for white)
* 		 Move *move - a pointer to a Move struct
* Output: TRUE (1) if there is check or FALSE (0) if not
* Function Operation: the function find if there is a threat to the king (check) by searching for around the king
* 					and calling the findPiece function in the check Check mode which means it's only searching
* 					for an existence of a piece that can reach the king
*********************************************************************************/
Boolean isCheck(char board[][SIZE], int kingColor, Move *move) {
	findKingOnBoard(board, kingColor, move);
	for (int index = 0; index < NUM_OF_PIECES; ++index) {
		move->srcPiece = setColor(PIECES[index], !kingColor);
		if (move->srcPiece == setColor(WHITE_PAWN, !kingColor)) {
			//checking for pawns that can capture the king
			int pawnMove = (int) pow(-1, kingColor);
			if (move->iDest + pawnMove < 0 || move->iDest + pawnMove >= SIZE)
				continue;
			if (move->jDest > 0)
				if (board[move->iDest + pawnMove][move->jDest - 1] == setColor(WHITE_PAWN, !kingColor))
					return TRUE;
			if (move->jDest < SIZE - 1)
				if (board[move->iDest + pawnMove][move->jDest + 1] == setColor(WHITE_PAWN, !kingColor))
					return TRUE;
		} else if (findPiece(board, move, TRUE))
			return TRUE;
	}
	return FALSE;
}

/******************************************************************************************************
* Function Name: isValidMove
* Input: char board[][Size] - 2D array
* 		 Move *move - a pointer to move struct
* 		 int row - the row of the piece we check if it's valid
* 		 int column - the column of the piece we check if it's valid
* Output: Boolean TRUE (1) if it's valid move or FALSE (0) if it's not
* Function Operation: the function checks if a given move is valid meaning it logically sound
* 					and not makes an illegal check to either kings on the board
******************************************************************************************************/
Boolean isValidMove(char board[][SIZE], Move *move, int row, int column) {
	if (!canStep(move))
		// the move was logically wrong
		return FALSE;
	
	Move tempMove = {};
	char tempBoard[SIZE][SIZE];
	int kingColor = move->isSrcWhite;
	makeTempMove(board, tempBoard, move, &tempMove, row, column);
	
	if (isCheck(tempBoard, kingColor, &tempMove))
		//if there is check on the played side after the step
		return FALSE;
	
	if (isCheck(tempBoard, !kingColor, &tempMove))
		//if there is check on the opponent side
		if (!move->isCheck)
			return FALSE;
	setSource(move, row, column);
	return TRUE;
}

/************************************************************************************************************
* Function Name: findPawn
* Input: char board[][Size] - 2D array
* 		 Move *move - a pointer to move struct
* Output: Boolean TRUE (1) if found a pawn and FALSE (0) if didnt
* Function Operation: the function searching in every place that a pawn could be around the destination
************************************************************************************************************/
Boolean findPawn(char board[][SIZE], Move *move) {
	int pawnMove = (int) pow(-1, move->isSrcWhite);
	int row = move->iDest;
	int column = move->jDest;
	char piece = move->srcPiece;
	if (move->isCapture) {
		if (board[row][column] != EMPTY) {
			if (board[row - pawnMove][move->jSrc] == piece) {
				move->isLegal = isValidMove(board, move, row - pawnMove, move->jSrc);
				return TRUE;
			}
		}
		return FALSE;
	}
	if (board[row - pawnMove][column] == piece) {
		move->isLegal = isValidMove(board, move, row - pawnMove, column);
		return TRUE;
	}
	if (board[row - (2 * pawnMove)][column] == piece && row - (2 * pawnMove) == (move->isSrcWhite ? SIZE - 2 : 1))
		//if the pawn is white then he should be in SIZE - 2 row if he's black then in the row 1
		if (board[row - pawnMove][column] == EMPTY) {
			move->isLegal = isValidMove(board, move, row - (2 * pawnMove), column);
			return TRUE;
		}
	return FALSE;
}

/************************************************************************************************************
* Function Name: findInCol
* Input: char board[][Size] - 2D array
* 		 Move *move - a pointer to move struct
* 		 Boolean checkCheck - if we are in checkCheck mode
* Output: Boolean TRUE (1) if found the piece and FALSE (0) if didnt
* Function Operation: the function goes from the destination and search in every row for a given column
* 					  and returning false if the piece searched is the piece can't make the move
* 					  (not there,blocked or not valid)
************************************************************************************************************/
Boolean findInCol(char board[][SIZE], Move *move, Boolean checkCheck) {
	int row = move->iDest - 1;
	while (row >= 0) {
		// searching upward to the destination
		if (board[row][move->jDest] == EMPTY) {
			row--;
			continue;
		}
		if (board[row][move->jDest] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, move->jDest))
				return TRUE;
		}
		break;
	}
	//resetting the row
	row = move->iDest + 1;
	while (row < SIZE) {
		//searching downward to the destination
		if (board[row][move->jDest] == EMPTY) {
			row++;
			continue;
		}
		if (board[row][move->jDest] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, move->jDest))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

/************************************************************************************************************
* Function Name: findInRow
* Input: char board[][Size] - 2D array
* 		 Move *move - a pointer to move struct
* 		 Boolean checkCheck - if we are in checkCheck mode
* Output: Boolean TRUE (1) if found the piece and FALSE (0) if didnt
* Function Operation: the function goes from the destination and search in every column for a given row
* 					  and returning false if the piece searched is the piece can't make the move
* 					  (not there,blocked or not valid)
************************************************************************************************************/
Boolean findInRow(char board[][SIZE], Move *move, Boolean checkCheck) {
	int column = move->jDest - 1;
	while (column >= 0) {
		//searching left to the destination
		if (board[move->iDest][column] == EMPTY) {
			column--;
			continue;
		}
		if (board[move->iDest][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, move->iDest, column))
				return TRUE;
		}
		break;
	}
	
	//resetting the column
	column = move->jDest + 1;
	while (column < SIZE) {
		//searching right to the destination
		if (board[move->iDest][column] == EMPTY) {
			column++;
			continue;
		}
		if (board[move->iDest][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, move->iDest, column))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

Boolean findRook(char board[][SIZE], Move *move, Boolean checkCheck) {
	//the function search in the row and the column of the destination for a rook and returning if found a valid one
	if (findInCol(board, move, checkCheck))
		return TRUE;
	if (findInRow(board, move, checkCheck))
		return TRUE;
	return FALSE;
}

/************************************************************************************************************
* Function Name: findInSecDiag
* Input: char board[][Size] - 2D array
* 		 Move *move - a pointer to move struct
* 		 Boolean checkCheck - if we are in checkCheck mode
* Output: Boolean TRUE (1) if found the piece and FALSE (0) if didnt
* Function Operation: the function goes from the destination and search in the secondary diagonal for the piece
* 					  and returning false if the piece searched is the piece can't make the move
* 					  (not there,blocked or not valid)
************************************************************************************************************/
Boolean findInSecDiag(char board[][SIZE], Move *move, Boolean checkCheck) {
	int row = move->iDest + 1;
	int column = move->jDest - 1;
	while (row < SIZE && column >= 0) {
		//searching downward and to the left
		if (board[row][column] == EMPTY) {
			row++;
			column--;
			continue;
		}
		if (board[row][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, column))
				return TRUE;
		}
		break;
	}
	row = move->iDest - 1;
	column = move->jDest + 1;
	while (row >= 0 && column < SIZE) {
		//searching upward and to the right
		if (board[row][column] == EMPTY) {
			row--;
			column++;
			continue;
		}
		if (board[row][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, column))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

/************************************************************************************************************
* Function Name: findInSecDiag
* Input: char board[][Size] - 2D array
* 		 Move *move - a pointer to move struct
* 		 Boolean checkCheck - if we are in checkCheck mode
* Output: Boolean TRUE (1) if found the piece and FALSE (0) if didnt
* Function Operation: the function goes from the destination and search in the main diagonal for the piece
* 					  and returning false if the piece searched is the piece can't make the move
* 					  (not there,blocked or not valid)
************************************************************************************************************/
Boolean findInMainDiag(char board[][SIZE], Move *move, Boolean checkCheck) {
	int row = move->iDest - 1;
	int column = move->jDest - 1;
	while (row >= 0 && column >= 0) {
		if (board[row][column] == EMPTY) {
			//searching upward and to the left
			row--;
			column--;
			continue;
		}
		if (board[row][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, column))
				return TRUE;
		}
		break;
	}
	row = move->iDest + 1;
	column = move->jDest + 1;
	while (row < SIZE && column < SIZE) {
		//searching downward and to the right
		if (board[row][column] == EMPTY) {
			row++;
			column++;
			continue;
		}
		if (board[row][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, column))
				return TRUE;
		}
		break;
	}
	return FALSE;
}

Boolean findBishop(char board[][SIZE], Move *move, Boolean checkCheck) {
	//the function search in the diagonals of the destination for a bishop and returning if found a valid one
	if (findInMainDiag(board, move, checkCheck))
		return TRUE;
	if (findInSecDiag(board, move, checkCheck))
		return TRUE;
	return FALSE;
}

Boolean findQueen(char board[][SIZE], Move *move, Boolean checkCheck) {
	/*the function search in the the row column and diagonals of the destination
	* for a queen and returning if found a valid one
	*/
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

Boolean findKing(char board[][SIZE], Move *move, Boolean checkCheck) {
	/*the function function searches in the 3X3 square around the destination
	 where the king might be and returning if found a valid one
	 */
	int const SQUARE_SIZE = 3;
	int iMove[] = {-1, 0, 1};
	int jMove[] = {-1, 0, 1};
	for (int i = 0; i < SQUARE_SIZE; i++) {
		for (int j = 0; j < SQUARE_SIZE; j++) {
			if (i == 1 && j == 1)
				//its board[iDest][jDest] which is the destination so we need to skip that
				continue;
			int row = move->iDest + iMove[j];
			int column = move->jDest + jMove[i];
			if (row >= SIZE || row < 0 || column >= SIZE || column < 0)
				continue;
			if (board[row][column] == move->srcPiece) {
				if (checkCheck)
					return TRUE;
				if (isValidMove(board, move, row, column))
					return TRUE;
			}
		}
	}
	return FALSE;
}

Boolean findKnight(char board[][SIZE], Move *move, Boolean checkCheck) {
	/* the function function searches in all the spots on the board that a knight
	 * can be given a destination returning if found a valid one
	 */
	//the number of places that a knight can be from a given destination
	int const NUM_OF_MOVES = 8;
	int iMove[] = {1, 2, 1, 2, -1, -2, -1, -2};
	int jMove[] = {2, 1, -2, -1, 2, 1, -2, -1};
	for (int index = 0; index < NUM_OF_MOVES; ++index) {
		int row = move->iDest + iMove[index];
		int column = move->jDest + jMove[index];
		if (row >= SIZE || row < 0 || column >= SIZE || column < 0)
			continue;
		if (board[row][column] == move->srcPiece) {
			if (checkCheck)
				return TRUE;
			if (isValidMove(board, move, row, column))
				return TRUE;
		}
	}
	return FALSE;
}

/******************************************************************************************************
* Function Name: findPiece
* Input: char board[][SIZE] - a 2D array
* 		 Move *move - a pointer to the move struct
* 		 Boolean checkCheck - if we in the checkCheck mode
* Output: Boolean TRUE (1) if found a valid piece and FALSE (0) if not
* Function Operation: the function search if there is a valid piece that can reach the destination
* 					and returning FALSE in couldn't
******************************************************************************************************/
Boolean findPiece(char board[][SIZE], Move *move, Boolean checkCheck) {
	char piece = move->srcPiece;
	if (isPawn(piece))
		//no need for checkCheck because the logic makes sure it won't enter here in checkCheck mode
		return findPawn(board, move);
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

Boolean isClearCol(char board[][SIZE], Move *move, int iSrc, int jSrc, int Direction) {
	//gets a source and a destination and find if the column between them is clear
	iSrc += Direction;
	while (iSrc != move->iDest) {
		if (board[iSrc][jSrc] != EMPTY)
			return FALSE;
		iSrc += Direction;
	}
	//make sure we got exactly to the destination
	return iSrc == move->iDest && jSrc == move->jDest ? TRUE : FALSE;
}

Boolean isClearRow(char board[][SIZE], Move *move, int iSrc, int jSrc, int Direction) {
	//gets a source and a destination and find if the row between them is clear
	jSrc += Direction;
	while (jSrc != move->jDest) {
		if (board[iSrc][jSrc] != EMPTY)
			return FALSE;
		jSrc += Direction;
	}
	//make sure we got exactly to the destination
	return jSrc == move->jDest && iSrc == move->iDest ? TRUE : FALSE;
}

Boolean isClearDiag(char board[][SIZE], Move *move, int row, int column, int iDirection, int jDirection) {
	//gets a source and a destination and find if the diagonal between them is clear
	row += iDirection;
	column += jDirection;
	while ((column != move->jDest) && (row != move->iDest)) {
		if (board[row][column] != EMPTY)
			return FALSE;
		row += iDirection;
		column += jDirection;
	}
	//make sure we got exactly to the destination
	return row == move->iDest && column == move->jDest ? TRUE : FALSE;
}

Boolean isClearKnight(int row, int column, Move *move) {
	//gets a source and a destination and find if a knight could make a move from the source to the destination
	if (abs(move->iDest - row) == 2 && abs(move->jDest - column) == 1)
		return TRUE;
	if (abs(move->iDest - row) == 1 && abs(move->jDest - column) == 2)
		return TRUE;
	return FALSE;
}

Boolean canMove(char board[][SIZE], Move *move, int row, int column) {
	//gets a source and a destination and returning if a given piece can reach the destination
	
	//calculating the direction between the source and the destination
	int iDirection = move->iDest - row > 0 ? 1 : -1;
	int jDirection = move->jDest - column > 0 ? 1 : -1;
	if (isRook(move->srcPiece))
		return isClearCol(board, move, row, column, iDirection) || isClearRow(board, move, row, column, jDirection);
	if (isBishop(move->srcPiece))
		return isClearDiag(board, move, row, column, iDirection, jDirection);
	if (isQueen(move->srcPiece))
		return isClearCol(board, move, row, column, iDirection) || isClearRow(board, move, row, column, jDirection) ||
		       isClearDiag(board, move, row, column, iDirection, jDirection);
	if (isKnight(move->srcPiece))
		return isClearKnight(row, column, move);
	return FALSE;
}

Boolean checkValidity(char board[][SIZE], Move *move, int row, int column) {
	if (canMove(board, move, row, column))
		if (isValidMove(board, move, row, column))
			return TRUE;
	return FALSE;
}

/******************************************************************************************************
* Function Name: updateMove
* Input: char board[][] - 2D array
* 		 Move *move - a pointer to a move struct which saves all the important aspect of a chess move
* Output: None
* Function Operation: the function search for the piece that made the move and then make it
******************************************************************************************************/
void updateMove(char board[][SIZE], Move *move) {
	move->destPiece = board[move->iDest][move->jDest];
	move->isDestWhite = isupper(move->destPiece);
	char piece = move->srcPiece;
	if (!move->srcRow || !move->srcCol) {
		//if we don't have the source row or the source column
		if (move->srcRow && !isPawn(piece)) {
			for (int column = 0; column < SIZE; column++) {
				// search the row for the piece
				if (board[move->iSrc][column] == piece)
					if (checkValidity(board, move, move->iSrc, column))
						break;
			}
		} else if (move->srcCol && !isPawn(piece)) {
			for (int row = 0; row < SIZE; row++) {
				// search the column for the piece
				if (board[row][move->jSrc] == piece)
					if (checkValidity(board, move, row, move->jSrc))
						break;
			}
		} else
			findPiece(board, move, FALSE);
	} else {
		// we have both
		checkValidity(board, move, move->iSrc, move->jSrc);
	}
	if (move->isLegal)
		makeStep(board, move);
}

/************************************************************************************************************
* Function Name: makeMove
* Input: char board[][] - 2D array representing a chess board
* 		 char pgn[] - a string representing  a move
* 		 int isWhiteTurn - 1 if it's the white's turn and 0 if it's the black turn
* Output: int 1 if the move is legal and 0 if not
* Function Operation: the function gets a board and a string of move and makes it or return if its not a valid move
************************************************************************************************************/
int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	//initialize the move struct to be all zeros
	Move move = {};
	move.isSrcWhite = isWhiteTurn;
	parseMove(pgn, &move);
	updateMove(board, &move);
	return move.isLegal;
}