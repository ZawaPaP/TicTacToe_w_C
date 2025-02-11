#ifndef CPU_H
#define CPU_H

#include "constants.h"
#include "board.h"

void getCpuMove(int *row, int *col, Board *board);
int evaluate(Board *board, char playerMark);
int max(int x, int y);
#endif
