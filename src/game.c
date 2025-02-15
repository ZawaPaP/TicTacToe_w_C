#include <stdio.h>
#include "plays.h"
#include "board.h"
#include "cpu.h"
#include "queue.h"
#include "game.h"

void playGame(int mode);
char getWinner(Board *board);
BOOL __isDrawGame(Board *board);

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
            if (isInBoard(startX, startY) && 
                isInBoard(endX, endY) &&
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
            if (isInBoard(startX, startY) && 
                isInBoard(startX2, startY2) &&
                board->cells[startX][startY] == EMPTY_CELL &&
                board->cells[startX2][startY2] == EMPTY_CELL) {
                isValid = TRUE;
            }
            
            // 終点側が空いているケース
            if (isInBoard(endX, endY) && 
                isInBoard(endX2, endY2) &&
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

// 3や4に見えて長連を作ってしまうラインかどうかを判定。
BOOL __wouldCreateOverline(Board* board, int row, int col, int dx, int dy, char playerMark) {
    board->cells[row][col] = playerMark;
    
    // dx, dyの正もしくは負方向に対して、連続する形でもう一つ石を置いたときに、
    // どうしても連続する石が6を超えてしまう時は三や四とは見做されない。

    // 左右それぞれの方向で、連続する石の数を数える
    int continuous[2] = {0, 0};         // gapまでの連続した石 [左, 右]
    int continuousOverGap[2] = {0, 0};  // gap後の連続した石 [左, 右]
    int filledGap[2] = {0, 0}; // 左右それぞれで、石を置けたかどうか

    for (int dir = 0; dir <= 1; dir++) {  // dir: 0=左方向, 1=右方向
        int tx = row + (dir == 0 ? -dx : dx);
        int ty = col + (dir == 0 ? -dy : dy);
        
        while (isInBoard(tx, ty)) {
            if (board->cells[tx][ty] == playerMark) {
                if (filledGap[dir] == 0) {
                    continuous[dir]++;
                } else {
                    continuousOverGap[dir]++;
                }
            } 
            else if (board->cells[tx][ty] == EMPTY_CELL) {
                if (filledGap[dir] > 0) break;  // 2つ目のgapで終了
                filledGap[dir]++;
            }
            else {
                break;
            }
            
            tx += (dir == 0 ? -dx : dx);
            ty += (dir == 0 ? -dy : dy);
        }
    }
    board->cells[row][col] = EMPTY_CELL;

    int leftGapLength = continuousOverGap[0] + continuous[0] + continuous[1] + 1;
    int rightGapLength = continuous[0] + continuous[1] + continuousOverGap[1] + 1;

    if (filledGap[0] == 0 && filledGap[1] == 0) {
        // 左右ともに石を置く場所がないため、FALSE
        return FALSE;
    }
    else if (filledGap[0] == 0 || filledGap[1] == 0) {
        // 石の置き方は1通りしかない。
        return leftGapLength + rightGapLength - 1 >= 6;
    }
    else {
        return leftGapLength >= 6 && rightGapLength >= 6;
    }
}

lineInfoArray __getEffectiveLines(Board *board, int row, int col, lineInfoArray* candidates, int dx, int dy, char playerMark) {
    lineInfoArray effectiveLines = {0};
    
    printf("Checking %d candidate lines for effectiveness in direction (%d,%d)\n", 
           candidates->count, dx, dy);
    
    for (int i = 0; i < candidates->count; i++) {
        lineInfo candidate = candidates->lines[i];
        printf("\nChecking candidate line %d: startIdx=%d endIdx=%d hasGap=%d\n", 
               i + 1, candidate.startIdx, candidate.endIdx, candidate.hasGap);
        
        // 長連チェック
        if (__wouldCreateOverline(board, row, col, dx, dy, playerMark)) {
            printf("Line would create overline\n");
            continue;
        }
        
        printf("Adding effective line: startIdx=%d endIdx=%d hasGap=%d\n", 
               candidate.startIdx, candidate.endIdx, candidate.hasGap);
        effectiveLines.lines[effectiveLines.count++] = candidate;
    }
    
    printf("Found %d effective lines\n", effectiveLines.count);
    return effectiveLines;
}
