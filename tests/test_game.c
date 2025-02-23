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



void runGameTests() {
    TestResults results = {0, 0, 0};
    test_suite_begin("Game Tests");
    
    suppress_output();

    testGameInitialization(&results);
    testPlayerSwitch(&results);

    restore_output();

    test_suite_end("Game Tests", &results);
}

