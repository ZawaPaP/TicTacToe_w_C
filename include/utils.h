#ifndef UTILS_H
#define UTILS_H

#include "constants.h"

Board __prepareBoard();
void initBoardWithStr(Board *board, const char *lines[BOARD_ROWS + 1]);
int min(int a, int b);
int max(int a, int b);

#endif
