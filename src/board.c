#include <stdio.h>
#include "board.h"

static const Direction DIRS[4] = {
    {1, 0},   // 垂直方向 (↓)
    {0, 1},   // 水平方向 (→)
    {1, 1},   // 右下がり斜め (↘)
    {1, -1}   // 右上がり斜め (↗)
};

static const int dirLength = 4;

void initBoard(Board *board) {
    size_t i, j;
    for (i = 1; i <= BOARD_ROWS; i++) {
        for (j = 1; j <= BOARD_COLUMNS; j++) {
            board->cells[i][j] = EMPTY_CELL;
        }
    }
}

BOOL boardIsFull(Board *board) {
    size_t i, j;
    for (i = 1; i <= BOARD_ROWS; i++) {
        for (j = 1; j <= BOARD_COLUMNS; j++) {
            if (board->cells[i][j] == EMPTY_CELL)
                return FALSE;
        }
    }
    return TRUE;
}

BOOL isInBoard(int r, int c) {
    if (1 <= r && r <= BOARD_ROWS && 1 <= c && c <= BOARD_COLUMNS)
        return TRUE;
    return FALSE;
}

// 特定の方向の連続した石の数を数える共通関数
static int countContinuousStones(Board *board, int r, int c, int dx, int dy, char playerMark) {
    int length = 1;  // 置いた石から開始
    
    // 両方向（正負）に探索
    for (int reverse = 0; reverse <= 1; reverse++) {
        int nextX = r + (reverse == 0 ? -dx : dx);
        int nextY = c + (reverse == 0 ? -dy : dy);
        
        while (isInBoard(nextX, nextY) && board->cells[nextX][nextY] == playerMark) {
            length++;
            nextX += (reverse == 0 ? -dx : dx);
            nextY += (reverse == 0 ? -dy : dy);
        }
    }
    
    return length;
}

BOOL isWinMove(Board *board, int r, int c, char playerMark) {
    if (playerMark == EMPTY_CELL)
        return FALSE;
        
    for (int dir = 0; dir < dirLength; dir++) {
        int length = countContinuousStones(board, r, c, DIRS[dir].dx, DIRS[dir].dy, playerMark);
        
        if ((playerMark == PLAYER_O && length >= 5) || length == 5)
            return TRUE;
    }
    return FALSE;
}

BOOL isMakingOverLine(Board *board, int r, int c, char playerMark) {
    if (playerMark != PLAYER_X)
        return FALSE;
        
    for (int dir = 0; dir < dirLength; dir++) {
        int length = countContinuousStones(board, r, c, DIRS[dir].dx, DIRS[dir].dy, playerMark);
        
        if (length >= 6)
            return TRUE;
    }
    return FALSE;
}

BOOL __isSameLine(Board* board, lineInfo* line1, lineInfo* line2, int row, int col, int dx, int dy) {
    // Gapなしのラインが2種類以上あることはない。
    if (!line1->hasGap && !line2->hasGap) {
        return TRUE;
    }

    // row, colから見た時、石の位置が全て同じであれば、同じライン
    // これは2つのlineで被っていないidx部分が全てEmptyであることと同義
    // なぜなら、ここでのlineは含まれる石の数が同じかつgapが1つあることが約束されているから。
    // ex:
    // 同じライン
    // line1: _O_OT  idx(-4, 0)
    // line2:  O_OT_ idx(-3, 1)
    //
    // 違うライン
    // line1: O_OT_ idx(-3, 1)
    // line2: T_OO_ idx(0, 4)

    int startIdx, endIdx, startIdxGap, endIdxGap;
    if (line1->startIdx < line2->startIdx)
    {
        startIdx = line1->startIdx;
        startIdxGap = line2->startIdx;
        endIdx = line1->endIdx;
        endIdxGap = line2->endIdx;
    } else {
        startIdx = line2->startIdx;
        startIdxGap = line1->startIdx;
        endIdx = line2->endIdx;
        endIdxGap = line1->endIdx;
    }

    for (int i = startIdx; i < startIdxGap; i++) {
        if (board->cells[row + (i * dx)][col + (i * dy)] != EMPTY_CELL)
            return FALSE;
    }

        for (int i = endIdx; i < endIdxGap; i++) {
        if (board->cells[row + (i * dx)][col + (i * dy)] != EMPTY_CELL)
            return FALSE;
    }
    return TRUE;
}


lineInfoArray __getTargetLengthLinesInDirectionCandidates(Board *board, int row, int col, int length, int dx, int dy, char playerMark) {
    /*
    T: target 石を置く場所
    としたときに、長さlengthのラインであれば、lineInfoをarrayで返す
        1. Tを含むlength + 1 の範囲にある石が最大でlength個かつ、残りの一つがEmptyであること
        ex.) length = 3
        {
        ___T,
         __T_,
          _T__,
           T___
        }
    */
    lineInfoArray result = {0};
    board->cells[row][col] = playerMark;

    // length+1マスのウィンドウを1マスずつずらしながら探索
    for (int i = 0; i <= length; i++) {
        int stones = 0;
        BOOL gap = FALSE;
        BOOL isValid = TRUE;
        
        // 現在のウィンドウ内の石を確認
        for (int j = i - length; j <= i; j++) {
            int x = row + (j * dx);
            int y = col + (j * dy);

            if (!isInBoard(x, y)) {
                isValid = FALSE;
                break;
            }

            if (board->cells[x][y] != playerMark && 
                board->cells[x][y] != EMPTY_CELL) {
                isValid = FALSE;
                break;
            }

            if (board->cells[x][y] == playerMark) {
                stones++;
            }

            if (board->cells[x][y] == EMPTY_CELL && 
                (j != (i - length) && j != i)) {
                gap = TRUE;
            }
        }
        if (isValid && stones == length) {
            lineInfo line = {
                .startIdx = i - length,
                .endIdx = i,
                .hasGap = gap
            };
            result.lines[result.count] = line;
            result.count++;
        }
    }

    board->cells[row][col] = EMPTY_CELL;
    return result;
}


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
