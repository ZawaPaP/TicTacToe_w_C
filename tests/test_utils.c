#include <stdio.h>
#include <assert.h>
#include "../include/board.h"

Board __prepareBoard() {
    Board board;
    initBoard(&board);
    return board;
}


void initBoardWithStr(Board *board, const char *lines[BOARD_ROWS + 1]) {
    initBoard(board);

    // 文字列から盤面を設定
    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            switch (lines[i][j - 1]) {
                case 'O':
                    board->cells[i][j] = PLAYER_O;
                    break;
                case 'X':
                    board->cells[i][j] = PLAYER_X;
                    break;
                case '.':
                    board->cells[i][j] = EMPTY_CELL;
                    break;
                default:
                    board->cells[i][j] = EMPTY_CELL;
            }
        }
    }
}


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

void runUtilsTests(){
    printf("Start runUtilsTests...\n");
    testInitBoardWithStr();
    printf("Finished runUtilsTests.\n");
}
