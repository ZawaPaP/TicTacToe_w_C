#ifndef GAME_H
#define GAME_H
#include "constants.h"

void runGameLoop();
char getWinner(Board *board);
BOOL isGameOver(Board *board);
BOOL hasWinner(Board *board, char playerMark);
// BOOL __checkDoubleThree(Board *board, int x, int y);
#endif
