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
BOOL __isSameMark(char playerMark, char currCellMark);
BOOL __hasWinnerInDirection(Board *board, char playerMark, int startX, int startY, int move[2], int (*visited)[BOARD_COLUMNS + 1]);
BOOL __hasWinnerInVertical(Board *board, char playerMark);
BOOL __hasWinnerInHorizontal(Board *board, char playerMark);
BOOL __hasWinnerInCross(Board *board, char playerMark);


/* static const Direction DIRS[4] = {
    {0, 1},    // 横
    {1, 0},    // 縦
    {1, 1},    // 右斜め下
    {1, -1}    // 左斜め下
};
 */

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

/* int countStonesWithGaps(Board* board, int x, int y){
    int gap = 0;

    for (int d = 0; d < 4; d++)
    {
        int dx = 0, dy = 0;
        switch(d) {
            case 0: dx = 0; dy = 1; break;  // 横方向
            case 1: dx = 1; dy = 0; break;  // 縦方向
            case 2: dx = 1; dy = 1; break;  // 右斜め
            case 3: dx = 1; dy = -1; break; // 左斜め
        }
        int prevX = x - dx;
        int prevY = y - dy;
        int nextX = x + dx;
        int nextY = y + dy;

        int prevGapLength = 1;
        int nextGapLength = 1;
    }
    return 1;
} */


lineInfoArray __getTargetLengthLinesInDirection(Board *board, int row, int col, int length, int dx, int dy, char playerMark) {
    /*
    dx, dy方向にちょうどlengthの長さと見做せるlineのlineInfoをarrayで返す。
    前提: 複数存在するケースもある。

    T: target 石を置く場所
    としたときに、Tが長さlengthのラインであるかどうかの判定
        candidates lineのうち、石の間にEmptyがあるものをGapがある、とする。
        Gapがない場合は、Tから見てEmptyを含む側の外側がEmpty,含まない側の外側2マスがEmptyであること
        Gapがある場合は、Tから見てlineの両端がEmptyであること
        2を満たす例 ( | はcandidatesのline両端を表す)
            Gapなし
                _|_XXT|__,
                _|_XTX|__,
                __|TXX_|_,
            Gapあり
                _|X_XT|_,
                _|XT_X|_,
    */
    lineInfoArray candidates = __getTargetLengthLinesInDirectionCandidates(board, row, col, length, dx, dy, playerMark);
        
    lineInfoArray result = {0};

    for (int i = 0; i < candidates.count; i++)
    {
        lineInfo candidate = candidates.lines[i];
        BOOL isValid = FALSE;
        
        // ラインの両端の外側をチェック
        int startX = row + ((candidate.startIdx - 1) * dx);
        int startY = col + ((candidate.startIdx - 1) * dy);
        int endX = row + ((candidate.endIdx + 1) * dx);
        int endY = col + ((candidate.endIdx + 1) * dy);
        
        if (candidate.hasGap) {
            // 飛び石パターンの場合：両端が空いている必要がある
            if (__isInBoardRange(startX, startY) && 
                __isInBoardRange(endX, endY) &&
                board->cells[startX][startY] == EMPTY_CELL &&
                board->cells[endX][endY] == EMPTY_CELL) {
                isValid = TRUE;
            }
        } else {
            // 連続パターンの場合：片方の端が2マス空いている必要がある
            int startX2 = row + ((candidate.startIdx - 2) * dx);
            int startY2 = col + ((candidate.startIdx - 2) * dy);
            int endX2 = row + ((candidate.endIdx + 2) * dx);
            int endY2 = col + ((candidate.endIdx + 2) * dy);
            
            // 始点側が空いているケース
            if (__isInBoardRange(startX, startY) && 
                __isInBoardRange(startX2, startY2) &&
                board->cells[startX][startY] == EMPTY_CELL &&
                board->cells[startX2][startY2] == EMPTY_CELL) {
                isValid = TRUE;
            }
            
            // 終点側が空いているケース
            if (__isInBoardRange(endX, endY) && 
                __isInBoardRange(endX2, endY2) &&
                board->cells[endX][endY] == EMPTY_CELL &&
                board->cells[endX2][endY2] == EMPTY_CELL) {
                isValid = TRUE;
            }
        }

        if (isValid) {
            BOOL isDuplicate = FALSE;
    
            // 既存のラインとの重複チェック
            for (int j = 0; j < result.count; j++) {
                if (__isSameLine(board, &candidate, &result.lines[j], row, col, dx, dy)) {
                    isDuplicate = TRUE;
                    break;
                }
            }
    
            if (!isDuplicate || result.count == 0) {
                result.lines[result.count] = candidate;
                result.count++;
            }
        }
    }

    return result;
}

    /* BOOL __isDoubleThree(Board *board, int x, int y) {
        for (int d = 0; d < 4; d++)
        {
            int dx = 0, dy = 0;
            switch(d) {
                case 0: dx = 0; dy = 1; break;  // 横方向
                case 1: dx = 1; dy = 0; break;  // 縦方向
                case 2: dx = 1; dy = 1; break;  // 右斜め
                case 3: dx = 1; dy = -1; break; // 左斜め
            }
        }

    } */



BOOL __isSameMark(char playerMark, char currCellMark) {
    return (playerMark == currCellMark) ? TRUE : FALSE;
}
