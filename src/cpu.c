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


int __evaluateLengths(Board *board, int r, int c, char playerMark) {
    int score = 0;

    static const Direction DIRS[4] = {
    {1, 0},   // 垂直方向 (↓)
    {0, 1},   // 水平方向 (→)
    {1, 1},   // 右下がり斜め (↘)
    {1, -1}   // 左下がり斜め (↙︎)
    };

    static const int dirLength = 4;

    for (int dir = 0; dir < dirLength; dir++) {
        int dx = DIRS[dir].dx;
        int dy = DIRS[dir].dy;

        // lineの始点ではない場合、すでにカウント済みのためSkip
        int prevX = r - dx;
        int prevY = c - dy;
        if (isInBoard(prevX, prevY) && board->cells[prevX][prevY] == playerMark)
            continue;

        LinePatterns patterns = countContinuousStonesWithGap(board, r, c, dx, dy, playerMark);

        for (int p = 0; p < patterns.pattern; p++) {
            LineInfo line = patterns.lines[p];
            
            // 両端の状態をチェック
            int prevX = line.start.r - line.dir.dx;
            int prevY = line.start.c - line.dir.dy;
            int nextX = line.end.r + line.dir.dx;
            int nextY = line.end.c + line.dir.dy;
            
            BOOL isPrevOpen = isInBoard(prevX, prevY) && 
                            board->cells[prevX][prevY] == EMPTY_CELL;
            BOOL isNextOpen = isInBoard(nextX, nextY) && 
                            board->cells[nextX][nextY] == EMPTY_CELL;

            BOOL fullOpen = !line.hasGap && isPrevOpen && isNextOpen;
            BOOL halfOpen = line.hasGap || (isPrevOpen || isNextOpen);

            if (line.length >= 5) {
                if (!line.hasGap) {
                    // 勝利
                    score += WIN_POINTS;
                } else {
                    // gapがある場合は、そこにおけたら勝利なので、片四と同じ
                    score += CLOSED_FOUR_POINTS;
                }
            } else if (line.length == 4) {
                if (fullOpen) {
                    score += OPEN_FOUR_POINTS;
                } else if (halfOpen) {
                    score += CLOSED_FOUR_POINTS;
                }
            } else if (line.length == 3) {
                if (fullOpen) {
                    score += OPEN_THREE_POINTS;
                } else if (halfOpen) {
                    score += CLOSED_THREE_POINTS;
                }
            } else if (line.length == 2) {
                if (fullOpen) {
                    score += OPEN_TWO_POINTS; 
                } else if (halfOpen) {
                    score += CLOSED_TWO_POINTS;
                }
            }
        }
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

    for (int row = 1; row <= BOARD_ROWS; row++) {
        for (int col = 1; col <= BOARD_COLUMNS; col++) {
            if (board->cells[row][col] != playerMark)
                continue;
            scores.positionScore += __evaluatePositions(board, row, col, playerMark);

            scores.lengthScore += __evaluateLengths(board, row, col, playerMark);
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
