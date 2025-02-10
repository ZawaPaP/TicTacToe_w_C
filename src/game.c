#include <stdio.h>
#include "plays.h"
#include "board.h"
#include "queue.h"
#include "game.h"


void printGameStatus(int turnCounts, char player);
int __isInBoardRange(int r, int c);
int __isSameMark(char playerMark, char currCellMark);
int __hasWinnerInDirection(Board *board, char playerMark, int startX, int startY, int move[2], int (*visited)[BOARD_COLUMNS + 1]);
int __hasWinnerInVertical(Board *board, char playerMark);
int __hasWinnerInHorizontal(Board *board, char playerMark);
int __hasWinnerInCross(Board *board, char playerMark);

void playGame()
{
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

int hasWinner(Board *board, char playerMark) {

    if (playerMark == EMPTY_CELL)
        return 0;

    return (
        __hasWinnerInVertical(board, playerMark) ||
        __hasWinnerInHorizontal(board, playerMark) ||
        __hasWinnerInCross(board, playerMark));
}

int __hasWinnerInVertical(Board *board, char playerMark) {
    int move[2] = {1, 0};
    int visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1] = {};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (__hasWinnerInDirection(board, playerMark, i, j, move, visited) == 1) {
                return 1;
            }
        }
    }
    return 0;
}

int __hasWinnerInHorizontal(Board *board, char playerMark) {
    int move[2] = {0, 1};
    int visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1] = {};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (__hasWinnerInDirection(board, playerMark, i, j, move, visited) == 1) {
                return 1;
            }
        }
    }
    return 0;
}

int __hasWinnerInCross(Board *board, char playerMark) {
    int move[2][2] = {{1, 1}, {1, -1}};
    int visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1] = {};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            for (int k = 0; k < 2; k++) {
                if (__hasWinnerInDirection(board, playerMark, i, j, move[k], visited) == 1)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int __hasWinnerInDirection(Board *board, char playerMark, int startX, int startY, int move[2], int (*visited)[BOARD_COLUMNS + 1]) {
    Queue queue;
    initQueue(&queue);

    int sequence = 1;

    if (board->cells[startX][startY] != playerMark)
        return 0;

    if (visited[startX][startY] == 1) {
        return 0;
    }
    Position pos = {startX, startY};

    push(&queue, pos);
    visited[startX][startY] = 1;

    while(!isEmpty(&queue)) {
        Position currPos = pop(&queue);

        Position nextCell = {currPos.x + move[0], currPos.y + move[1]};
        if (__isInBoardRange(nextCell.x, nextCell.y) == 1 && visited[nextCell.x][nextCell.y] == 0)
        {
            char currMark = board->cells[nextCell.x][nextCell.y];
            if (__isSameMark(playerMark, currMark))
            {
                sequence += 1;
                push(&queue, nextCell);
                visited[nextCell.x][nextCell.y] = 1;
            }

            if (sequence >= WIN_LENGTH)
                return 1;
        }
    }
    return 0;
}

int __isInBoardRange(int r, int c) {
    if (1 <= r && r <= BOARD_ROWS && 1 <= c && c <= BOARD_COLUMNS)
        return 1;
    return 0;
}

int __isSameMark(char playerMark, char currCellMark) {
    return playerMark == currCellMark;
}
