#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "test_utils.h"
#include "test_board.h"



void testInitBoard()
{
    printf("Start testInitBoard...\n");

    Board board;

    board.cells[1][1] = PLAYER_X;
    board.cells[4][6] = PLAYER_O;
    board.cells[8][8] = PLAYER_X;
    
    initBoard(&board);

    for (int i = 1; i <= BOARD_ROWS; i++) {
        for (int j = 1; j <= BOARD_COLUMNS; j++) {
            if (board.cells[i][j] != EMPTY_CELL) {
                printf("Error: %d, %d is not empty.\n", i, j);
                assert(board.cells[i][j] == EMPTY_CELL);
            }
        }
    }
    printf("test testInitBoard success.\n");
}

void testIsWinMove() {
    printf("Start testIsWinMove...\n");

    Board board;

    // 横に5つ並んでいる
    const char *testBoard1[] = {
        NULL,
        ".........",
        "..XXXX@..",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "O@OOO...."
    };
    
    initBoardWithStr(&board, testBoard1);
    assert(isWinMove(&board, 2, 7, PLAYER_X) == TRUE);
    assert(isWinMove(&board, 9, 2, PLAYER_O) == TRUE);

    // 左上隅、FALSE case & TRUE case
    const char *testBoard2[] = {
        NULL,
        "@XXXX....",
        "O........",
        "O........",
        ".........",
        "O........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);
    assert(isWinMove(&board, 1, 1, PLAYER_O) == FALSE);
    assert(isWinMove(&board, 1, 1, PLAYER_X) == TRUE);

    // 長さが5を超えるケース
    const char *testBoard3[] = {
        NULL,
        ".........",
        "X........",
        "X........",
        "X..O.....",
        "@...O....",
        "X....O...",
        "X.....O..",
        ".......O.",
        "........@"
    };
    
    initBoardWithStr(&board, testBoard3);
    assert(isWinMove(&board, 5, 1, PLAYER_X) == FALSE);
    assert(isWinMove(&board, 9, 9, PLAYER_O) == TRUE);

    //
    const char *testBoard4[] = {
        NULL,
        ".........",
        "......X..",
        ".....@...",
        "....X....",
        "...X.....",
        "..X......",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard4);
    assert(isWinMove(&board, 3, 6, PLAYER_X) == TRUE);


    printf("Success testIsWinMove.\n");
}


void testIsMakingOverLine() {
    printf("Start testIsMakingOverLine...\n");

    Board board;

    // 横に5つ並んでいる
    const char *testBoard1[] = {
        NULL,
        ".........",
        ".XXXXX@..",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "O@OOOO..."
    };
    
    initBoardWithStr(&board, testBoard1);
    assert(isMakingOverLine(&board, 2, 7, PLAYER_X) == TRUE);
    assert(isMakingOverLine(&board, 9, 2, PLAYER_O) == FALSE);

    const char *testBoard2[] = {
        NULL,
        ".........",
        "X........",
        "X........",
        "X..X.....",
        "@...X....",
        "X....X...",
        "X.....X..",
        ".......X.",
        "........@"
    };
    
    initBoardWithStr(&board, testBoard2);
    assert(isMakingOverLine(&board, 5, 1, PLAYER_X) == TRUE);
    assert(isMakingOverLine(&board, 9, 9, PLAYER_X) == TRUE);

    printf("Success testIsMakingOverLine.\n");
}


void testIsSameLine() {
    Board board;
    printf("Start testIsSameLine...\n");

    // テストケース1: 同じライン（連続した石）
    const char *testBoard1[] = {
        NULL,
        ".........",
        "...X@X...",  // 横方向の連続した石
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard1);

    LineInfo line1 = {.startIdx = -2, .endIdx = 1, .hasGap = FALSE};
    LineInfo line2 = {.startIdx = -1, .endIdx = 2, .hasGap = FALSE};
    
    // 横方向（dx=0, dy=1）で比較
    assert(__isSameLine(&board, &line1, &line2, 2, 5, 0, 1) == TRUE);

    // テストケース2: 斜め方向のライン
    const char *testBoard2[] = {
        NULL,
        ".........",
        "...X.....",
        "....@....",
        ".....X...",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);
    
    LineInfo line3 = {.startIdx = -1, .endIdx = 2, .hasGap = FALSE};
    LineInfo line4 = {.startIdx = -2, .endIdx = 1, .hasGap = FALSE};
    
    // 斜め方向（dx=1, dy=1）で比較
    assert(__isSameLine(&board, &line3, &line4, 3, 5, 1, 1) == TRUE);

    // テストケース3: 空きマスを含むライン
    const char *testBoard3[] = {
        NULL,
        ".........",
        "...X.X@..",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard3);
    
    LineInfo line5 = {.startIdx = -4, .endIdx = 0, .hasGap = TRUE};
    LineInfo line6 = {.startIdx = -3, .endIdx = 1, .hasGap = TRUE};
    
    assert(__isSameLine(&board, &line5, &line6, 2, 7, 0, 1) == TRUE);


    // テストケース4: 異なるライン
    const char *testBoard4[] = {
        NULL,
        ".........",
        ".X.X@.XX.",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard4);
    
    LineInfo line7 = {.startIdx = -3, .endIdx = 0, .hasGap = TRUE};
    LineInfo line8 = {.startIdx = 0, .endIdx = 3, .hasGap = TRUE};
    
    assert(__isSameLine(&board, &line7, &line8, 2, 5, 0, 1) == FALSE);

    printf("All testIsSameLine passed!\n");
}

void testGetTargetLengthLinesInDirection() {
    Board board;
    LineInfoArray candidateResult, result;

    printf("Start testGetTargetLengthLinesInDirection...\n");

    // テストケース1: 連続した石のパターン
    const char *testBoard1[] = {
        NULL,
        ".........",
        "...XX@...",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard1);

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 2, 6, 3, 0, 1, PLAYER_X); 
    assert(candidateResult.count == 2);
    assert(candidateResult.lines[0].startIdx == -3);
    assert(candidateResult.lines[0].endIdx == 0);
    assert(candidateResult.lines[0].hasGap == FALSE);
    assert(candidateResult.lines[1].startIdx == -2);
    assert(candidateResult.lines[1].endIdx == 1);
    assert(candidateResult.lines[1].hasGap == FALSE);

    result = __getTargetLengthLinesInDirection(&board, 2, 6, 3, 0, 1, PLAYER_X);  // 横方向
    assert(result.count == 1);
    assert(result.lines[0].startIdx == -3);
    assert(result.lines[0].endIdx == 0);
    assert(result.lines[0].hasGap == FALSE);

    // テストケース2: 飛び石パターン
    const char *testBoard2[] = {
        NULL,
        ".........",
        ".........",
        "..X.X@...", 
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 3, 6, 3, 0, 1, PLAYER_X); 
    assert(candidateResult.count == 1);
    assert(candidateResult.lines[0].startIdx == -3);
    assert(candidateResult.lines[0].endIdx == 0);
    assert(candidateResult.lines[0].hasGap == TRUE);

    result = __getTargetLengthLinesInDirection(&board, 3, 6, 3, 0, 1, PLAYER_X);  // 横方向
    assert(result.count == 1);
    assert(result.lines[0].startIdx == -3);
    assert(result.lines[0].endIdx == 0);
    assert(result.lines[0].hasGap == TRUE);

    // テストケース3: 複数の候補パターン
    const char *testBoard3[] = {
        NULL,
        ".........",
        ".........",
        ".X.X@.X..",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard3);

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 3, 5, 3, 0, 1, PLAYER_X);
    assert(candidateResult.count == 2);
    assert(candidateResult.lines[0].startIdx == -3);
    assert(candidateResult.lines[0].endIdx == 0);
    assert(candidateResult.lines[0].hasGap == TRUE);
    assert(candidateResult.lines[1].startIdx == -1);
    assert(candidateResult.lines[1].endIdx == 2);
    assert(candidateResult.lines[1].hasGap == TRUE);

    result = __getTargetLengthLinesInDirection(&board, 3, 5, 3, 0, 1, PLAYER_X);
    assert(result.count == 2);  // 2つの異なる3の候補

    // テストケース4: 相手の石がある or 壁があるケース
    const char *testBoard4[] = {
        NULL,
        ".........",
        ".........",
        ".OX.X@...",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".....@X.X"
    };
    
    initBoardWithStr(&board, testBoard4);

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 3, 6, 3, 0, 1, PLAYER_X);
    // candidateはLINEの両端を気にしない
    assert(candidateResult.count == 1);
    assert(candidateResult.lines[0].startIdx == -3);
    assert(candidateResult.lines[0].endIdx == 0);
    assert(candidateResult.lines[0].hasGap == TRUE);

    LineInfoArray result1 = __getTargetLengthLinesInDirection(&board, 3, 5, 3, 0, 1, PLAYER_X);
    assert(result1.count == 0);  // 候補なし

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 9, 6, 3, 0, 1, PLAYER_X);
    // candidateはLINEの両端を気にしない
    assert(candidateResult.count == 1);
    assert(candidateResult.lines[0].startIdx == 0);
    assert(candidateResult.lines[0].endIdx == 3);
    assert(candidateResult.lines[0].hasGap == TRUE);

    LineInfoArray result2 = __getTargetLengthLinesInDirection(&board, 9, 6, 3, 0, 1, PLAYER_X);
    assert(result2.count == 0);  // 候補なし

    // テストケース5: lengthを超えるケース
    const char *testBoard5[] = {
        NULL,
        ".........", 
        ".........",
        ".........",
        "..XX.@X..",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard5);

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 4, 6, 3, 0, 1, PLAYER_X);
    // candidateはlength + 1 の長さにlengthの石があれば、結果を返す
    assert(candidateResult.count == 2);
    assert(candidateResult.lines[0].startIdx == -3);
    assert(candidateResult.lines[0].endIdx == 0);
    assert(candidateResult.lines[0].hasGap == TRUE);
    assert(candidateResult.lines[1].startIdx == -2);
    assert(candidateResult.lines[1].endIdx == 1);
    assert(candidateResult.lines[1].hasGap == TRUE);

    result = __getTargetLengthLinesInDirection(&board, 4, 6, 3, 0, 1, PLAYER_X);
    assert(result.count == 0);

    printf("All testGetTargetLengthLinesInDirection passed!\n");
}


