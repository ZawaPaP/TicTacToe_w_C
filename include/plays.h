#ifndef PLAYS_H
#define PLAYS_H
#include "constants.h"
#include "board.h"

typedef struct {
    int row;
    int column;
} Hand;

BOOL isValidMoveInput(int *x, int *y);
BOOL canApplyMove(int row, int col, Board *board, char player);
BOOL isValidMove(int row, int col, Board *board);

#endif
