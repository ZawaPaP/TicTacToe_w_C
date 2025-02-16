#ifndef GAME_H
#define GAME_H
#include "constants.h"

void
runGameLoop();
BOOL isGameOver(Board *board, int *row, int *col, char player);
#endif