void testWouldCreateOverline() {
    Board board;

    printf("Start testWouldCreateOverline...\n");

    // @に石を置くと、4x4に見えるが、横の列は完成させると長連を作るため、
    // 横の列は完成させることができず4と見做されない。
    const char *testBoard1[] = {
            NULL,
            ".........",
            "X.@XXXO..", 
            "..X......",
            "..X......",
            "..X......",
            ".........",
            ".........",
            ".........",
            "........."
        };
    initBoardWithStr(&board, testBoard1);
    assert(__wouldCreateOverline(&board, 2, 3, 0, 1, PLAYER_X) == TRUE);

    // @に石を置くと、4x4になる
    // {2, 1}に石を置くと、5を作れてしまうため４として成立している。
    const char *testBoard2[] = {
            NULL,
            ".........",
            ".X@XX.X..", 
            "..X......",
            "..X......",
            "..X......",
            ".........",
            ".........",
            ".........",
            "........."
        };
    initBoardWithStr(&board, testBoard2);
    assert(__wouldCreateOverline(&board, 2, 3, 0, 1, PLAYER_X) == FALSE);

    // 横の列を完成させようとすると長連になってしまうため、@に石を置いても、4x4ににならない
    const char *testBoard3[] = {
            NULL,
            ".........",
            "OX@XX.X..", 
            "..X......",
            "..X......",
            "..X......",
            ".........",
            ".........",
            ".........",
            "........."
        };
    initBoardWithStr(&board, testBoard3);
    assert(__wouldCreateOverline(&board, 2, 3, 0, 1, PLAYER_X) == TRUE);
    printf("All testWouldCreateOverline passed!\n");

}


