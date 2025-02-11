#include <stdio.h>
#include <stdlib.h>
#include "plays.h"
#include "game.h"
#include "cpu.h"

EDGE_STATUS __evaluateLineEdge(Board *board, connectedLine *line, int *direction);
connectedLine __countConnected(Board *board, int row, int col, char playerMark, BOOL (*visited)[BOARD_ROWS + 1][BOARD_COLUMNS + 1][4], int directionIndex, int (*directions)[4][2]);
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

    // 縦、横、右斜め、左斜めについてそれぞれカウント済みかどうかをメモする。
    BOOL visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1][4] = {};

    int directions[4][2] = {
        {0, 1}, // 右
        {1, 0}, // 下
        {1, 1}, // 右斜め下
        {1, -1} // 左斜め下
    };

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (board->cells[i][j] == playerMark) {
                for (int d = 0; d < 4; d++) {
                    if(visited[i][j][d]) continue;
                    
                    connectedLine line = __countConnected(board, i, j, playerMark, &visited, d, &directions);
                    EDGE_STATUS edge = __evaluateLineEdge(board, &line, directions[d]);

                    switch (line.length)
                    {
                    case 5:
                        score += WIN_POINTS;
                        break;
                    case 4:
                        if (edge == OPEN)
                            score += OPEN_FOUR_POINTS;
                        if (edge == SEMI_CLOSE)
                            score += CLOSED_FOUR_POINTS;
                        break;
                    case 3:
                        if (edge == OPEN)
                            score += OPEN_THREE_POINTS;
                        if (edge == SEMI_CLOSE)
                            score += CLOSED_THREE_POINTS;
                        break;
                    case 2:
                        if (edge == OPEN)
                            score += OPEN_TWO_POINTS;
                        if (edge == SEMI_CLOSE)
                            score += CLOSED_TWO_POINTS;
                        break;
                    default:
                        break;
                    }
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

connectedLine __countConnected(Board *board, int row, int col, char playerMark, BOOL (*visited)[BOARD_ROWS + 1][BOARD_COLUMNS + 1][4], int directionIndex, int (*directions)[4][2]) {
    connectedLine line;
    line.startRow = row;
    line.StartCol = col;
    line.endRow = row;
    line.endCol = col;
    line.length = 1;

    int rowMove = (*directions)[directionIndex][0];
    int colMove = (*directions)[directionIndex][1];

    int nextRow = row + rowMove;
    int nextCol = col + colMove;
    while (isInRange(nextRow, nextCol)) {
        if (board->cells[nextRow][nextCol] != playerMark) break;

        line.length++;
        line.endRow = nextRow;
        line.endCol = nextCol;
        (*visited)[nextRow][nextCol][directionIndex] = TRUE;
        nextRow += rowMove;
        nextCol += colMove;
    }
    return line;
}

EDGE_STATUS __evaluateLineEdge(Board *board, connectedLine* line, int *direction) {
    EDGE_STATUS edgeStatus = OPEN;

    int rowMove = direction[0];
    int colMove = direction[1];

    int prevRow = line->startRow - rowMove;
    int prevCol = line->StartCol - colMove;
    if (!isInRange(prevRow, prevCol) || board->cells[prevRow][prevCol] != EMPTY_CELL) 
        edgeStatus += 1;

    int nextRow = line->endRow + rowMove;
    int nextCol = line->endCol + colMove;
    if (!isInRange(nextRow, nextCol) || board->cells[nextRow][nextCol] != EMPTY_CELL) 
        edgeStatus += 1;

    return edgeStatus;
}

int max(int x, int y) {
    if (x >= y)
        return x;
    return y;
}
