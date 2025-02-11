#include <stdio.h>
#include <stdlib.h>
#include "plays.h"
#include "board.h"

BOOL isValidMove(int row, int col, Board *board);


BOOL canApplyMove(int row, int col, Board* board) {
    if (!isValidMove(row, col, board)) {
        return FALSE;
    }
    return TRUE;
}

BOOL isValidMoveInput(int *x, int *y){
    char str[8];

    printf("Please input your hand as row, col (or 'q' to quit): ");
    
    if (fgets(str, sizeof(str), stdin) == NULL) {
        printf("Error: invalid input. Try again.\n");
        return FALSE;
    }

    if (str[0] == 'q') {
        printf("\tGame Ended.\n");
        exit(0);
    }

    // sscanfは変換されたフィールドの数を返す
    if (sscanf(str, " %d , %d ", x, y) != 2 && sscanf(str, " %d %d ", x, y) != 2) {
        printf("Error: invalid input. Try again.\n");
        return FALSE;
    }

    return TRUE;
}

BOOL isValidMove(int row, int col, Board* board) {
    if (row < 1 || BOARD_ROWS < row || col < 1 || BOARD_COLUMNS < col) {
        printf("Error: Row and Column must be between 1 and %d.\n", BOARD_ROWS);
        return FALSE;
    }

    if (board->cells[row][col] != EMPTY_CELL) {
        printf("Error: The %d, %d is already marked.\n", row, col);
        return FALSE;
    }
    return TRUE;
}
