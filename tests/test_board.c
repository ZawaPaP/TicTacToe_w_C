#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/board.h"
#include "test_board.h"


// This helper function is too much but keep and use it in the future.
/* static void placeRandomMarks(Board *board) {

    int place_marks = rand() % 11;

    if (place_marks > BOARD_ROWS * BOARD_COLUMNS) {
        printf("Error: too big mark counts.\n");
    }

    for (int i = 0; i < place_marks; i++) {
        int r = rand() % BOARD_ROWS;
        int c = rand() % BOARD_COLUMNS;
        
        board->cells[r][c] = (rand() % 2) ? PLAYER_X : PLAYER_O;
    }
} */

void testInitBoard()
{
    printf("Start testInitBoard...\n");

    Board board;

    board.cells[0][0] = PLAYER_X;
    board.cells[4][6] = PLAYER_O;
    board.cells[8][8] = PLAYER_X;
    
    initBoard(&board);

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLUMNS; j++) {
            if (board.cells[i][j] != EMPTY_CELL) {
                printf("Error: %d, %d is not empty.\n", i, j);
                assert(board.cells[i][j] == EMPTY_CELL);
            }
        }
    }
    printf("test testInitBoard success.\n");
}

void runBoardTests() {
    printf("Start runBoardTests...\n");
    testInitBoard();
    printf("Finished runBoardTests.\n");
}


