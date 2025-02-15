#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "../include/game.h"
#include "test_utils.h"
#include "test_game.h"



void testHasWinnerInVerticalExpected()
 {
    Board board_1 = __prepareBoard();

    board_1.cells[1][1] = PLAYER_X;
    board_1.cells[2][1] = PLAYER_X;
    board_1.cells[3][1] = PLAYER_X;
    board_1.cells[4][1] = PLAYER_X;
    board_1.cells[5][1] = PLAYER_X;

    assert(hasWinner(&board_1, PLAYER_X) == TRUE);
    assert(hasWinner(&board_1, PLAYER_O) == FALSE);
    assert(getWinner(&board_1) == PLAYER_X);

    Board board_2 = __prepareBoard();

    board_2.cells[4][7] = PLAYER_O;
    board_2.cells[5][7] = PLAYER_O;
    board_2.cells[6][7] = PLAYER_O;
    board_2.cells[7][7] = PLAYER_O;
    board_2.cells[8][7] = PLAYER_O;

    assert(hasWinner(&board_2, PLAYER_O) == TRUE);
    assert(hasWinner(&board_2, PLAYER_X) == FALSE);
    assert(getWinner(&board_2) == PLAYER_O);

 }

void testHasWinnerInHorizontalExpected() {
    Board board_1 = __prepareBoard();

    board_1.cells[1][1] = PLAYER_X;
    board_1.cells[1][2] = PLAYER_X;
    board_1.cells[1][3] = PLAYER_X;
    board_1.cells[1][4] = PLAYER_X;
    board_1.cells[1][5] = PLAYER_X;

    assert(hasWinner(&board_1, PLAYER_X) == TRUE);
    assert(hasWinner(&board_1, PLAYER_O) == FALSE);
    assert(getWinner(&board_1) == PLAYER_X);

    Board board_2 = __prepareBoard();

    board_2.cells[8][5] = PLAYER_O;
    board_2.cells[8][6] = PLAYER_O;
    board_2.cells[8][7] = PLAYER_O;
    board_2.cells[8][8] = PLAYER_O;
    board_2.cells[8][9] = PLAYER_O;

    assert(hasWinner(&board_2, PLAYER_O) == TRUE);
    assert(hasWinner(&board_2, PLAYER_X) == FALSE);
    assert(getWinner(&board_2) == PLAYER_O);

}

void testHasWinnerInCrossExpected() {
    Board board = __prepareBoard();
    board.cells[1][1] = PLAYER_X;
    board.cells[2][2] = PLAYER_X;
    board.cells[3][3] = PLAYER_X;
    board.cells[4][4] = PLAYER_X;
    board.cells[5][5] = PLAYER_X;

    assert(hasWinner(&board, PLAYER_X) == TRUE);
    assert(hasWinner(&board, PLAYER_O) == FALSE);
    assert(getWinner(&board) == PLAYER_X);

}

void testNotHasWinnerExpected() {
    Board board = __prepareBoard();

    board.cells[1][1] = PLAYER_X;
    board.cells[2][2] = PLAYER_O;
    board.cells[3][3] = PLAYER_X;
    board.cells[4][4] = PLAYER_X;
    board.cells[5][4] = PLAYER_X;
    board.cells[5][5] = PLAYER_X;
    board.cells[5][6] = PLAYER_X;
    board.cells[6][6] = PLAYER_X;
    board.cells[7][7] = EMPTY_CELL;
    board.cells[8][8] = PLAYER_X;
    

    assert(hasWinner(&board, PLAYER_X) == FALSE);
    assert(hasWinner(&board, PLAYER_O) == FALSE);
    assert(getWinner(&board) == EMPTY_CELL);

}

void testGetTargetLengthLinesInDirection() {
    Board board;
    lineInfoArray candidateResult, result;

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

    lineInfoArray result1 = __getTargetLengthLinesInDirection(&board, 3, 5, 3, 0, 1, PLAYER_X);
    assert(result1.count == 0);  // 候補なし

    candidateResult = __getTargetLengthLinesInDirectionCandidates(&board, 9, 6, 3, 0, 1, PLAYER_X);
    // candidateはLINEの両端を気にしない
    assert(candidateResult.count == 1);
    assert(candidateResult.lines[0].startIdx == 0);
    assert(candidateResult.lines[0].endIdx == 3);
    assert(candidateResult.lines[0].hasGap == TRUE);

    lineInfoArray result2 = __getTargetLengthLinesInDirection(&board, 9, 6, 3, 0, 1, PLAYER_X);
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

/* void testIsThree(){
    assert(__getTargetLengthLinesInDirection("..XX@..", 5) == TRUE);  // @の場所に石を置いたら三になる
    assert(__getTargetLengthLinesInDirection("..@XX..", 3) == TRUE);  // @の場所に石を置いたら三になる
    assert(__getTargetLengthLinesInDirection("..X@X..", 4) == TRUE);  // @を含めて三つ並んでいる
    assert(__getTargetLengthLinesInDirection("..XX.@.", 6) == TRUE);  // @の場所に石を置いたら飛び三になる
    assert(__getTargetLengthLinesInDirection(".@.XX..", 2) == TRUE);  // @の場所に石を置いたら飛び三になる
    assert(__getTargetLengthLinesInDirection("..X@.X.", 4) == TRUE);  // @の場所に石を置いたら飛び三になる
    assert(__getTargetLengthLinesInDirection(".X@.X.O", 3) == TRUE);  // @の場所に石を置いたら飛び三になる

    assert(__getTargetLengthLinesInDirection(".X.@.X.", 4) == FALSE);  // @の場所に石を置いても三にならない
    assert(__getTargetLengthLinesInDirection(".XX@.X.", 4) == FALSE);  // @の場所に石を置いたら四になるため、三にならない
    assert(__getTargetLengthLinesInDirection("XX@....", 3) == FALSE);  // @の場所に石を置いたら四になるため、三にならない
    assert(__getTargetLengthLinesInDirection(".OX@.X.", 4) == FALSE);  // 2の場所に、他の石があるため、三にならない
    assert(__getTargetLengthLinesInDirection("..X@.XO", 4) == FALSE);  // 7の場所に、他の石があるため、三にならない
    assert(__getTargetLengthLinesInDirection("O.X@X.O", 4) == FALSE);  // 1, 7の場所に、他の石があるため、三にならない
    assert(__getTargetLengthLinesInDirection(".X@X.O.", 3) == FALSE);  // 左が壁となり白が止めることができるため、三にならない


} */


void testSimpleProhibitedDoubleThree(){
    Board board;
    // {4, 3}, {4, 7}, {9, 7} は3x3を作る禁じ手
    const char *lines[] = {
        NULL,
        ".........",
        "..X......",
        "..X......",
        "..@XX.@..",
        "........",
        "......X..",
        "......X..",
        ".........",
        ".....X@X."
    };
    initBoardWithStr(&board, lines);
}

void runGameTests() {
    printf("Start runGameTests...\n");
    testHasWinnerInVerticalExpected();
    testHasWinnerInHorizontalExpected();
    testHasWinnerInCrossExpected();
    testNotHasWinnerExpected();
    testGetTargetLengthLinesInDirection();
    printf("Finished runGameTests.\n");
}

