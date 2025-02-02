#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "../include/plays.h"
#include "../include/game.h"
#include "test_game.h"



/* void testHasWinnerExpected()
 {
    Board board = __prepareBoard();

    board.cells[1][1] = PLAYER_X;
    board.cells[2][1] = PLAYER_X;
    board.cells[3][1] = PLAYER_X;
    board.cells[4][1] = PLAYER_X;
    board.cells[5][1] = PLAYER_X;

    assert(hasWinner(&board) == 1);
 } */

void runGameTests() {
    printf("Start runGameTests...\n");
    //testHasWinnerExpected();
    printf("Finished runGameTests.\n");
}

