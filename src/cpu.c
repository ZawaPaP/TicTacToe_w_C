#include <stdio.h>
#include <stdlib.h>
#include "plays.h"
#include "cpu.h"

void getCpuMove(int *row, int *col, Board *board) {
    while(1) {

        int randR = 1 + rand() % BOARD_ROWS;
        int randC = 1 + rand() % BOARD_COLUMNS;

        if (canApplyMove(randR, randC, board)) {
            *row = randR;
            *col = randC;
            break;
        }
    }
}

