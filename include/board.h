#ifndef BOARD_H
#define BOARD_H
#include "constants.h"

void initBoard(Board *board);
BOOL boardIsFull(Board *board);
BOOL isInBoard(int r, int c);
BOOL isWinMove(Board *board, int r, int c, char playerMark);
BOOL isMakingOverLine(Board *board, int r, int c, char playerMark);
BOOL isOpenLine(Board *board, Cell start, Cell end, Direction dir);
BOOL isAtLeastHalfOpenLine(Board *board, Cell start, Cell end, Direction dir);
LinePatterns countContinuousStonesWithGap(Board *board, int r, int c, int dx, int dy, char playerMark);
BOOL __isSameLine(Board *board, LineInfo *line1, LineInfo *line2, int row, int col, int dx, int dy);
LineInfoArray __getTargetLengthLinesInDirectionCandidates(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
BOOL __wouldCreateOverline(Board *board, int row, int col, int dx, int dy, char playerMark);
LineInfoArray __getTargetLengthLinesInDirection(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
BOOL __isDoubleThree(Board *board, int x, int y);
Cell getGapIdx(Board *board, LineIdx *line);
BOOL isFour(Board *board, LineIdx *line, char playerMark);
BOOL isMakingDoubleFour(Board *board, int row, int col, char playerMark);
BOOL isMakingGreatFour(Board *board, int r, int c, char playerMark);
#endif
