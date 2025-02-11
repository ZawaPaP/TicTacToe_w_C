#include <stdio.h>
#include <stdlib.h>
#include "plays.h"
#include "game.h"
#include "cpu.h"

int evaluate(Board *board, char playerMark);
int max(int x, int y);

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

int negaMax(Board board, int depth, int isMyTurn, char playerMark) {
    if (depth == 0 || isGameOver(&board)) {
        return evaluate(&board, playerMark);
    }

    int maxScore = -99999;
    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (board.cells[i][j] == EMPTY_CELL) {
                board.cells[i][j] = playerMark;

                char nextPlayerMark = (playerMark == PLAYER_X) ? PLAYER_O : PLAYER_X;
                int score = -negaMax(board, depth - 1, !isMyTurn, nextPlayerMark);
                maxScore = max(score, maxScore);

                board.cells[i][j] = EMPTY_CELL;
            }
        }
    }
    return maxScore;
}


int evaluate(Board *board, char playerMark) {
    printf("%c, %c\n", board->cells[1][1], playerMark);
    return 1;
    /*
    手番で勝ち
        Open four
        Closed four
    手番ではなくても勝ち
        Open four
        二つ以上のFour

    それ以外
        各連結の数 x 点数 + 石の位置 (中央の方が連結可能性が高い)
        openFour: 200 points
        closedFour: 100 points
        openThree: 100 points
        closedThree: 50 points
        openTwo: 50 points
        closedTwo: 25 points

        石の位置
            周囲の空いているマス x 1 points
    */
}

int max(int x, int y) {
    if (x >= y)
        return x;
    return y;
}
