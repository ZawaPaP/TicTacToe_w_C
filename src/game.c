#include <stdio.h>
#include "ui.h"
#include "board.h"
#include "cpu.h"
#include "queue.h"
#include "game.h"

void playGame(int mode);
char getWinner(Board *board);
BOOL __isDrawGame(Board *board);

void initGame(Game* game, MODE mode) {
    initBoard(&game->board);
    game->currentPlayer = PLAYER_X;
    game->gameState = GAME_PLAYING;
    game->winner = EMPTY_CELL;
    game->moveCount = 0;
    game->gameMode = mode;
}

void switchPlayer(Game* game) {
    game->currentPlayer = (game->currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

BOOL isGameOver(Board *board, int *row, int *col, char player) {
    return isWinMove(board, *row, *col, player) || boardIsFull(board);
}

BOOL isValidMove(Board* board, int row, int col, char playerMark) {
    if (row < 1 || BOARD_ROWS < row || col < 1 || BOARD_COLUMNS < col) {
        printf("Error: Row and Column must be between 1 and %d.\n", BOARD_ROWS);
        return FALSE;
    }

    if (board->cells[row][col] != EMPTY_CELL) {
        printf("Error: The %d, %d is already marked.\n", row, col);
        return FALSE;
    }

    if (isProhibitedMove(board, row, col, playerMark)) {
        printf("Error: The %d, %d is prohibited move.\n", row, col);
        return FALSE;
    }
    return TRUE;
}

void handlePlayerMove(Game *game, int* row, int* col) {
    while(TRUE) {
        if (handlePlayerInput(row, col)) {
            if (isValidMove(&game->board, *row, *col, game->currentPlayer)) {
                return;
            }
        }
    }
}

void handleCPUMove(Game* game, int* row, int* col) {
    getCpuMove(&game->board, row, col, game->currentPlayer);
    return;
}

void applyMove(Game* game, int row, int col) {
    game->board.cells[row][col] = game->currentPlayer;
    Move move = {.move.r = row, .move.c = col, .player = game->currentPlayer};
    game->moveHistory[game->moveCount] = move;
    game->moveCount++;

    printf("%c placed at: %d, %d\n", game->currentPlayer, row, col);
}

void updateGameState(Game *game, int row, int col){
    if (isWinMove(&game->board, row, col, game->currentPlayer)) {
        game->gameState = GAME_WIN;
        game->winner = game->currentPlayer;
    }
    else if (boardIsFull(&game->board)) {
        game->gameState = GAME_DRAW;
    }
}


void playTurn(Game* game) {
    // 1ターンの処理
    int row, col;
    printGameStatus(game->moveCount + 1, game->currentPlayer);

    if (
        (
            game->gameMode == PLAYER_CPU &&
            game->currentPlayer == PLAYER_O) ||
        game->gameMode == CPU_CPU) {
        handleCPUMove(game, &row, &col);
    } else {
        handlePlayerMove(game, &row, &col);
    }

    applyMove(game, row, col);
    updateGameState(game, row, col);
    printBoard(game);
}


void playGame(int mode)
{

    Game game;
    initGame(&game, mode);

    printf("\n\tRenju Game Started!\n\n");
    printBoard(&game);

    while (game.gameState == GAME_PLAYING) {
        playTurn(&game);

        if (game.gameState == GAME_PLAYING) {
            switchPlayer(&game);
        }
    }
}

void runGameLoop() {
    while (TRUE) {
        MODE mode = selectGameMode();

        playGame(mode);

        if (!askForRematch()) {
            displayThanksMessage();
            break;
        }
    }
}
