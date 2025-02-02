#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/plays.h"
#include "../include/board.h"
#include "test_plays.h"


Board _prepareBoard() {
    Board board;
    initBoard(&board);
    return board;
}


void testPlaceMoveExpected(){

    Board board = _prepareBoard();
    assert(placeMove(5, 5, &board, PLAYER_X) == 1);
    assert(board.cells[5][5] == PLAYER_X);  

    // ４隅のエッジケースについてテスト
    assert(placeMove(1, 1, &board, PLAYER_X) == 1);
    assert(board.cells[1][1] == PLAYER_X);  
    
    assert(placeMove(BOARD_ROWS, 1, &board, PLAYER_X) == 1);
    assert(board.cells[BOARD_ROWS][1] == PLAYER_X);  

    assert(placeMove(1, BOARD_COLUMNS, &board, PLAYER_O) == 1);
    assert(board.cells[1][BOARD_COLUMNS] == PLAYER_O);  

    assert(placeMove(BOARD_ROWS, BOARD_COLUMNS, &board, PLAYER_O) == 1);
    assert(board.cells[BOARD_ROWS][BOARD_COLUMNS] == PLAYER_O);  
}


void testPlaceMoveFailedAlreadyMarked() {
    Board board = _prepareBoard();
    board.cells[5][5] = PLAYER_X;

    assert(placeMove(5, 5, &board, PLAYER_O) == 0);
    assert(board.cells[5][5] == PLAYER_X);
}

void testPlaceMoveFailedOutOfRange() {
    Board board = _prepareBoard();
    assert(placeMove(BOARD_ROWS + 1, BOARD_COLUMNS, &board, PLAYER_O) == 0);
}

void testGetInputExpectedStr()
{
    //"1, 2\n" →1, 2
    int x, y;
    char input[] = "1, 2\n";
    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);

    FILE* input_stream = fmemopen(input, sizeof(input), "r");
    stdin = input_stream;
    assert(getInput(&x, &y) == 1);
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(input_stream);
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testGetInputWithoutComma()
{
    //"1 2\n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testGetInputWithSpace()
{
    //"1 , 2 \n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1 , 2 \n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testGetInputWithoutSpace()
{
    //"1,2\n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1,2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testGetInputFailedWithAlphabet()
{
    //"a b\n"
    int x = 0, y = 0;
    char input[] = "a b\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 
    assert(x == 0 && y == 0);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);

}

void testGetInputFailedFloat()
{
    //"3.5 2\n"
    int x = 0, y = 0;
    char input[] = "3.5 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);

}

void testGetInputFailedEmpty()
{
    //" \n"
    int x = 0, y = 0;
    char input[] = "\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testGetInputFailedEOF()
{
    //"EOF"
    int x = 0, y = 0;
    char input[] = "EOF";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testGetInputFailedTooLongInt()
{
    //"12345678901234567890, 12345678901234567890\n"
    int x = 0, y = 0;
    char input[] = "12345678901234567890, 12345678901234567890\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
    fclose(stdin_backup);
    fclose(stdout_backup);
}

void testValidateInputExpectedRange() {
    Board board = _prepareBoard();

    int row = 5;
    int col = 5;
    assert(validateInput(row, col, &board) == 1);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    assert(validateInput(rowEdgeBegin, colEdgeBegin, &board) == 1);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    assert(validateInput(rowEdgeEnd, colEdgeEnd, &board) == 1);

}

void testValidateInputFailedOutOfRange()
 {
    Board board = _prepareBoard();

    int zeroRow = 0;
    int zeroCol = 0;

    assert(validateInput(zeroRow, zeroCol, &board) == 0);
    
    int negativeInt = -1;
    int randInt = 4;
    assert(validateInput(negativeInt, randInt, &board) == 0);

    int ToobigInt = 123490;
    assert(validateInput(ToobigInt, randInt, &board) == 0);
 }

 void testValidateInputFailedNotEmpty() {
     Board board = _prepareBoard();

     int row = 5;
     int col = 5;

     board.cells[row][col] = PLAYER_O;
    assert(validateInput(row, col, &board) == 0);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    board.cells[rowEdgeBegin][colEdgeBegin] = PLAYER_O;
    assert(validateInput(rowEdgeBegin, colEdgeBegin, &board) == 0);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    board.cells[rowEdgeEnd][colEdgeEnd] = PLAYER_O;
    assert(validateInput(rowEdgeEnd, colEdgeEnd, &board) == 0);
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

