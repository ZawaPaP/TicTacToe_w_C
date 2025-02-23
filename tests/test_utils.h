#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../include/constants.h"
#include "../include/board.h"

typedef struct {
    int total;
    int passed;
    int failed;
} TestResults;

void test_begin(const char* test_name);
void test_end(const char* test_name);
void test_suite_begin(const char* suite_name);
void test_suite_end(const char* suite_name, TestResults* results);
void test_assert(BOOL condition, const char* message, TestResults* results);

void suppress_output(void);
void restore_output(void);

Board __prepareBoard();
void initBoardWithStr(Board *board, const char *lines[BOARD_ROWS + 1]);
void runUtilsTests();
#endif
