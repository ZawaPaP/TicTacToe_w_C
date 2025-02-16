#ifndef CPU_H
#define CPU_H

#include "constants.h"
#include "board.h"

typedef struct {
    int lengthScore;      // 連続した石の評価
    int positionScore;    // 位置の価値評価
    int patternScore;     // その他パターンの評価
} EvaluationScores;


void getCpuMove(int *row, int *col, Board *board, char playerMark);
int negaMax(Board *board, int depth, char playerMark, int *bestRow, int *bestCol);
EvaluationScores __evaluateStones(Board *board, char playerMark);
int evaluate(Board *board, char playerMark);

#endif
