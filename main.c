#include "ass4.h"
#include <stdio.h>

void printMove(char board[][SIZE], char *const pgn[], int gameLength) {
	int leagal = 0;
	for (int i = 0; i < gameLength; i++) {
		if (i == 11) {
			printf("%d", i);
		}
		int isWhite = !(i % 2);
		leagal = makeMove(board, pgn[i], isWhite);
		if (isWhite)
			printf("White:");
		else
			printf("Black:");
		if (leagal) {
			printf("%s\n", pgn[i]);
			printBoard(board);
		} else {
			printf("\n\n\n\n%d: %s\n\n\n\n\n", i, pgn[i]);
			printf("illigal Move\n");
		}
	}
}

int main() {
	char board[SIZE][SIZE] = {};
	char fen81[] = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR";
	char fen82[] = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR";
	char fen83[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	char fen84[] = "rnb2b2/pppppppp/5Qk1/R1B5/1r1R1P1n/5B2/PPPP1PPP/1N1qK1N1";
	char fen85[] = "5R2/6Q1/7P/3K4/8/8/2k3P1/8";
	char fen86[] = "6B1/2b5/7b/8/8/8/5b1B/B7";
	char fen87[] = "r1bqqb1r/8/8/8/8/8/8/R1BQQB1R";
	char fen88[] = "r1bqkb1r/8/8/8/8/8/8/R1BQKB1R";
	char fen89[] = "3rR3/8/8/r6r/R6R/8/8/3rR3";
	char fen810[] = "4k3/8/4N3/8/8/8/8/8";
	char fen41[] = "rnb1/1p3/4/R1Bk";
	
	char *pgn82[] = {
			"Nh3", "Na6",
			"Qd5", "Ba3",
			"Bh6", "Nxh6",
			"Rxa3", "Bxh3",
			"Nc3", "Nf5",
			"Bxh3", "Qa5",
			"Qxa8+", "Nb8",
			"Bxf5", "Qxc3+",
			"Rxc3", "Rxh1+",
			"Qxh1", "Nc6",
			"Bc8", "Ne5",
			"Rc4", "Nxc4",
			"Bd7+", "Kxd7",
			"Qe4", "Kd6",
			"Qxc4", "Ke5",
			"Qe4+", "Kxe4"
	};
	char *pgn84[] = {
			"e4", "d5",
			"c3", "g6",
			"a4", "b5",
			"d3", "a6",
			"e5", "g5",
			"c4", "bxc4",
			"dxc4", "c5",
			"cxd5", "c4"
	};
	
	
	char *pgn81[] = {
			"Qe2+", "Qe7",
			"Rxa8", "Qxe2+",
			"Bxe2", "Rxh1",
			"Be3", "Bd6",
			"Nc3", "Bb7",
			"Ra7", "Bc6",
			"Kd2", "Ne7",
			"Bf3", "Rh2+",
			"Kd3", "Bd7",
			"Ra8", "Nc6",
			"Nb5", "Ne5+",
			"Kd4", "Bxb5",
			"Bf4", "Nxf3+",
			"Nxf3", "Bxf4",
			"Nxh2", "Bxh2",
			"Kc5", "Bd7",
			"Kb6", "Ke7",
			"Kb7", "Bc6+",
			"Kc8", "Nd7",
			"Ra7", "Ke8",
			"Ra2", "Nb6#"
	};
	char *pgn83[] = {
			"e4", "d6",
			"d4", "Nf6",
			"Nc3", "g6",
			"Be3", "Bg7",
			"Qd2", "c6",
			"f3", "b5",
			"Nge2", "Nbd7",
			"Bh6", "Bxh6",
			"Qxh6", "Bb7",
			"a3", "e5",
			"aaa", "Qe7",
			"Kb1", "a6",
			"Nc1", "bbb",
			"Nb3", "exd4",
			"Rxd4", "c5",
			"Rd1", "Nb6",
			"g3", "Kb8",
			"Na5", "Ba8",
			"Bh3", "d5",
			"Qf4+", "Ka7",
			"Rhe1", "d4",
			"Nd5", "Nbxd5",
			"exd5", "Qd6",
			"Rxd4", "cxd4",
			"Re7+", "Kb6",
			"Qxd4+", "Kxa5",
			"b4+", "Ka4",
			"Qc3", "Qxd5",
			"Ra7", "Bb7",
			"Rxb7", "Qc4",
			"Qxf6", "Kxa3",
			"Qxa6+", "Kxb4",
			"c3+", "Kxc3",
			"Qa1+", "Kd2",
			"Qb2+", "Kd1",
			"Bf1", "Rd2",
			"Rd7", "Rxd7",
			"Bxc4", "bxc4",
			"Qxh8", "Rd3",
			"Qa8", "c3",
			"Qa4+", "Ke1",
			"f4", "f5",
			"Kc1", "Rd2",
			"Qa7"
	};
	char *pgn85[] = {
			"Kb1", "h7",
			"Ka2", "h8=B"
	};
	
	char *pgn86[] = {
			"Bd4", "Bh4",
			"Bxc7", "Bf8",
			"Ba2", "Bd8",
			"Bxd8", "Bh6"
	};
	char *pgn87[] = {
			"Rh5", "Ba3",
			"Qa4", "Bh3",
			"Bxh3", "Rxa4",
			"Rxa3", "Qxh5",
			"Qh4", "Rxh4",
			"Bc8", "Rh6",
			"Bxh6", "Qb5",
			"Rd3", "Rd4",
			"Bh3", "Qb3",
			"Rxb3", "Qh4",
			"Bd2", "Ra4",
			"Rb8", "Ra5",
			"Bxa5", "Qg3",
			"Bd8", "Qxb8",
			"Bb6", "Qxb6",
			"Bc8", "Qa6",
			"Bxa6"
	};
	
	char *pgn88[] = {
			"Qa4+", "Bd7",
			"Qxa8", "Kf7",
			"Bh6", "Bb4+",
			"Kd1", "Qxa8",
			"Rxa8", "Rxa8",
			"Bf8", "Be1",
			"Kxe1", "Kxf8",
			"Rh7", "Bh3",
			"Rxh3", "Ra6",
			"Bg2", "Ra2",
			"Rh6", "Rxg2",
			"Rh1", "Rg1+",
			"Rxg1", "Kf7",
			"Rg7+", "Kxg7"
	};
	
	
	char *pgn89[] = {
			"Ra1", "R1d6",
			"Rac1", "Raa8",
			"Ra1", "R8d7",
			"Rexa8", "Rxh4",
			"Rh1", "Rd1",
			"Rhxd1", "Rh1",
			"Rh8", "Rhxd1",
			"Raa8", "R7d4",
			"Rh3", "R4d3",
			"Ra3", "Rc3",
			"Rhxc3", "Rb1",
			"Rd3", "Rb3",
			"Rdxb3"
	};
	int gameLength = sizeof(pgn84) / sizeof(char *);
	createBoard(board, fen83);
	printBoard(board);
	printMove(board, pgn84, gameLength);
	return 0;
}

