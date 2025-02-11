#include <stdio.h>
#include "board.h"

void printBoard(Board *board) {
    int i, j, k;
    printf("    ");
    for (k = 1; k <= BOARD_COLUMNS; k++) {
        printf("%d   ", k);
    }
    printf("\n\n");

    for (i = 1; i <= BOARD_ROWS; i++)
    {
        printf("%d   ", i);
        for (j = 1; j < BOARD_COLUMNS; j++)
        {
            printf("%c | ", board->cells[i][j]);
        }
        printf("%c\n", board->cells[i][BOARD_COLUMNS]);

        if (i < BOARD_ROWS)
        {
            printf("    - + - + - + - + - + - + - + - + - \n");
        }
    }
    printf("\n\n");
}

void initBoard(Board *board) {
    size_t i, j;
    for (i = 1; i <= BOARD_ROWS; i++) {
        for (j = 1; j <= BOARD_COLUMNS; j++) {
            board->cells[i][j] = EMPTY_CELL;
        }
    }
}

BOOL boardIsFull(Board *board) {
    size_t i, j;
    for (i = 1; i <= BOARD_ROWS; i++) {
        for (j = 1; j <= BOARD_COLUMNS; j++) {
            if (board->cells[i][j] == EMPTY_CELL)
                return FALSE;
        }
    }
    return TRUE;
}
