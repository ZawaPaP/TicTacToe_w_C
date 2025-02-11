#include <stdio.h>
#include "plays.h"
#include "board.h"
#include "cpu.h"
#include "queue.h"
#include "game.h"

void playGame(int mode);
void printGameStatus(int turnCounts, char player);
void printWinner(Board *board);
void printDrawGame();
char getWinner(Board *board);
BOOL hasWinner(Board *board, char playerMark);
BOOL __isDrawGame(Board *board);
BOOL __isInBoardRange(int r, int c);
BOOL __isSameMark(char playerMark, char currCellMark);
BOOL __hasWinnerInDirection(Board *board, char playerMark, int startX, int startY, int move[2], int (*visited)[BOARD_COLUMNS + 1]);
BOOL __hasWinnerInVertical(Board *board, char playerMark);
BOOL __hasWinnerInHorizontal(Board *board, char playerMark);
BOOL __hasWinnerInCross(Board *board, char playerMark);


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

        if (mode == PLAYER_CPU && currentPlayer == PLAYER_O) {
            getCpuMove(&row, &col, &board);
            board.cells[row][col] = currentPlayer;
            turnCounts++;
        } else if (mode == CPU_CPU) {
            getCpuMove(&row, &col, &board);
            board.cells[row][col] = currentPlayer;
            turnCounts++;
        }

        else {
            while (1)
            {
                if(isValidMoveInput(&row, &col)) {
                    if (canApplyMove(row, col, &board)) {
                        board.cells[row][col] = currentPlayer;
                        turnCounts++;
                        break;
                    }
                }
            }
        }
        printBoard(&board);
        printf("Player %c placed at: %d, %d\n", currentPlayer, row, col);
        if (hasWinner(&board, currentPlayer)) {
            printWinner(&board);
            break;
        }
        if (__isDrawGame(&board)) {
            printDrawGame();
            break;
        }
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
}


void printGameStatus(int turnCounts, char player) {
    printf("Turn %d, %c's turn.\n",turnCounts, player);    
}

void printWinner(Board *board) {
    char winner = getWinner(board);
    if (winner == ' ') {
        printf("No winner yet.\n");
    } else {
        printf("\t\tWinner is %c!\n\n", winner);
    }
}

void printDrawGame() {
    printf("\tDrow. Nice game!\n\n");
}

BOOL isGameOver(Board *board) {
    return __isDrawGame(board) || getWinner(board) != EMPTY_CELL;
}

char getWinner(Board *board) {
    if (hasWinner(board, PLAYER_X))
        return PLAYER_X;
    if (hasWinner(board, PLAYER_O))
        return PLAYER_O;
    return EMPTY_CELL;
}

BOOL __isDrawGame(Board *board) {
    if (getWinner(board) != EMPTY_CELL)
        return FALSE;
    return boardIsFull(board);
}

BOOL hasWinner(Board *board, char playerMark) {

    if (playerMark == EMPTY_CELL)
        return FALSE;

    return (
               __hasWinnerInVertical(board, playerMark) ||
               __hasWinnerInHorizontal(board, playerMark) ||
               __hasWinnerInCross(board, playerMark))
               ? TRUE
               : FALSE;
}

BOOL __hasWinnerInVertical(Board *board, char playerMark) {
    int move[2] = {1, 0};
    int visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1] = {};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (__hasWinnerInDirection(board, playerMark, i, j, move, visited) == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL __hasWinnerInHorizontal(Board *board, char playerMark) {
    int move[2] = {0, 1};
    int visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1] = {};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (__hasWinnerInDirection(board, playerMark, i, j, move, visited) == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL __hasWinnerInCross(Board *board, char playerMark) {
    int move[2][2] = {{1, 1}, {1, -1}};
    int visited[BOARD_ROWS + 1][BOARD_COLUMNS + 1] = {};

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            for (int k = 0; k < 2; k++) {
                if (__hasWinnerInDirection(board, playerMark, i, j, move[k], visited) == 1)
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL __hasWinnerInDirection(Board *board, char playerMark, int startX, int startY, int move[2], int (*visited)[BOARD_COLUMNS + 1]) {
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
                return TRUE;
        }
    }
    return FALSE;
}


BOOL __isInBoardRange(int r, int c) {
    if (1 <= r && r <= BOARD_ROWS && 1 <= c && c <= BOARD_COLUMNS)
        return TRUE;
    return FALSE;
}

BOOL __isSameMark(char playerMark, char currCellMark) {
    return (playerMark == currCellMark) ? TRUE : FALSE;
}
