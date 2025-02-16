#ifndef BOARD_H
#define BOARD_H
#include "constants.h"

void initBoard(Board *board);
BOOL boardIsFull(Board *board);
BOOL isInBoard(int r, int c);
BOOL isWinMove(Board *board, int r, int c, char playerMark);
BOOL isMakingOverLine(Board *board, int r, int c, char playerMark);
LineLengthPattern countContinuousStonesWithGap(Board *board, int r, int c, int dx, int dy, char playerMark);
BOOL __isSameLine(Board *board, LineInfo *line1, LineInfo *line2, int row, int col, int dx, int dy);
LineInfoArray __getTargetLengthLinesInDirectionCandidates(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
BOOL __wouldCreateOverline(Board *board, int row, int col, int dx, int dy, char playerMark);
LineInfoArray __getTargetLengthLinesInDirection(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
BOOL __isDoubleThree(Board *board, int x, int y);
#endif
