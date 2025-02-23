#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "../include/game.h"
#include "test_utils.h"
#include "test_game.h"

void testGameInitialization(TestResults* results) {
    test_begin("GameInitialization");
    
    Game game;
    initGame(&game, PLAYER_PLAYER);
    
    test_assert(game.currentPlayer == PLAYER_X,
                "Game should start with PLAYER_X", results);
    test_assert(game.gameState == GAME_PLAYING,
                "Game should start in PLAYING state", results);
    test_assert(game.moveCount == 0,
                "Move count should start at 0", results);
    test_assert(game.gameMode == PLAYER_PLAYER,
                "Game mode should be the PLAYER_PLAYER", results);
                
    test_end("GameInitialization");
}

void testPlayerSwitch(TestResults* results) {
    test_begin("PlayerSwitch");
    
    Game game;
    initGame(&game, PLAYER_PLAYER);
    
    switchPlayer(&game);
    test_assert(game.currentPlayer == PLAYER_O,
                "Should switch to PLAYER_O", results);
    
    switchPlayer(&game);
    test_assert(game.currentPlayer == PLAYER_X,
                "Should switch back to PLAYER_X", results);
    
    test_end("PlayerSwitch");
}

void testPlaceMoveExpected(TestResults* results) {
    test_begin("PlaceMoveExpected");

    Board board = __prepareBoard();
    test_assert(isValidMove(&board, 5, 5, PLAYER_X) == TRUE,
                "Should allow move to empty center position", results);

    test_assert(isValidMove(&board, 1, 1, PLAYER_X) == TRUE,
                "Should allow move to top-left corner", results);
    
    test_assert(isValidMove(&board, BOARD_ROWS, 1, PLAYER_X) == TRUE,
                "Should allow move to bottom-left corner", results);

    test_assert(isValidMove(&board, 1, BOARD_COLUMNS, PLAYER_X) == TRUE,
                "Should allow move to top-right corner", results);

    test_assert(isValidMove(&board, BOARD_ROWS, BOARD_COLUMNS, PLAYER_X) == TRUE,
                "Should allow move to bottom-right corner", results);

    test_end("PlaceMoveExpected");
}

void testPlaceMoveFailedAlreadyMarked(TestResults* results) {
    Board board = __prepareBoard();
    board.cells[5][5] = PLAYER_X;

    test_assert(isValidMove(&board, 5, 5, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);
}

void testPlaceMoveFailedOutOfRange(TestResults* results) {
    Board board = __prepareBoard();
    test_assert(isValidMove(&board, BOARD_ROWS + 1, BOARD_COLUMNS, PLAYER_X) == FALSE,
                "Should not allow move out of range", results);
}

void testValidateInputExpectedRange(TestResults* results) {
    test_begin("ValidateInputExpectedRange");

    printf("000000");

    Board board = __prepareBoard();

    int row = 5;
    int col = 5;
    test_assert(isValidMove(&board, row, col, PLAYER_X) == TRUE,
                "Should allow move in valid range", results);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    test_assert(isValidMove(&board, rowEdgeBegin, colEdgeBegin, PLAYER_X) == TRUE,
                "Should allow move to top-left corner", results);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    test_assert(isValidMove(&board, rowEdgeEnd, colEdgeEnd, PLAYER_X) == TRUE,
                "Should allow move to bottom-right corner", results);

    test_end("ValidateInputExpectedRange");
}

void testValidateInputFailedOutOfRange(TestResults* results) {
    test_begin("ValidateInputFailedOutOfRange");

    printf("000000");

    Board board = __prepareBoard();

    int zeroRow = 0;
    int zeroCol = 0;

    test_assert(isValidMove(&board, zeroRow, zeroCol, PLAYER_X) == FALSE,
                "Should not allow move in zero range", results);
    
    int negativeInt = -1;
    int randInt = 4;
    test_assert(isValidMove(&board, negativeInt, randInt, PLAYER_X) == FALSE,
                "Should not allow move in negative range", results);

    int ToobigInt = 123490;
    test_assert(isValidMove(&board, ToobigInt, randInt, PLAYER_X) == FALSE,
                "Should not allow move in too big range", results);

    test_end("ValidateInputFailedOutOfRange");
}

void testValidateInputFailedNotEmpty(TestResults* results) {
    test_begin("ValidateInputFailedNotEmpty");

    Board board = __prepareBoard();

    int row = 5;
    int col = 5;

    board.cells[row][col] = PLAYER_O;
    test_assert(isValidMove(&board, row, col, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    board.cells[rowEdgeBegin][colEdgeBegin] = PLAYER_O;
    test_assert(isValidMove(&board, rowEdgeBegin, colEdgeBegin, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    board.cells[rowEdgeEnd][colEdgeEnd] = PLAYER_O;
    test_assert(isValidMove(&board, rowEdgeEnd, colEdgeEnd, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);

    test_end("ValidateInputFailedNotEmpty");
}



void runGameTests() {
    TestResults results = {0, 0, 0};
    test_suite_begin("Game Tests");
    
    suppress_output();

    testGameInitialization(&results);
    testPlayerSwitch(&results);
    testPlaceMoveExpected(&results);
    testPlaceMoveFailedAlreadyMarked(&results);
    testValidateInputExpectedRange(&results);
    testValidateInputFailedOutOfRange(&results);
    testValidateInputFailedNotEmpty(&results);
    
    restore_output();

    test_suite_end("Game Tests", &results);
}

