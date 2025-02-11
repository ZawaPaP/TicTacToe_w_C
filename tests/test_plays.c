#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/plays.h"
#include "test_utils.h"
#include "test_plays.h"



void testPlaceMoveExpected(){

    Board board = __prepareBoard();
    assert(canApplyMove(5, 5, &board) == TRUE);

    // ４隅のエッジケースについてテスト
    assert(canApplyMove(1, 1, &board) == TRUE);
    
    assert(canApplyMove(BOARD_ROWS, 1, &board) == TRUE);

    assert(canApplyMove(1, BOARD_COLUMNS, &board) == TRUE);

    assert(canApplyMove(BOARD_ROWS, BOARD_COLUMNS, &board) == TRUE);
}


void testPlaceMoveFailedAlreadyMarked() {
    Board board = __prepareBoard();
    board.cells[5][5] = PLAYER_X;

    assert(canApplyMove(5, 5, &board) == FALSE);
}

void testPlaceMoveFailedOutOfRange() {
    Board board = __prepareBoard();
    assert(canApplyMove(BOARD_ROWS + 1, BOARD_COLUMNS, &board) == FALSE);
}

void testGetInputExpectedStr()
{
    //"1, 2\n" →1, 2
    int x, y;
    char input[] = "1, 2\n";
    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 

    stdin = fmemopen(input, sizeof(input), "r");
    assert(isValidMoveInput(&x, &y) == TRUE);
    assert(x == TRUE && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputWithoutComma()
{
    //"1 2\n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == TRUE); 
    assert(x == TRUE && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputWithSpace()
{
    //"1 , 2 \n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1 , 2 \n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == TRUE); 
    assert(x == TRUE && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputWithoutSpace()
{
    //"1,2\n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1,2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == TRUE); 
    assert(x == TRUE && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputFailedWithAlphabet()
{
    //"a b\n"
    int x = 0, y = 0;
    char input[] = "a b\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == FALSE); 
    assert(x == FALSE && y == FALSE);

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputFailedFloat()
{
    //"3.5 2\n"
    int x = 0, y = 0;
    char input[] = "3.5 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == FALSE); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputFailedEmpty()
{
    //" \n"
    int x = 0, y = 0;
    char input[] = "\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == FALSE); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputFailedEOF()
{
    //"EOF"
    int x = 0, y = 0;
    char input[] = "EOF";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin;     
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == FALSE); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testGetInputFailedTooLongInt()
{
    //"12345678901234567890, 12345678901234567890\n"
    int x = 0, y = 0;
    char input[] = "12345678901234567890, 12345678901234567890\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(isValidMoveInput(&x, &y) == FALSE); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
}

void testValidateInputExpectedRange() {
    printf("000000");

    Board board = __prepareBoard();

    int row = 5;
    int col = 5;
    assert(isValidMove(row, col, &board) == TRUE);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    assert(isValidMove(rowEdgeBegin, colEdgeBegin, &board) == TRUE);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    assert(isValidMove(rowEdgeEnd, colEdgeEnd, &board) == TRUE);

}

void testValidateInputFailedOutOfRange()
 {
    printf("000000");

    Board board = __prepareBoard();

    int zeroRow = 0;
    int zeroCol = 0;

    assert(isValidMove(zeroRow, zeroCol, &board) == FALSE);
    
    int negativeInt = -1;
    int randInt = 4;
    assert(isValidMove(negativeInt, randInt, &board) == FALSE);

    int ToobigInt = 123490;
    assert(isValidMove(ToobigInt, randInt, &board) == FALSE);
 }

 void testValidateInputFailedNotEmpty() {
     Board board = __prepareBoard();

     int row = 5;
     int col = 5;

     board.cells[row][col] = PLAYER_O;
    assert(isValidMove(row, col, &board) == FALSE);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    board.cells[rowEdgeBegin][colEdgeBegin] = PLAYER_O;
    assert(isValidMove(rowEdgeBegin, colEdgeBegin, &board) == FALSE);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    board.cells[rowEdgeEnd][colEdgeEnd] = PLAYER_O;
    assert(isValidMove(rowEdgeEnd, colEdgeEnd, &board) == FALSE);
 }

void runPlaysTests() {
    printf("Start runPlaysTests...\n");
    printf("Start placeMoveTests...\n");
    testPlaceMoveExpected();
    testPlaceMoveFailedAlreadyMarked();
    printf("Finished placeMoveTests...\n");
    printf("Start getInputTests...\n");
    testGetInputExpectedStr();
    testGetInputWithoutComma();
    testGetInputWithSpace();
    testGetInputWithoutSpace();
    testGetInputFailedWithAlphabet();
    testGetInputFailedFloat();
    testGetInputFailedEmpty();
    testGetInputFailedEOF();
    testGetInputFailedTooLongInt();
    printf("Finished getInputTests...\n");
    printf("Start validateInputTests...\n");
    testValidateInputExpectedRange();
    testValidateInputFailedOutOfRange();
    testValidateInputFailedNotEmpty();
    printf("Finished validateInputTests...\n");
    printf("Finished runPlaysTests.\n");
}

