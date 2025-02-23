#include <stdio.h>
#include <assert.h>
#include "../include/utils.h"
#include "../include/cpu.h"
#include "test_utils.h"
#include "test_cpu.h"

void testEvaluateOpenFour(TestResults* results) {
    test_begin("EvaluateOpenFour");

    Board board = __prepareBoard();
    board.cells[2][2] = PLAYER_X;
    board.cells[2][3] = PLAYER_X;
    board.cells[2][4] = PLAYER_X;
    board.cells[2][5] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);
    
    test_assert(scores.lengthScore == OPEN_FOUR_POINTS, 
                "Open four should score OPEN_FOUR_POINTS", results);
    test_assert(scores.positionScore == 32, 
                "Position score should be 32", results);
    
    test_end("EvaluateOpenFour");
}

void testEvaluateCloseFour(TestResults* results) {
    test_begin("EvaluateCloseFour");

    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[2][1] = PLAYER_X;
    board.cells[3][1] = PLAYER_X;
    board.cells[4][1] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);
    
    test_assert(scores.lengthScore == CLOSED_FOUR_POINTS, 
                "Close four should score CLOSED_FOUR_POINTS", results);
    test_assert(scores.positionScore == 18, 
                "Position score should be 18", results);
    
    test_end("EvaluateCloseFour");
}

void testEvaluateOpenThree(TestResults* results) {
    test_begin("EvaluateOpenThree");

    Board board = __prepareBoard();
    board.cells[4][4] = PLAYER_X;
    board.cells[5][3] = PLAYER_X;
    board.cells[6][2] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    test_assert(scores.lengthScore == OPEN_THREE_POINTS, 
                "Open three should score OPEN_THREE_POINTS", results);
    test_assert(scores.positionScore == 40, 
                "Position score should be 40", results);
    
    test_end("EvaluateOpenThree");
}

void testEvaluateCloseThree(TestResults* results) {
    test_begin("EvaluateCloseThree");

    Board board = __prepareBoard();
    board.cells[7][7] = PLAYER_X;
    board.cells[8][8] = PLAYER_X;
    board.cells[9][9] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    test_assert(scores.lengthScore == CLOSED_THREE_POINTS, 
                "Close three should score CLOSED_THREE_POINTS", results);
    test_assert(scores.positionScore == 19, 
                "Position score should be 19", results);
    
    test_end("EvaluateCloseThree");
}

void testEvaluateOpenTwo(TestResults* results) {
    test_begin("EvaluateOpenTwo");

    Board board = __prepareBoard();
    board.cells[4][9] = PLAYER_X;
    board.cells[5][9] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    test_assert(scores.lengthScore == OPEN_TWO_POINTS, 
                "Open two should score OPEN_TWO_POINTS", results);
    test_assert(scores.positionScore == 10, 
                "Position score should be 10", results);
    
    test_end("EvaluateOpenTwo");
}

void testEvaluateCloseTwo(TestResults* results) {
    test_begin("EvaluateCloseTwo");

    Board board = __prepareBoard();
    board.cells[9][1] = PLAYER_X;
    board.cells[9][2] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    test_assert(scores.lengthScore == CLOSED_TWO_POINTS, 
                "Close two should score CLOSED_TWO_POINTS", results);
    test_assert(scores.positionScore == 8, 
                "Position score should be 8", results);
    
    test_end("EvaluateCloseTwo");
}

void testEvaluateDoubleClosedLine(TestResults* results) {
    test_begin("EvaluateDoubleClosedLine");

    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[1][2] = PLAYER_X;
    board.cells[1][3] = PLAYER_X;
    board.cells[1][4] = PLAYER_X;
    board.cells[1][5] = PLAYER_O;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    test_assert(scores.lengthScore == 0, 
                "Double closed line should score 0", results);
    test_assert(scores.positionScore == 17, 
                "Position score should be 17", results);
    
    test_end("EvaluateDoubleClosedLine");
}

void testEvaluateMultiLine(TestResults* results) {
    test_begin("EvaluateMultiLine");

    Board board = __prepareBoard();
    board.cells[1][6] = PLAYER_X;
    board.cells[1][7] = PLAYER_X;
    board.cells[1][8] = PLAYER_X;
    board.cells[1][9] = PLAYER_X;
    board.cells[2][7] = PLAYER_X;
    board.cells[3][7] = PLAYER_X;
    board.cells[4][7] = PLAYER_X;
    EvaluationScores scores = __evaluateStones(&board, PLAYER_X);

    test_assert(scores.lengthScore == CLOSED_FOUR_POINTS * 2 + CLOSED_TWO_POINTS * 2, 
                "Multi line should score CLOSED_FOUR_POINTS * 2 + CLOSED_TWO_POINTS * 2", results);
    test_assert(scores.positionScore == 42, 
                "Position score should be 42", results);
    
    test_end("EvaluateMultiLine");
}

void testNegaMax(TestResults* results) {
    test_begin("NegaMax");

    Board board = __prepareBoard();
    board.cells[2][2] = PLAYER_X;
    board.cells[2][3] = PLAYER_X;
    board.cells[2][4] = PLAYER_X;
    board.cells[2][5] = PLAYER_X;
    board.cells[7][9] = PLAYER_O;
    board.cells[8][9] = PLAYER_O;

    int bestRow = -1, bestCol = -1;
    negaMax(&board, NEGA_MAX_DEPTH, PLAYER_X, &bestRow, &bestCol, -9999999, 9999999);
    
    test_assert(bestRow == 2, 
                "NegaMax should choose row 2 for best move", results);
    test_assert(bestCol == 6, 
                "NegaMax should choose column 6 for best move", results);
    
    test_end("NegaMax");
}

void runCPUTests() {
    TestResults results = {0, 0, 0};
    test_suite_begin("CPU Tests");
    
    suppress_output();
    
    testEvaluateOpenFour(&results);
    testEvaluateCloseFour(&results);
    testEvaluateOpenThree(&results);
    testEvaluateCloseThree(&results);
    testEvaluateOpenTwo(&results);
    testEvaluateCloseTwo(&results);
    testEvaluateDoubleClosedLine(&results);
    testEvaluateMultiLine(&results);
    testNegaMax(&results);
    
    restore_output();
    
    test_suite_end("CPU Tests", &results);
}
