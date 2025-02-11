#include <stdio.h>
#include <assert.h>
#include "../include/cpu.h"
#include "test_utils.h"
#include "test_cpu.h"

void testMax() {
    int x, y;
    x = 10;
    y = 3;
    assert(max(x, y) == x);

    x = -19;
    y = 3;
    assert(max(x, y) == y);
}

void testEvaluateOpenFour() {
    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[2][1] = PLAYER_X;
    board.cells[3][1] = PLAYER_X;
    board.cells[4][1] = PLAYER_X;

    assert(evaluate(&board, PLAYER_X) == 200);
}

void runCPUTests(){
    printf("Start runCPUTests...\n");
    testMax();
    testEvaluateOpenFour();
    printf("Finished runCPUTests.\n");
}
