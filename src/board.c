#include <stdio.h>
#include "utils.h"
#include "board.h"

static const Direction DIRS[4] = {
    {1, 0},   // 垂直方向 (↓)
    {0, 1},   // 水平方向 (→)
    {1, 1},   // 右下がり斜め (↘)
    {1, -1}   // 右上がり斜め (↗)
};

static const int dirLength = 4;

BOOL isProhibitedMove(Board *board, int r, int c, char playerMark);

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

// r, cに石を置いた場合の特定の方向の連続した石を1つのGAPありで数える
LinePatterns countContinuousStonesWithGap(Board *board, int r, int c, int dx, int dy, char playerMark) {
    LinePatterns result = {.pattern = 0};
    for (int gapSide = 0; gapSide <= 1; gapSide++) {  // 0:左側, 1:右側
        int minIdx = 0, maxIdx = 0;
        int length = 1;  // 置いた石から開始
        BOOL gapUsed = FALSE;

        for (int reverse = 0; reverse <= 1; reverse++) {
            int nextX = r + (reverse == 0 ? -dx : dx);
            int nextY = c + (reverse == 0 ? -dy : dy);
            int relativePos = (reverse == 0 ? -1 : 1);
            
            while (isInBoard(nextX, nextY)) {
                if (board->cells[nextX][nextY] == playerMark)
                {
                    length++;
                    minIdx = min(minIdx, relativePos);
                    maxIdx = max(maxIdx, relativePos);
                // 意図した方向のgapの場合は、有効なgapかどうかをチェック。
                }
                else if (!gapUsed &&
                         board->cells[nextX][nextY] == EMPTY_CELL &&
                         ((gapSide == 0 && reverse == 0) || (gapSide == 1 && reverse == 1)))
                {
                    // gapをまだ1度も通っておらず、gapの次の石が自分の石の場合、数え続ける。
                    //　その他の場合は、数えるのをやめる。
                    nextX += (reverse == 0 ? -dx : dx);
                    nextY += (reverse == 0 ? -dy : dy);
                    relativePos += (reverse == 0 ? -1 : 1);
                    if (isInBoard(nextX, nextY) && board->cells[nextX][nextY] == playerMark) {
                        gapUsed = TRUE;
                        continue;
                    } else
                        break;
                }
                else
                {
                    break;
                }
                nextX += (reverse == 0 ? -dx : dx);
                nextY += (reverse == 0 ? -dy : dy);
                relativePos += (reverse == 0 ? -1 : 1);
            }
        }
        // gapを使っている場合、新しい長さとして保存
        // gapを使っていない場合、両サイドともgapがない場合に限り1度だけ保存
        // 片側のみgapがあるのは、長さパターン1つとなる
        if (gapUsed || (result.pattern == 0 && gapSide == 1)) {
            result.lines[result.pattern].start.r = r + dx * minIdx;
            result.lines[result.pattern].start.c = c + dy * minIdx;
            result.lines[result.pattern].end.r = r + dx * maxIdx;
            result.lines[result.pattern].end.c = c + dy * maxIdx;
            result.lines[result.pattern].dir.dx = dx;
            result.lines[result.pattern].dir.dy = dy;
            result.lines[result.pattern].hasGap = gapUsed;
            result.lines[result.pattern].length = length;
            result.pattern++;
        }
    }
    return result;
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

// 禁じ手である、長連の判定関数
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

// ちょうど5連の並びを作っているかの判定関数
BOOL isMakingExactFive(Board *board, int r, int c, char playerMark) {
    for (int dir = 0; dir < dirLength; dir++)
    {
        int length = countContinuousStones(board, r, c, DIRS[dir].dx, DIRS[dir].dy, playerMark);
        if (length == 5)
            return TRUE;
    }
    return FALSE;
}

// 両側が空いているラインかどうかを確認
BOOL isOpenLine(Board *board, Cell start, Cell end, Direction dir) {

    // 両サイドが盤面外の場合
    if (!isInBoard(start.r - dir.dx, start.c - dir.dy) || !isInBoard(end.r + dir.dx, end.c + dir.dy))
        return FALSE;
    return (
        board->cells[start.r - dir.dx][start.c - dir.dy] == EMPTY_CELL && 
        board->cells[end.r + dir.dx][end.c + dir.dy] == EMPTY_CELL
    );
}

BOOL isAtLeastHalfOpenLine(Board *board, Cell start, Cell end, Direction dir) {
    // ラインとして少なくとも片側が空いているかどうかの判定関数

    // 両サイドが盤面外の場合
    if (!isInBoard(start.r - dir.dx, start.c - dir.dy) && !isInBoard(end.r + dir.dx, end.c + dir.dy))
        return FALSE;

    return (
        board->cells[start.r - dir.dx][start.c - dir.dy] == EMPTY_CELL ||
        board->cells[end.r + dir.dx][end.c + dir.dy] == EMPTY_CELL
        );
}


Cell getGapIdx(Board *board, LineIdx *line) {
    Cell result = {.r = -1, .c = -1};
    
    // gapがない場合は早期リターン
    if (!line->hasGap)
        return result;
        
    // 開始位置から終了位置まで探索
    // ただし両端は探索しない
    int x = line->start.r + line->dir.dx;
    int y = line->start.c + line->dir.dy;
    int endX = line->end.r;
    int endY = line->end.c;
    
    while (x != endX || y != endY) {
        if (board->cells[x][y] == EMPTY_CELL) {
            return (Cell){.r = x, .c = y};
        }
        x += line->dir.dx;
        y += line->dir.dy;
    }
    
    return result;
}

BOOL isFour(Board *board, LineIdx *line, char playerMark) {
    /* 
    四の定義は、一個の石を加えることで五にできるもの
    そのため、両サイドが塞がれているものは四ではない。
    また一個石を置くと5を超えてしまうものは、五にできていないため、四ではない
    */

    if (line->length != 4)
        return FALSE;

    if (!isAtLeastHalfOpenLine(board, line->start, line->end, line->dir))
        return FALSE;
    
    // gapがある場合は、そこに石を置いて五になるかを判定
    if (line->hasGap) {
        Cell gap = getGapIdx(board, line);
        return isMakingExactFive(board, gap.r, gap.c, playerMark);
    }

    // gapがない場合は、両側の空いているマスに置いて五になるかを確認
    Cell leftEdge = {
        .r = line->start.r - line->dir.dx, 
        .c = line->start.c - line->dir.dy
    };
    Cell rightEdge = {
        .r = line->end.r + line->dir.dx, 
        .c = line->end.c + line->dir.dy
    };

    // 両端のマスが盤内かつ空いているかをチェック
    BOOL canPlaceLeft = isInBoard(leftEdge.r, leftEdge.c) && 
                       board->cells[leftEdge.r][leftEdge.c] == EMPTY_CELL;
    BOOL canPlaceRight = isInBoard(rightEdge.r, rightEdge.c) && 
                        board->cells[rightEdge.r][rightEdge.c] == EMPTY_CELL;

    // どちらかの端に置いて五になるかをチェック
    return (canPlaceLeft && isMakingExactFive(board, leftEdge.r, leftEdge.c, playerMark)) ||
           (canPlaceRight && isMakingExactFive(board, rightEdge.r, rightEdge.c, playerMark));
}

// r, cに石を置いたときに、四四を作るかどうかの判定関数
BOOL isMakingDoubleFour(Board *board, int r, int c, char playerMark){
    //一時的にボードを作り石を置いてみる
    Board tmpBoard = *board;
    tmpBoard.cells[r][c] = playerMark;
    
    int fourLineCount = 0;

    // 4方向それぞれについて四を数える
    for (int dir = 0; dir < dirLength; dir++) {
        LinePatterns linePatterns = countContinuousStonesWithGap(
            &tmpBoard, r, c, DIRS[dir].dx, DIRS[dir].dy, playerMark
        );
        
        for (int i = 0; i < linePatterns.pattern; i++) {
            if (isFour(&tmpBoard, &linePatterns.lines[i], playerMark)) {
                fourLineCount++;
                if (fourLineCount > 1)
                    return TRUE;
            }
        }
    }
    
    return FALSE;
}

// 達四であるかどうかの判定関数
// 達四とは、五にする場所が2つある四のこと
BOOL isMakingGreatFour(Board *board, int r, int c, char playerMark) {
    BOOL result = FALSE;

    Board tmpBoard = *board;
    tmpBoard.cells[r][c] = playerMark;

    for (int dir = 0; dir < dirLength; dir++)
    {
        LinePatterns linePatterns = countContinuousStonesWithGap(
            &tmpBoard, r, c, DIRS[dir].dx, DIRS[dir].dy, playerMark
        );
        
        for (int i = 0; i < linePatterns.pattern; i++) {
            LineIdx line = linePatterns.lines[i];
            // 黒番の場合長さが4ではない、またギャップがあると達四になり得ないため、early return
            if (playerMark == PLAYER_X && (line.length != 4 || line.hasGap == TRUE))
                continue;

            // LINEの両サイドのセル
            int prevX = line.start.r - line.dir.dx;
            int prevY = line.start.c - line.dir.dy;

            int nextX = line.end.r + line.dir.dx;
            int nextY = line.end.c + line.dir.dy;

            // gapがあって、条件を満たす特殊ケース
            // 白手番かつgapを埋めて五になり、さらに両側のどちらかに石を置いて五になるケース
            if (playerMark == PLAYER_O && line.length >= 5 && line.hasGap == TRUE) {
                Cell gap = getGapIdx(&tmpBoard, &line);

                if (
                    isWinMove(&tmpBoard, gap.r, gap.c, playerMark) &&
                    (
                        (
                            isInBoard(prevX, prevY) &&
                            isWinMove(&tmpBoard, prevX, prevY, playerMark)) ||
                        (
                            isInBoard(nextX, nextY) &&
                            isWinMove(&tmpBoard, nextX, nextY, playerMark))
                    )
                )
                    result = TRUE;
            }
            // 以下は、白、黒共有ケース
            // 両側のマスが空いている
            if (!isOpenLine(&tmpBoard, line.start, line.end, line.dir))
                continue;

            // 白石の場合、5以上であれば五、だが黒石は5を超えると長連となり五とみなされない
            if (
                isWinMove(&tmpBoard, prevX, prevY, playerMark) == TRUE &&
                isWinMove(&tmpBoard, nextX, nextY, playerMark) == TRUE)
                result = TRUE;
        }
    }
    return result;
}


BOOL isThree(Board *board, LineIdx *line, char playerMark) {
    /* 
    三であるかどうかの判定関数
    三とは、達四にする手段のある3のこと
    */
    if (line->length != 3)
        return FALSE;

    if (!isAtLeastHalfOpenLine(board, line->start, line->end, line->dir))
        return FALSE;

    // gapがある場合は、そこに石を置いて達四になるかを判定
    if (line->hasGap) {
        Cell gap = getGapIdx(board, line);
        return (
            !isProhibitedMove(board, gap.r, gap.c, playerMark) && 
            isMakingGreatFour(board, gap.r, gap.c, playerMark)
        );
    }

    // gapがない場合は、両側の空いているマスに置いて五になるかを確認
    Cell leftEdge = {
        .r = line->start.r - line->dir.dx, 
        .c = line->start.c - line->dir.dy
    };
    Cell rightEdge = {
        .r = line->end.r + line->dir.dx, 
        .c = line->end.c + line->dir.dy
    };

    // 両端のマスが盤内かつ空いているかをチェック
    BOOL canPlaceLeft = isInBoard(leftEdge.r, leftEdge.c) && 
                       board->cells[leftEdge.r][leftEdge.c] == EMPTY_CELL;
    BOOL canPlaceRight = isInBoard(rightEdge.r, rightEdge.c) && 
                        board->cells[rightEdge.r][rightEdge.c] == EMPTY_CELL;

    // どちらかの端に置いて達四になるかをチェック
    return (
            (
                canPlaceLeft && 
                !isProhibitedMove(board, leftEdge.r, leftEdge.c, playerMark) &&
                isMakingGreatFour(board, leftEdge.r, leftEdge.c, playerMark)
            ) ||
           (
                canPlaceRight && 
                !isProhibitedMove(board, rightEdge.r, rightEdge.c, playerMark) &&
                isMakingGreatFour(board, rightEdge.r, rightEdge.c, playerMark)
            )
        );
}

BOOL isMakingDoubleThree(Board *board, int r, int c, char playerMark){
    // r, cに石を置いたときに、三三を作るかどうかの判定関数

    //一時的にボードを作り石を置いてみる
    Board tmpBoard = *board;
    tmpBoard.cells[r][c] = playerMark;
    
    int threeLineCount = 0;

    // 4方向それぞれについて四を数える
    for (int dir = 0; dir < dirLength; dir++) {
        LinePatterns linePatterns = countContinuousStonesWithGap(
            &tmpBoard, r, c, DIRS[dir].dx, DIRS[dir].dy, playerMark
        );
        
        for (int i = 0; i < linePatterns.pattern; i++) {
            if (isThree(&tmpBoard, &linePatterns.lines[i], playerMark)) {
                threeLineCount++;
                if (threeLineCount > 1)
                    return TRUE;
            }
        }
    }
    
    return FALSE;
}


// 禁じ手かどうかの判定関数
BOOL isProhibitedMove(Board *board, int r, int c, char playerMark) {
    if (playerMark != PLAYER_X)
        return FALSE;
    
    // 五がある場合、五が優先される
    if (isWinMove(board, r, c, playerMark))
        return FALSE;

    return (
        isMakingOverLine(board, r, c, playerMark) ||
        isMakingDoubleFour(board, r, c, playerMark) ||
        isMakingDoubleThree(board, r, c, playerMark)
        );
}

/* BOOL isDoubleThree(Board *board, int row, int col, int dx, int dy, char playerMark){
    if (playerMark != PLAYER_X)
        return FALSE;

    int threeCounts = 0;

    LinePatterns linePatterns;
    for (int dir = 0; dir < dirLength; dir++) {
        linePatterns = countContinuousStonesWithGap(board, row, col, dx, dy, playerMark);
        if (linePatterns.pattern == 0) 
            continue;
        for (int i = 0; i < linePatterns.pattern; i++) {
            if (isThree(board, &linePatterns.lines[i]))
                threeCounts++;
        }
        if (threeCounts > 1)
            return TRUE;
    }
    return FALSE;
} */


BOOL __isSameLine(Board* board, LineInfo* line1, LineInfo* line2, int row, int col, int dx, int dy) {
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


LineInfoArray __getTargetLengthLinesInDirectionCandidates(Board *board, int row, int col, int length, int dx, int dy, char playerMark) {
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
    LineInfoArray result = {0};
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
            LineInfo line = {
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


LineInfoArray __getTargetLengthLinesInDirection(Board *board, int row, int col, int length, int dx, int dy, char playerMark) {
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
    LineInfoArray candidates = __getTargetLengthLinesInDirectionCandidates(board, row, col, length, dx, dy, playerMark);
        
    LineInfoArray result = {0};

    for (int i = 0; i < candidates.count; i++)
    {
        LineInfo candidate = candidates.lines[i];
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
/* 
BOOL isForbiddenMove(Board *board, int row, int col, char playerMark) {

    // 黒の手番のみチェック
    if (playerMark != PLAYER_X) {
        return FALSE;
    }
    
    // 勝利する手なら問題ない
    if (isWinMove(board, row, col, playerMark)) {
        return FALSE;
    }

    // 長連がある場合、禁じ手
    if (isMakingOverLine(board, row, col, playerMark)) {
        return TRUE;
    }


    // 上記以外の場合、有効な3, 4の数を調べる
    // 有効とは、相手が最善手を打っても、5を作ることを防げないライン。
    // そのため、片側が壁や石で塞がれているなど、防ぐことができるラインや、
    // 禁じ手となるためラインを伸ばせない状態は有効とは見做されず、3x3,4x4の対象とならない。

    board->cells[row][col] = playerMark;
    
    int effectiveThrees = 0;
    int effectiveFours = 0;
    
    // 4方向それぞれについて、3と4のラインを探し、有効なものをカウント
    for (int i = 0; i < 4; i++) {
        // 3のライン
        LineInfoArray threeLines = __getTargetLengthLinesInDirection(
            board, row, col, 3, DIRS[i].dx, DIRS[i].dy, playerMark
        );
    }
    
    board->cells[row][col] = EMPTY_CELL;
    
    // 3x3または4x4の判定
    return (effectiveThrees >= 2 || effectiveFours >= 2);
}
 */