void testCountContinuousStonesWithGap() {
    Board board;
    printf("Start testCountContinuousStonesWithGap...\n");
    
    // テストケース1: 左右方向に2種類のラインができるケース
    const char *testBoard1[] = {
        NULL,
        ".........",
        "X.X@.X...",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard1);
    LinePatterns result1 = countContinuousStonesWithGap(&board, 2, 4, 0, 1, PLAYER_X);
    assert(result1.pattern == 2);
    assert(result1.lines[0].start.r == 2);
    assert(result1.lines[0].start.c == 1);
    assert(result1.lines[0].end.r == 2);
    assert(result1.lines[0].end.c == 4);
    assert(result1.lines[0].dir.dx == 0);
    assert(result1.lines[0].dir.dy == 1);
    assert(result1.lines[1].start.r == 2);
    assert(result1.lines[1].start.c == 3);
    assert(result1.lines[1].end.r == 2);
    assert(result1.lines[1].end.c == 6);
    

    // テストケース2: 1種類のみのケース (gapなし)
    const char *testBoard2[] = {
        NULL,
        ".........",
        "..X@X....",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);
    LinePatterns result2 = countContinuousStonesWithGap(&board, 2, 4, 0, 1, PLAYER_X);
    assert(result2.pattern == 1);
    assert(result2.lines[0].start.r == 2);
    assert(result2.lines[0].start.c == 3);
    assert(result2.lines[0].end.r == 2);
    assert(result2.lines[0].end.c == 5);
    assert(result2.lines[0].dir.dx == 0);
    assert(result2.lines[0].dir.dy == 1);
    

        // テストケース3: 片側にしかgapがないケース
    const char *testBoard3[] = {
        NULL,
        ".........",
        "XXX@.X..",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard3);
    LinePatterns result3 = countContinuousStonesWithGap(&board, 2, 4, 0, 1, PLAYER_X);
    assert(result3.pattern == 1);
    assert(result3.lines[0].start.r == 2);
    assert(result3.lines[0].start.c == 1);
    assert(result3.lines[0].end.r == 2);
    assert(result3.lines[0].end.c == 6);
    assert(result3.lines[0].dir.dx == 0);
    assert(result3.lines[0].dir.dy == 1);
    

    printf("All testCountContinuousStonesWithGap passed!\n");
}

