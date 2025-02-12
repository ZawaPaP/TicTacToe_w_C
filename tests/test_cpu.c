#include <stdio.h>
#include <assert.h>
#include "../include/cpu.h"
#include "test_utils.h"
#include "test_cpu.h"

void testMax() {
    int x, y;
    x = 10;
    y = 3;
    assert(max(x, y) == x);

    x = -19;
    y = 3;
    assert(max(x, y) == y);
}

void testEvaluateOpenFour() {
    Board board = __prepareBoard();
    board.cells[2][2] = PLAYER_X;
    board.cells[2][3] = PLAYER_X;
    board.cells[2][4] = PLAYER_X;
    board.cells[2][5] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);
    assert(scores.lengthScore == OPEN_FOUR_POINTS);
    assert(scores.positionScore == 32);
}

void testEvaluateCloseFour() {
    // 左上隅のエッジケースも兼ねている。
    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[2][1] = PLAYER_X;
    board.cells[3][1] = PLAYER_X;
    board.cells[4][1] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);
    assert(scores.lengthScore == CLOSED_FOUR_POINTS);
    assert(scores.positionScore == 18);
}

void testEvaluateOpenThree() {
    // 左斜め下方向の連結のテストも兼ねている。
    Board board = __prepareBoard();
    board.cells[4][4] = PLAYER_X;
    board.cells[5][3] = PLAYER_X;
    board.cells[6][2] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    assert(scores.lengthScore == OPEN_THREE_POINTS);
    assert(scores.positionScore == 40);
}

void testEvaluateCloseThree() {
    // 右斜め下方向の連結のテストと右下墨のエッジケーステストも兼ねている。
    Board board = __prepareBoard();
    board.cells[7][7] = PLAYER_X;
    board.cells[8][8] = PLAYER_X;
    board.cells[9][9] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    assert(scores.lengthScore == CLOSED_THREE_POINTS);
    assert(scores.positionScore == 19);
}

void testEvaluateOpenTwo() {
    Board board = __prepareBoard();
    board.cells[4][9] = PLAYER_X;
    board.cells[5][9] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    assert(scores.lengthScore == OPEN_TWO_POINTS);
    assert(scores.positionScore == 10);
}

void testEvaluateCloseTwo() {
    // 左下隅のエッジケースも兼ねている。
    Board board = __prepareBoard();
    board.cells[9][1] = PLAYER_X;
    board.cells[9][2] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    assert(scores.lengthScore == CLOSED_TWO_POINTS);
    assert(scores.positionScore == 8);
}

void testEvaluateDoubleClosedLine() {
    // 両側がClosedの場合、0点
    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[1][2] = PLAYER_X;
    board.cells[1][3] = PLAYER_X;
    board.cells[1][4] = PLAYER_X;
    board.cells[1][5] = PLAYER_O;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    assert(scores.lengthScore == 0);
    assert(scores.positionScore == 17);
}

void testEvaluateMultiLine() {
    // 右上隅のエッジケースも兼ねている。
    Board board = __prepareBoard();
    board.cells[1][6] = PLAYER_X;
    board.cells[1][7] = PLAYER_X;
    board.cells[1][8] = PLAYER_X;
    board.cells[1][9] = PLAYER_X;
    board.cells[2][7] = PLAYER_X;
    board.cells[3][7] = PLAYER_X;
    board.cells[4][7] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    // 縦と横の4マスづつの連結と、両斜めの連結
    assert(scores.lengthScore == CLOSED_FOUR_POINTS * 2 + CLOSED_TWO_POINTS * 2);
    assert(scores.positionScore == 42);
}

void testNegaMax() {
    Board board = __prepareBoard();
    board.cells[2][2] = PLAYER_X;
    board.cells[2][3] = PLAYER_X;
    board.cells[2][4] = PLAYER_X;
    board.cells[2][5] = PLAYER_X;
    board.cells[7][9] = PLAYER_O;
    board.cells[8][9] = PLAYER_O;

    int bestRow = -1, bestCol = -1;
    negaMax(&board, NEGA_MAX_DEPTH, PLAYER_X, &bestRow, &bestCol);

    assert(bestRow == 2);
    assert(bestCol == 1);
}

void runCPUTests(){
    printf("Start runCPUTests...\n");
    testMax();
    testEvaluateOpenFour();
    testEvaluateCloseFour();
    testEvaluateOpenThree();
    testEvaluateCloseThree();
    testEvaluateOpenTwo();
    testEvaluateCloseTwo();
    testEvaluateDoubleClosedLine();
    testEvaluateMultiLine();
    testNegaMax();
    printf("Finished runCPUTests.\n");
}
