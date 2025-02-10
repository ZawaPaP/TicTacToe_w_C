#ifndef PLAYS_H
#define PLAYS_H
#include "constants.h"
#include "board.h"

typedef struct {
    int row;
    int column;
} Hand;

int getInput(int *x, int *y);
int placeMove(int row, int col, Board *board, char player);
int validateInput(int row, int col, Board *board);

#endif