void testIsOpenLine() {
    Board board;

    printf("Start testIsOpenLine...\n");

    const char *testBoard1[] = {
            NULL,
            ".........",
            "..XXX....", 
            ".......X.",
            "..X...X..",
            "..X......",
            ".....X...",
            "......X..",
            ".......X.",
            "........."
        };
    initBoardWithStr(&board, testBoard1);
    Cell start1 = {.r = 2, .c = 3};
    Cell end1 = {.r = 2, .c = 5};
    Direction dir1 = {.dx = 0, .dy = 1};

    Cell start2 = {.r = 4, .c = 3};
    Cell end2 = {.r = 5, .c = 3};
    Direction dir2 = {.dx = 1, .dy = 0};

    Cell start3 = {.r = 3, .c = 8};
    Cell end3 = {.r = 4, .c = 7};
    Direction dir3 = {.dx = 1, .dy = -1};

    Cell start4 = {.r = 6, .c = 6};
    Cell end4 = {.r = 8, .c = 8};
    Direction dir4 = {.dx = 1, .dy = 1};

    assert(isOpenLine(&board, start1, end1, dir1) == TRUE);
    assert(isOpenLine(&board, start2, end2, dir2) == TRUE);
    assert(isOpenLine(&board, start3, end3, dir3) == TRUE);
    assert(isOpenLine(&board, start4, end4, dir4) == TRUE);


    const char *testBoard2[] = {
            NULL,
            ".........",
            "XXXX....X", 
            ".......X.",
            "..X...X..",
            "..X......",
            "..O..X...",
            "......X..",
            ".......X.",
            "........O"
        };
    initBoardWithStr(&board, testBoard2);
    Cell start5 = {.r = 2, .c = 1};
    Cell end5 = {.r = 2, .c = 4};
    Direction dir5 = {.dx = 0, .dy = 1};

    Cell start6 = {.r = 4, .c = 3};
    Cell end6 = {.r = 5, .c = 3};
    Direction dir6 = {.dx = 1, .dy = 0};

    Cell start7 = {.r = 2, .c = 9};
    Cell end7 = {.r = 4, .c = 7};
    Direction dir7 = {.dx = 1, .dy = -1};

    Cell start8 = {.r = 6, .c = 6};
    Cell end8 = {.r = 8, .c = 8};
    Direction dir8 = {.dx = 1, .dy = 1};

    assert(isOpenLine(&board, start5, end5, dir5) == FALSE);
    assert(isOpenLine(&board, start6, end6, dir6) == FALSE);
    assert(isOpenLine(&board, start7, end7, dir7) == FALSE);
    assert(isOpenLine(&board, start8, end8, dir8) == FALSE);

    printf("All testIsOpenLine passed!\n");

}



