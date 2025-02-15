#ifndef PLAYS_H
#define PLAYS_H
#include "constants.h"
#include "board.h"


BOOL isValidMoveInput(int *x, int *y);
BOOL canApplyMove(int row, int col, Board *board);
BOOL isValidMove(int row, int col, Board *board);
void printBoard(Board *board);
void printGameStatus(int turnCounts, char player);
void printWinner(char player);
void printDrawGame();
#endif
