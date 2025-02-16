#include <stdio.h>
#include "board.h"

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


int min(int a, int b) {
    return (a < b) ? a : b;
}

// 2つの整数の最大値を返す
int max(int a, int b) {
    return (a > b) ? a : b;
}