void testIsHalfOpenLine() {
    Board board;

    printf("Start testIsHalfOpenLine...\n");

    const char *testBoard1[] = {
            NULL,
            ".........",
            ".OXXX...X", 
            "..O....X.",
            "..X...X..",
            "..X......",
            "..O..X...",
            "......X..",
            ".......X.",
            "........."
        };
    initBoardWithStr(&board, testBoard1);
    Cell start1 = {.r = 2, .c = 3};
    Cell end1 = {.r = 2, .c = 5};
    Direction dir1 = {.dx = 0, .dy = 1};

    Cell start2 = {.r = 4, .c = 3};
    Cell end2 = {.r = 5, .c = 3};
    Direction dir2 = {.dx = 1, .dy = 0};

    Cell start3 = {.r = 2, .c = 9};
    Cell end3 = {.r = 4, .c = 7};
    Direction dir3 = {.dx = 1, .dy = -1};

    Cell start4 = {.r = 6, .c = 6};
    Cell end4 = {.r = 8, .c = 8};
    Direction dir4 = {.dx = 1, .dy = 1};

    assert(isAtLeastHalfOpenLine(&board, start1, end1, dir1) == TRUE);
    assert(isAtLeastHalfOpenLine(&board, start2, end2, dir2) == FALSE);
    assert(isAtLeastHalfOpenLine(&board, start3, end3, dir3) == TRUE);
    assert(isAtLeastHalfOpenLine(&board, start4, end4, dir4) == TRUE);

    printf("All testIsHalfOpenLine passed!\n");

}

void testGetGapIdx() {
    Board board;
    printf("Start testGetGapIdx...\n");
    
    // テストケース1: 水平方向のgap
    const char *testBoard1[] = {
        NULL,
        ".........",
        ".@XX.X...", 
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard1);
    LineIdx line1 = {
        .start = {.r = 2, .c = 2},
        .end = {.r = 2, .c = 6},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = TRUE
    };
    
    Cell gap1 = getGapIdx(&board, &line1);
    assert(gap1.r == 2);
    assert(gap1.c == 5);
    
    // テストケース2: gapなしのライン
    const char *testBoard2[] = {
        NULL,
        ".........",
        "..XXX....", 
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);
    LineIdx line2 = {
        .start = {.r = 2, .c = 3},
        .end = {.r = 2, .c = 6},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = FALSE
    };
    
    Cell gap2 = getGapIdx(&board, &line2);
    assert(gap2.r == -1);
    assert(gap2.c == -1);
    
    // テストケース3: 斜め方向のgap
    const char *testBoard3[] = {
        NULL,
        ".........",
        "..X......",
        "...X.....",
        ".........", 
        ".....X...",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard3);
    LineIdx line3 = {
        .start = {.r = 2, .c = 3},
        .end = {.r = 5, .c = 6},
        .dir = {.dx = 1, .dy = 1},
        .hasGap = TRUE
    };
    
    Cell gap3 = getGapIdx(&board, &line3);
    assert(gap3.r == 4);
    assert(gap3.c == 5);
    
    printf("All testGetGapIdx passed!\n");
}

