#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "ui.h"
#include "game.h"
#include "cpu.h"

int evaluate(Board *board, char playerMark);
int negaMax(Board *board, int depth, char playerMark, int *bestRow, int *bestCol, int alpha, int beta);


void getCpuMove(Board *board, int *row, int *col, char playerMark) {

    if (playerMark == PLAYER_O) {
        int bestRow = -1;
        int bestCol = -1;
        negaMax(board, NEGA_MAX_DEPTH, playerMark, &bestRow, &bestCol, -9999999, 9999999);
        *row = bestRow;
        *col = bestCol;
        return;
    }

    while(1) {
        int randR = 1 + rand() % BOARD_ROWS;
        int randC = 1 + rand() % BOARD_COLUMNS;
        if (isValidMove(board, randR, randC, playerMark)) {
            *row = randR;
            *col = randC;
            break;
        }
    }
}

int negaMax(Board *board, int depth, char playerMark, int* bestRow, int* bestCol, int alpha, int beta) {
    if (depth == 0 || isGameOver(board, bestRow, bestCol, playerMark)) {
        return evaluate(board, playerMark);
    }

    int maxScore = -9999999;
    for (int r = 1; r <= BOARD_ROWS; r++) {
        for (int c = 1; c <= BOARD_COLUMNS; c++) {
            if (board->cells[r][c] == EMPTY_CELL) {
                // 禁じ手チェック
                if (playerMark == PLAYER_X && isProhibitedMove(board, r, c, playerMark))
                    continue;  

                board->cells[r][c] = playerMark;

                char nextPlayerMark = (playerMark == PLAYER_X) ? PLAYER_O : PLAYER_X;
                int score = -negaMax(board, depth - 1, nextPlayerMark, bestRow, bestCol, -beta, -alpha);
                if (score > maxScore) {
                    maxScore = score;
                    if (depth == NEGA_MAX_DEPTH) {
                        *bestRow = r;
                        *bestCol = c;
                    }
                }

                board->cells[r][c] = EMPTY_CELL;


                alpha = max(alpha, maxScore);
                
                // 枝刈りの条件: この時点でbeta以上のスコアが出ていれば、
                // 親ノードはこの手を選択しないことが確定する
                if (alpha >= beta)
                    return maxScore;
            }
        }
    }
    return maxScore;
}


int __evaluateLengths(Board *board, int i, int j, int dx, int dy, char playerMark) {
    int score = 0;

    // lineの始点ではない場合、すでにカウント済みのためSkip
    int prevX = i - dx;
    int prevY = j - dy;
    if (isInBoard(prevX, prevY) && board->cells[prevX][prevY] == playerMark)
        return score;

    int length = 1;
    int nextX = i + dx;
    int nextY = j + dy;
    while (isInBoard(nextX, nextY) && board->cells[nextX][nextY] == playerMark)
    {
        length++;
        nextX += dx;
        nextY += dy;
    }
    EDGE_STATUS edge = OPEN;

    if (!isInBoard(prevX, prevY) || board->cells[prevX][prevY] != EMPTY_CELL) 
        edge++;

    if (!isInBoard(nextX, nextY) || board->cells[nextX][nextY] != EMPTY_CELL) 
        edge++;

    if (edge == CLOSED)
        return score;

    if (length >= 5) {
        score += WIN_POINTS;
    }
    switch (length)
    {
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
    return score;
}

int __evaluatePositions(Board *board, int row, int col, char playerMark) {
    // 周囲が空いているとポイントがつく
    // さらに中央付近だと、CENTER_MULTIPLIER倍にしている
    int score = 0;

    static const int DIRS[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    int adjacentCount = 0;
    for (int d = 0; d < 8; d++) {
        int newRow = row + DIRS[d][0];
        int newCol = col + DIRS[d][1];

        if (isInBoard(newRow, newCol))
        {
            if (board->cells[newRow][newCol] == EMPTY_CELL ||
                board->cells[newRow][newCol] == playerMark) {
                adjacentCount++;
            }
        }
    }
    score = adjacentCount;

        if (row >= CENTER_START_ROW && row <= CENTER_END_ROW &&
            col >= CENTER_START_COL && col <= CENTER_END_COL)
    {
        score *= CENTER_MULTIPLIER;
    }
    return score;
}

EvaluationScores __evaluateStones(Board *board, char playerMark) {
    EvaluationScores scores = {0, 0, 0};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (board->cells[i][j] != playerMark)
                continue;
            scores.positionScore += __evaluatePositions(board, i, j, playerMark);

            for (int d = 0; d < 4; d++) {
                int dx = 0, dy = 0;
                switch(d) {
                    case 0: dx = 0; dy = 1; break;  // 右
                    case 1: dx = 1; dy = 0; break;  // 下
                    case 2: dx = 1; dy = 1; break;  // 右斜め下
                    case 3: dx = 1; dy = -1; break; // 左斜め下
                }
                scores.lengthScore += __evaluateLengths(board, i, j, dx, dy, playerMark);
            }
        }
    }
    return scores;
}

int evaluate(Board *board, char playerMark) {

    char opponentPlayer = (playerMark == PLAYER_X) ? PLAYER_O : PLAYER_X;
    EvaluationScores playerScore = __evaluateStones(board, playerMark);
    EvaluationScores opponentScore = __evaluateStones(board, opponentPlayer);

    return playerScore.lengthScore + playerScore.positionScore - opponentScore.lengthScore - opponentScore.positionScore;
}
