#include <stdio.h>
#include "board.h"

void printBoard(Board *board) {
    int i, j, k;
    printf("    ");
    for (k = 1; k <= BOARD_COLUMNS; k++) {
        printf("%d   ", k);
    }
    printf("\n\n");

    for (i = 1; i <= BOARD_ROWS; i++)
    {
        printf("%d   ", i);
        for (j = 1; j < BOARD_COLUMNS; j++)
        {
            if (i == board->lastRow && j == board->lastCol) {
                printf("\x1b[32m%c\x1b[39m | ", board->cells[i][j]);
            }
            else
                printf("%c | ", board->cells[i][j]);
        }
        if (i == board->lastRow && BOARD_COLUMNS == board->lastCol) {
            printf("\x1b[32m%c\x1b[0m\n", board->cells[i][BOARD_COLUMNS]);
        } else {
            printf("%c\n", board->cells[i][BOARD_COLUMNS]);
        }

        if (i < BOARD_ROWS)
        {
            printf("    - + - + - + - + - + - + - + - + - \n");
        }
    }
    printf("\n\n");
}

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

BOOL __isInBoardRange(int r, int c) {
    if (1 <= r && r <= BOARD_ROWS && 1 <= c && c <= BOARD_COLUMNS)
        return TRUE;
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

            if (!__isInBoardRange(x, y)) {
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