void testIsFour() {
    Board board;
    printf("Start testIsFour...\n");
    
    // テストケース1: gapあり、なしの四
    const char *testBoard1[] = {
        NULL,
        ".........",
        ".XXX.X...",
        ".........",
        "..X......",
        "...X.....",
        "....X....",
        ".....X...",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard1);
    LineIdx line1 = {
        .start = {.r = 2, .c = 2},
        .end = {.r = 2, .c = 6},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = TRUE,
        .length = 4
    };

    LineIdx line2 = {
        .start = {.r = 4, .c = 3},
        .end = {.r = 7, .c = 6},
        .dir = {.dx = 1, .dy = 1},
        .hasGap = FALSE,
        .length = 4
};
    assert(isFour(&board, &line1, PLAYER_X) == TRUE);
    assert(isFour(&board, &line2, PLAYER_X) == TRUE);

    // テストケース2: 四ではないケース
    const char *testBoard2[] = {
        NULL,
        ".........",
        ".XXOX....",
        ".........",
        "....XXX..",
        ".........",
        ".........",
        ".........",
        "XXXX.X...",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);
    LineIdx line3 = {
        .start = {.r = 2, .c = 2},
        .end = {.r = 2, .c = 6},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = TRUE,
        .length = 4
    };

    LineIdx line4 = {
        .start = {.r = 4, .c = 5},
        .end = {.r = 4, .c = 7},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = FALSE,
        .length = 3
    };

    LineIdx line5 = {
        .start = {.r = 8, .c = 1},
        .end = {.r = 8, .c = 6},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = FALSE,
        .length = 4
    };
    assert(isFour(&board, &line3, PLAYER_X) == FALSE);
    assert(isFour(&board, &line4, PLAYER_X) == FALSE);
    assert(isFour(&board, &line5, PLAYER_X) == FALSE);
    
    printf("All testIsFour passed!\n");
}

void testIsMakingDoubleFour() {
    Board board;
    printf("Start testIsMakingDoubleFour...\n");
    
    // テストケース1: 四四
    const char *testBoard1[] = {
        NULL,
        "..X......",
        "..@XXX...", 
        "..X......", 
        "..X......",
        ".........",
        "...X.....",
        "....X...",
        ".....X...",
        "...XXX@.."
    };
    
    initBoardWithStr(&board, testBoard1);
    assert(isMakingDoubleFour(&board, 2, 3, PLAYER_X) == TRUE);
    assert(isMakingDoubleFour(&board, 9, 7, PLAYER_X) == TRUE);
    
    // テストケース2: 四が1つだけ
    const char *testBoard2[] = {
        NULL,
        "..X......",
        ".O@XXXO..",
        "..X......",
        "..X......",
        ".........",
        ".....X...",
        "..XXX@.X.",
        ".....X...",
        ".....X..."
    };
    
    initBoardWithStr(&board, testBoard2);
    assert(isMakingDoubleFour(&board, 2, 3, PLAYER_X) == FALSE);
    assert(isMakingDoubleFour(&board, 7, 6, PLAYER_X) == FALSE);


    // テストケース3: 五を作っていても、この関数は気にせず四四を判定する
    const char *testBoard3[] = {
        NULL,
        ".XX......",
        ".X@X.X...",
        "..XX.....",
        "..X.X....",
        ".....X...",
        ".........",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard3);
    assert(isMakingDoubleFour(&board, 2, 3, PLAYER_X) == TRUE);

    // テストケース4: 一直線上の四四のケース
    const char *testBoard4[] = {
        NULL,
        "XXX.@.XXX",
        ".........",
        ".X.X@X.X.",
        ".........",
        ".XX.X@.XX",
        ".........",
        ".........",
        ".X.XX@.X.",
        "........."
    };
    
    initBoardWithStr(&board, testBoard4);
    assert(isMakingDoubleFour(&board, 1, 5, PLAYER_X) == TRUE);
    assert(isMakingDoubleFour(&board, 3, 5, PLAYER_X) == TRUE);
    assert(isMakingDoubleFour(&board, 5, 6, PLAYER_X) == TRUE);
    assert(isMakingDoubleFour(&board, 8, 6, PLAYER_X) == TRUE);

    // テストケース4: 白の手番
    assert(isMakingDoubleFour(&board, 1, 1, PLAYER_O) == FALSE);
    printf("All testIsMakingDoubleFour passed!\n");
}

