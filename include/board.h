#ifndef BOARD_H
#define BOARD_H
#include "constants.h"

typedef struct {
    char cells[BOARD_ROWS + 1][BOARD_COLUMNS + 1];
    int lastRow, lastCol;
} Board;

void printBoard(Board *board);
void initBoard(Board *board);
BOOL boardIsFull(Board *board);
BOOL __isInBoardRange(int r, int c);
BOOL __isSameLine(Board *board, lineInfo *line1, lineInfo *line2, int row, int col, int dx, int dy);
lineInfoArray __getTargetLengthLinesInDirectionCandidates(Board *board, int row, int col, int length, int dx, int dy, char playerMark);
#endif
