#include <stdio.h>
#include "plays.h"
#include "board.h"


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

    if (1 <= *x && *x <= 9 && 1 <= *y && *y <= 9) {
        return 1;
    }
    return 0;
}

int validateInput(int row, int col, Board* board) {
    if (row < 1 || BOARD_ROWS < row || col < 1 || BOARD_COLUMNS < col) {
        return 0;
    }

    if (board->cells[row][col] != EMPTY_CELL) {
        return 0;
    }
    return 1;
}