void testIsMakingGreatFour() {
    Board board;
    printf("Start testIsMakingGreatFour...\n");
    
    // テストケース:
    // @は達四になるケース
    // #はならないケース
    // Ref (https://tokai-renjukai.pya.jp/siryo/RenjuKiso/RenjuKiso-1-2.pdf)
    const char *testBoard1[] = {
        NULL,
        ".#@XXX@#.",
        "....O....",
        ".O#XXX@#.",
        "...O.O...",
        ".........",
        ".........",
        ".........",
        "..OO@O.O.",
        "OOO@....."
    };
    
    initBoardWithStr(&board, testBoard1);
    assert(isMakingGreatFour(&board, 1, 2, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 1, 3, PLAYER_X) == TRUE);
    assert(isMakingGreatFour(&board, 1, 7, PLAYER_X) == TRUE);
    assert(isMakingGreatFour(&board, 1, 8, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 3, 3, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 3, 7, PLAYER_X) == TRUE);
    assert(isMakingGreatFour(&board, 3, 8, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 8, 5, PLAYER_O) == TRUE);
    assert(isMakingGreatFour(&board, 9, 4, PLAYER_O) == FALSE);

    // テストケース2:
    const char *testBoard2[] = {
        NULL,
        ".........",
        "..OXXX##.",
        "....#....",
        ".#XX@X#..",
        "..#......",
        "#XX#.X.X.",
        ".........",
        ".........",
        "........."
    };
    
    initBoardWithStr(&board, testBoard2);
    assert(isMakingGreatFour(&board, 2, 7, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 2, 8, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 3, 5, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 4, 2, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 4, 5, PLAYER_X) == TRUE);
    assert(isMakingGreatFour(&board, 4, 7, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 5, 3, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 6, 1, PLAYER_X) == FALSE);
    assert(isMakingGreatFour(&board, 6, 4, PLAYER_X) == FALSE);
    printf("All testIsMakingGreatFour passed!\n");
}

void testIsThree() {
    Board board;
    printf("Start testIsThree...\n");
    
    // テストケース:
    // Ref (https://tokai-renjukai.pya.jp/siryo/RenjuKiso/RenjuKiso-1-2.pdf)
    const char *testBoard1[] = {
        NULL,
        "..XXX....",
        ".....O.X.",
        ".........",
        "..XX.X...",
        "....X....",
        ".....O...",
        "........",
        ".........",
        ".O.OO....",
    };
    
    initBoardWithStr(&board, testBoard1);
    LineIdx line1 = {
        .start = {.r = 1, .c = 3},
        .end = {.r = 1, .c = 5},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = FALSE,
        .length = 3
    };
    LineIdx line2 = {
        .start = {.r = 2, .c = 8},
        .end = {.r = 5, .c = 5},
        .dir = {.dx = 1, .dy = -1},
        .hasGap =TRUE, 
        .length = 3
    };
    LineIdx line3 = {
        .start = {.r = 4, .c = 3},
        .end = {.r = 4, .c = 6},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = TRUE,
        .length = 3
    };
    LineIdx line4 = {
        .start = {.r = 9, .c = 2},
        .end = {.r = 9, .c = 5},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = TRUE,
        .length = 3
    };

    assert(isThree(&board, &line1, PLAYER_X) == TRUE);
    assert(isThree(&board, &line2, PLAYER_X) == TRUE);
    assert(isThree(&board, &line3, PLAYER_X) == TRUE);
    assert(isThree(&board, &line4, PLAYER_O) == TRUE);

    const char *testBoard2[] = {
        NULL,
        ".XXX.O...",
        ".........",
        ".X..XXX.O",
        ".........",
        ".XX.X.X.O",
        ".........",
        ".........",
        ".........",
        ".........",
    };
    // 1行目は夏止め、
    // 3行目は長連筋の夏止め
    // 5行目は長連筋でそれぞれ三とはならない
    
    initBoardWithStr(&board, testBoard2);
    LineIdx line5 = {
        .start = {.r = 1, .c = 2},
        .end = {.r = 1, .c = 4},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = FALSE,
        .length = 3
    };
    LineIdx line6 = {
        .start = {.r = 3, .c = 5},
        .end = {.r = 3, .c = 7},
        .dir = {.dx = 0, .dy = 1},
        .hasGap =FALSE, 
        .length = 3
    };
    LineIdx line7 = {
        .start = {.r = 5, .c = 2},
        .end = {.r = 5, .c = 5},
        .dir = {.dx = 0, .dy = 1},
        .hasGap = TRUE,
        .length = 3
    };

    assert(isThree(&board, &line5, PLAYER_X) == FALSE);
    assert(isThree(&board, &line6, PLAYER_X) == FALSE);
    assert(isThree(&board, &line7, PLAYER_X) == FALSE);

    const char *testBoard3[] = {
        NULL,
        ".........",
        ".XX.@....",
        "..OOB.X..",
        "...XXOX..",
        "...OXE.X.",
        "....#....",
        "...X.....",
        "....X....",
        ".........",
    };
    // Bにおいても、@が三三の禁手のため、達四にできない
    // Eにおいても、#が四四の禁手のため、達四にできない

    initBoardWithStr(&board, testBoard3);
    board.cells[3][5] = PLAYER_X;

    LineIdx line8 = {
        .start = {.r = 3, .c = 5},
        .end = {.r = 5, .c = 5},
        .dir = {.dx = 1, .dy = 0},
        .hasGap = FALSE,
        .length = 3};

    assert(isThree(&board, &line8, PLAYER_X) == FALSE);

    board.cells[3][5] = EMPTY_CELL;
    board.cells[5][6] = PLAYER_X;

    LineIdx line9 = {
        .start = {.r = 4, .c = 7},
        .end = {.r = 7, .c = 4},
        .dir = {.dx = 1, .dy = -1},
        .hasGap = TRUE,
        .length = 3};

    assert(isThree(&board, &line9, PLAYER_X) == FALSE);
    printf("All testIsThree passed!\n");
}

