#ifndef CPU_H
#define CPU_H

#include "constants.h"
#include "board.h"

void getCpuMove(Board *board, int *row, int *col, char playerMark);
int negaMax(Board *board, int depth, char playerMark, int *bestRow, int *bestCol, int alpha, int beta);
EvaluationScores __evaluateStones(Board *board, char playerMark);
int evaluate(Board *board, char playerMark);

#endif
