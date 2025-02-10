#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "../include/game.h"
#include "test_utils.h"
#include "test_game.h"



void testHasWinnerInVerticalExpected()
 {
    Board board_1 = __prepareBoard();

    board_1.cells[1][1] = PLAYER_X;
    board_1.cells[2][1] = PLAYER_X;
    board_1.cells[3][1] = PLAYER_X;
    board_1.cells[4][1] = PLAYER_X;
    board_1.cells[5][1] = PLAYER_X;

    assert(hasWinner(&board_1, PLAYER_X) == TRUE);
    assert(hasWinner(&board_1, PLAYER_O) == FALSE);
    assert(getWinner(&board_1) == PLAYER_X);

    Board board_2 = __prepareBoard();

    board_2.cells[4][7] = PLAYER_O;
    board_2.cells[5][7] = PLAYER_O;
    board_2.cells[6][7] = PLAYER_O;
    board_2.cells[7][7] = PLAYER_O;
    board_2.cells[8][7] = PLAYER_O;

    assert(hasWinner(&board_2, PLAYER_O) == TRUE);
    assert(hasWinner(&board_2, PLAYER_X) == FALSE);
    assert(getWinner(&board_2) == PLAYER_O);

 }

void testHasWinnerInHorizontalExpected() {
    Board board_1 = __prepareBoard();

    board_1.cells[1][1] = PLAYER_X;
    board_1.cells[1][2] = PLAYER_X;
    board_1.cells[1][3] = PLAYER_X;
    board_1.cells[1][4] = PLAYER_X;
    board_1.cells[1][5] = PLAYER_X;

    assert(hasWinner(&board_1, PLAYER_X) == TRUE);
    assert(hasWinner(&board_1, PLAYER_O) == FALSE);
    assert(getWinner(&board_1) == PLAYER_X);

    Board board_2 = __prepareBoard();

    board_2.cells[8][5] = PLAYER_O;
    board_2.cells[8][6] = PLAYER_O;
    board_2.cells[8][7] = PLAYER_O;
    board_2.cells[8][8] = PLAYER_O;
    board_2.cells[8][9] = PLAYER_O;

    assert(hasWinner(&board_2, PLAYER_O) == TRUE);
    assert(hasWinner(&board_2, PLAYER_X) == FALSE);
    assert(getWinner(&board_2) == PLAYER_O);

}

void testHasWinnerInCrossExpected() {
    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[2][2] = PLAYER_X;
    board.cells[3][3] = PLAYER_X;
    board.cells[4][4] = PLAYER_X;
    board.cells[5][5] = PLAYER_X;

    assert(hasWinner(&board, PLAYER_X) == TRUE);
    assert(hasWinner(&board, PLAYER_O) == FALSE);
    assert(getWinner(&board) == PLAYER_X);

}

void testNotHasWinnerExpected() {
    Board board = __prepareBoard();

    board.cells[1][1] = PLAYER_X;
    board.cells[2][2] = PLAYER_O;
    board.cells[3][3] = PLAYER_X;
    board.cells[4][4] = PLAYER_X;
    board.cells[5][4] = PLAYER_X;
    board.cells[5][5] = PLAYER_X;
    board.cells[5][6] = PLAYER_X;
    board.cells[6][6] = PLAYER_X;
    board.cells[7][7] = EMPTY_CELL;
    board.cells[8][8] = PLAYER_X;
    

    assert(hasWinner(&board, PLAYER_X) == FALSE);
    assert(hasWinner(&board, PLAYER_O) == FALSE);
    assert(getWinner(&board) == EMPTY_CELL);

}

void runGameTests() {
    printf("Start runGameTests...\n");
    testHasWinnerInVerticalExpected();
    testHasWinnerInHorizontalExpected();
    testHasWinnerInCrossExpected();
    testNotHasWinnerExpected();
    printf("Finished runGameTests.\n");
}

