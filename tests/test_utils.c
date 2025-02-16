#include <stdio.h>
#include <assert.h>
#include "../include/board.h"
#include "../include/utils.h"


void testInitBoardWithStr() {
    Board board;

    const char *lines[] = {
        NULL,
        "........O",
        "...X.....",
        "..X......",
        "..X...O..",
        "X...O....",
        ".........",
        ".....@...",
        ".........",
        "........X"
    };
    initBoardWithStr(&board, lines);
    assert(board.cells[1][9] == PLAYER_O);
    assert(board.cells[2][4] == PLAYER_X);
    assert(board.cells[3][3] == PLAYER_X);
    assert(board.cells[4][3] == PLAYER_X);
    assert(board.cells[4][7] == PLAYER_O);
    assert(board.cells[5][1] == PLAYER_X);
    assert(board.cells[5][5] == PLAYER_O);
    assert(board.cells[7][6] == EMPTY_CELL);
    assert(board.cells[9][1] == EMPTY_CELL);
    assert(board.cells[9][9] == PLAYER_X);
}

void testMax() {
    int x, y;
    x = 10;
    y = 3;
    assert(max(x, y) == x);

    x = -19;
    y = 3;
    assert(max(x, y) == y);
}



void runUtilsTests(){
    printf("Start runUtilsTests...\n");
    testMax();
    testInitBoardWithStr();
    printf("Finished runUtilsTests.\n");
}
