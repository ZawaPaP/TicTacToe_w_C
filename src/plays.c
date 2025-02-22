#include <stdio.h>
#include <stdlib.h>
#include "plays.h"
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
            if (i == board->lastRow && j == board->lastCol) {
                printf("\x1b[32m%c\x1b[39m | ", board->cells[i][j]);
            }
            else
                printf("%c | ", board->cells[i][j]);
        }
        if (i == board->lastRow && BOARD_COLUMNS == board->lastCol) {
            printf("\x1b[32m%c\x1b[0m\n", board->cells[i][BOARD_COLUMNS]);
        } else {
            printf("%c\n", board->cells[i][BOARD_COLUMNS]);
        }

        if (i < BOARD_ROWS)
        {
            printf("    - + - + - + - + - + - + - + - + - \n");
        }
    }
    printf("\n\n");
}

void printGameStatus(int turnCounts, char player) {
    printf("Turn %d, %c's turn.\n",turnCounts, player);    
}

void printWinner(char player) {
    printf("\t\tWinner is %c!\n\n", player);
}

void printDrawGame() {
    printf("\tDrow. Nice game!\n\n");
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

BOOL isValidMove(int row, int col, Board* board, char playerMark) {
    if (row < 1 || BOARD_ROWS < row || col < 1 || BOARD_COLUMNS < col) {
        printf("Error: Row and Column must be between 1 and %d.\n", BOARD_ROWS);
        return FALSE;
    }

    if (board->cells[row][col] != EMPTY_CELL) {
        printf("Error: The %d, %d is already marked.\n", row, col);
        return FALSE;
    }

    if (isProhibitedMove(board, row, col, playerMark)) {
        printf("Error: The %d, %d is prohibited move.\n", row, col);
        return FALSE;
    }
    return TRUE;
}

BOOL canApplyMove(int row, int col, Board* board, char playerMark) {
    if (!isValidMove(row, col, board, playerMark)) {
        return FALSE;
    }
    return TRUE;
}
