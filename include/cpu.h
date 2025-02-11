#ifndef CPU_H
#define CPU_H

#include "constants.h"
#include "board.h"

void getCpuMove(int *row, int *col, Board *board, char playerMark);
int evaluate(Board *board, char playerMark);
int max(int x, int y);

typedef struct {
    int startRow;
    int StartCol;
    int endRow;
    int endCol;
    int length;
} connectedLine;

#endif
