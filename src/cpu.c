#include <stdio.h>
#include <stdlib.h>
#include "plays.h"
#include "game.h"
#include "cpu.h"

int evaluate(Board *board, char playerMark);
int negaMax(Board *board, int depth, char playerMark, int *bestRow, int *bestCol);

void getCpuMove(int *row, int *col, Board *board, char playerMark) {

    if (playerMark == PLAYER_O) {
        int bestRow = -1;
        int bestCol = -1;
        int bestScore = negaMax(board, NEGA_MAX_DEPTH, playerMark, &bestRow, &bestCol);
        printf("negaMax Score: %d, move %d, %d\n", bestScore, bestRow, bestCol);
        *row = bestRow;
        *col = bestCol;
        return;
    }

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

int negaMax(Board *board, int depth, char playerMark, int* bestRow, int* bestCol) {
    if (depth == 0 || isGameOver(board)) {
        return evaluate(board, playerMark);
    }

    int maxScore = -9999999;
    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (board->cells[i][j] == EMPTY_CELL) {
                board->cells[i][j] = playerMark;

                char nextPlayerMark = (playerMark == PLAYER_X) ? PLAYER_O : PLAYER_X;
                int score = -negaMax(board, depth - 1, nextPlayerMark, bestRow, bestCol);
                if (score > maxScore) {
                    maxScore = score;
                    if (depth == NEGA_MAX_DEPTH) {
                        *bestRow = i;
                        *bestCol = j;
                    }
                }

                board->cells[i][j] = EMPTY_CELL;
            }
        }
    }
    return maxScore;
}

int __evaluatePlayerPositions(Board *board, char playerMark) {
    int score = 0;

    for (int d = 0; d < 4; d++) {
        int dx = 0, dy = 0;
        switch(d) {
            case 0:
                dx = 0;
                dy = 1;
                break;
            case 1:
                dx = 1;
                dy = 0;
                break;
            case 2:
                dx = 1;
                dy = 1;
                break;
            case 3:
                dx = 1;
                dy = -1;
                break;
            }

        for (int i = 1; i <= BOARD_ROWS; i++) {
            for (int j = 1; j <= BOARD_COLUMNS; j++) {
                if (board->cells[i][j] != playerMark)
                    continue;

                // lineの始点ではない場合、すでにカウント済みのためSkip
                int prevX = i - dx;
                int prevY = j - dy;
                if (isInRange(prevX, prevY) && board->cells[prevX][prevY] == playerMark)
                    continue;

                int length = 1;
                int nextX = i + dx;
                int nextY = j + dy;
                while(isInRange(nextX, nextY) && board->cells[nextX][nextY] == playerMark) {
                    length++;
                    nextX += dx;
                    nextY += dy;
                }
                EDGE_STATUS edge = OPEN;

                if (!isInRange(prevX, prevY) || board->cells[prevX][prevY] != EMPTY_CELL) 
                    edge++;

                if (!isInRange(nextX, nextY) || board->cells[nextX][nextY] != EMPTY_CELL) 
                    edge++;

                if (edge == CLOSED)
                    continue;

                switch (length)
                {
                case 5:
                    score += WIN_POINTS;
                    break;
                case 4:
                    score += (edge == OPEN) ? OPEN_FOUR_POINTS : CLOSED_FOUR_POINTS;
                    break;
                case 3:
                    score += (edge == OPEN) ? OPEN_THREE_POINTS : CLOSED_THREE_POINTS;
                    break;
                case 2:
                    score += (edge == OPEN) ? OPEN_TWO_POINTS : CLOSED_TWO_POINTS;
                    break;
                default:
                    break;
                }
            }
        }
    }
    return score;
}

int evaluate(Board *board, char playerMark) {

    char opponentPlayer = (playerMark == PLAYER_X) ? PLAYER_O : PLAYER_X;
    int playerScore = __evaluatePlayerPositions(board, playerMark);
    int opponentScore = __evaluatePlayerPositions(board, opponentPlayer);

    return playerScore - opponentScore;
}


int max(int x, int y) {
    if (x >= y)
        return x;
    return y;
}
