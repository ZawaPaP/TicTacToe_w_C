#ifndef BOARD_H
#define BOARD_H
#include "constants.h"

typedef struct {
    char cells[BOARD_ROWS + 1][BOARD_COLUMNS + 1];
} Board;


void printBoard(Board *board);
void initBoard(Board *board);
BOOL boardIsFull(Board *board);
#endif