void testIsMakingDoubleThree() {
    Board board;
    printf("Start testIsMakingDoubleThree...\n");
    
    // テストケース:
    // Ref (https://tokai-renjukai.pya.jp/siryo/RenjuKiso/RenjuKiso-1-2.pdf)
    const char *testBoard1[] = {
        NULL,
        ".........",
        "...X.....",
        "..X@X.O..",
        ".O.X.OO..",
        "....@X.X.",
        "...#.....",
        "....O....",
        ".........",
        ".........",
    };

    // @は黒の三三、#は白の三三
    
    initBoardWithStr(&board, testBoard1);
    assert(isMakingDoubleThree(&board, 3, 4, PLAYER_X) == TRUE);
    assert(isMakingDoubleThree(&board, 5, 5, PLAYER_X) == TRUE);
    assert(isMakingDoubleThree(&board, 6, 4, PLAYER_O) == TRUE);

    const char *testBoard2[] = {
        NULL,
        ".........",
        ".X.......",
        "X...O....",
        ".X.OO#...",
        "O..X.X@..",
        ".......O.",
        "..O...X..",
        "......X..",
        ".........",
    };

    // @は黒の三三、#は白の三三
    
    initBoardWithStr(&board, testBoard2);
    assert(isMakingDoubleThree(&board, 4, 6, PLAYER_O) == TRUE);
    assert(isMakingDoubleThree(&board, 5, 7, PLAYER_X) == TRUE);

    printf("Start testIsMakingDoubleThree...\n");
}

void runBoardTests() {
    printf("Start runBoardTests...\n");
    testInitBoard();
    testIsWinMove();
    testIsMakingOverLine();
    testIsSameLine();
    testGetTargetLengthLinesInDirection();
    testWouldCreateOverline();
    testCountContinuousStonesWithGap();
    testIsOpenLine();
    testIsHalfOpenLine();
    testGetGapIdx();
    testIsFour();
    testIsMakingDoubleFour();
    testIsMakingGreatFour();
    testIsThree();
    testIsMakingDoubleThree();
    printf("Finished runBoardTests.\n");
}
