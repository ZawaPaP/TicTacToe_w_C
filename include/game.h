#ifndef GAME_H
#define GAME_H
#include "constants.h"

void
runGameLoop();
char getWinner(Board *board);
BOOL isGameOver(Board *board, int *row, int *col, char player);
BOOL __wouldCreateOverline(Board *board, int row, int col, int dx, int dy, char playerMark);
lineInfoArray __getTargetLengthLinesInDirection(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
BOOL __isDoubleThree(Board *board, int x, int y);
#endif
