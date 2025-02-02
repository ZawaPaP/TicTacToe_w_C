#include <stdio.h>
#include "plays.h"
#include "board.h"

void printGameStatus(int turnCounts, char player);

void playGame() {    
    Board board;
    initBoard(&board);
    int row, col;
    char currentPlayer = PLAYER_X;
    int turnCounts = 0;

    printf("\n        TicTacToe Game Started!\n\n");

    while(1) {
        turnCounts += 1;
        printBoard(&board);
        printGameStatus(turnCounts, currentPlayer);
        while (1)
        {
            if(getInput(&row, &col)) {
                if (placeMove(row, col, &board, currentPlayer)) {
                    break;
                }
            }
        }
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
}

void printGameStatus(int turnCounts, char player) {
    printf("Turn %d, %c's tarn.\n",turnCounts, player);    
}

int hasWinner(Board *board) {
    printf("hasWinner! %d\n", board->cells[1][1]);
    return 1;
}
