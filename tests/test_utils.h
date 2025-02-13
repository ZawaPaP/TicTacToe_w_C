#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../include/board.h"
Board __prepareBoard();
void initBoardWithStr(Board *board, const char *lines[BOARD_ROWS + 1]);
void runUtilsTests();
#endif
