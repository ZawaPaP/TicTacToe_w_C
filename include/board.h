#ifndef BOARD_H
#define BOARD_H

#define BOARD_ROWS 9
#define BOARD_COLUMNS 9

#define EMPTY_CELL ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

typedef struct {
    char cells[BOARD_ROWS + 1][BOARD_COLUMNS + 1];
} Board;


void printBoard(Board *board);
void initBoard(Board *board);
#endif
