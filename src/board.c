#include <stdio.h>
#include "board.h"

void printBoard(Board *board) {
    int i, j, k;
    printf("    ");
    for (k = 0; k < BOARD_COLUMNS; k++) {
        printf("%d   ", k + 1);
    }
    printf("\n\n");

    for (i = 0; i < BOARD_ROWS; i++)
    {
        printf("%d   ", i + 1);
        for (j = 0; j < BOARD_COLUMNS - 1; j++)
        {
            printf("%c | ", board->cells[i][j]);
        }
        printf("%c\n", board->cells[i][BOARD_COLUMNS - 1]);

        if (i < BOARD_ROWS - 1)
        {
            printf("    - + - + - + - + - + - + - + - + - \n");
        }
    }
    printf("\n\n");
}

void initBoard(Board *board) {
    size_t i, j;
    for (i = 0; i < sizeof(board->cells) / sizeof(board->cells[0][0]); i++)
    {
        for (j = 0; j < sizeof(board->cells[0]) / sizeof(board->cells[0][0]); j++) {
            board->cells[i][j] = EMPTY_CELL;
        }
    }
}