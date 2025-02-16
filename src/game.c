#include <stdio.h>
#include "plays.h"
#include "board.h"
#include "cpu.h"
#include "queue.h"
#include "game.h"

void playGame(int mode);
char getWinner(Board *board);
BOOL __isDrawGame(Board *board);

MODE selectGameMode() {
    MODE mode;
    while(1) {
        printf("Choose game mode: (1) Player vs Player, (2) Player vs CPU, (3) CPU vs CPU: : ");
        int tempMode;
        if (scanf("%d", &tempMode) != 1)
        {
            printf("Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n'); // 入力バッファをクリア
            continue;
        }
        mode = (MODE)tempMode;
        if (mode == PLAYER_PLAYER || mode == PLAYER_CPU || mode == CPU_CPU)
        {
            while (getchar() != '\n'); 
            return mode;
        }
        printf("Invalid choice. Please enter 1, 2 or 3.\n");
    }
}

BOOL shouldPlayAgain() {
    char response;
    while(1) {
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &response);
        while (getchar() != '\n');

        if (response == 'y' || response == 'Y') {
            return TRUE;
        }
        if (response == 'n' || response == 'N') {
            return FALSE;
        }
        printf("Invalid input. Please enter 'y' or 'n'.\n");
    }
}

void runGameLoop() {
    while (1) {
        MODE mode = selectGameMode();

        playGame(mode);

        if (!shouldPlayAgain()) {
            printf("\tThanks for playing!\n");
            break;
        }
    }
}

void playGame(int mode)
{
    Board board;
    initBoard(&board);
    int row, col;
    char currentPlayer = PLAYER_X;
    int turnCounts = 1;

    printf("\n\tTicTacToe Game Started!\n\n");
    printBoard(&board);
    while(1) {
        printGameStatus(turnCounts, currentPlayer);

        if ((mode == PLAYER_CPU && currentPlayer == PLAYER_O) || mode == CPU_CPU)
            getCpuMove(&row, &col, &board, currentPlayer);

        else {
            while (1)
            {
                if(isValidMoveInput(&row, &col)) {
                    if (canApplyMove(row, col, &board)) {
                        break;
                    }
                }
            }
        }
        board.cells[row][col] = currentPlayer;
        board.lastRow = row;
        board.lastCol = col;
        turnCounts++;

        printBoard(&board);
        printf("Player %c placed at: %d, %d\n", currentPlayer, row, col);
        if (isWinMove(&board, row, col, currentPlayer)) {
            printWinner(currentPlayer);
            break;
        }
        if (__isDrawGame(&board)) {
            printDrawGame();
            break;
        }
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
}


BOOL isGameOver(Board *board, int* row, int* col, char player) {
    return __isDrawGame(board) || isWinMove(board, *row, *col, player);
}

BOOL __isDrawGame(Board *board) {
    return boardIsFull(board);
}
