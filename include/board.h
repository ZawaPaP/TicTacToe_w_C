#ifndef BOARD_H
#define BOARD_H
#include "constants.h"

void initBoard(Board *board);
BOOL boardIsFull(Board *board);
BOOL isInBoard(int r, int c);
BOOL isWinMove(Board *board, int r, int c, char playerMark);
BOOL __isSameLine(Board *board, lineInfo *line1, lineInfo *line2, int row, int col, int dx, int dy);
lineInfoArray __getTargetLengthLinesInDirectionCandidates(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
#endif
