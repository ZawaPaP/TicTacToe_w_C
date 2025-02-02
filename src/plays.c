#include <stdio.h>
#include "plays.h"
#include "board.h"

int validateInput(int row, int col, Board *board);


int placeMove(int row, int col, Board* board, char player) {
    if (!validateInput(row, col, board)) {
        return 0;
    }
    board->cells[row][col] = player;
    return 1;
}

int getInput(int *x, int *y){
    char str[8];

    printf("Please input your hand as row, col: ");
    
    if (fgets(str, sizeof(str), stdin) == NULL) {
        return 0;
    }
    // sscanfは変換されたフィールドの数を返す
    if (sscanf(str, " %d , %d ", x, y) != 2 && sscanf(str, " %d %d ", x, y) != 2) {
        return 0;
    }

    return 1;
}

int validateInput(int row, int col, Board* board) {
    if (row < 1 || BOARD_ROWS < row || col < 1 || BOARD_COLUMNS < col) {
        printf("Error: Row and Column must be between 1 and %d.\n", BOARD_ROWS);
        return 0;
    }

    if (board->cells[row][col] != EMPTY_CELL) {
        printf("Error: The %d, %d is already marked.\n", row, col);
        return 0;
    }
    return 1;
}
