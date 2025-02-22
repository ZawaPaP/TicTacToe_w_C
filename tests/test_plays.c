#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/plays.h"
#include "test_utils.h"
#include "test_plays.h"


void testPrintBoard(){
    printf("Start testPrintBoard...\n");

    Board board;

    initBoard(&board);

    board.cells[1][1] = PLAYER_X;
    board.cells[5][7] = PLAYER_O;
    board.cells[9][9] = PLAYER_X;

    freopen("test_print_board_output.txt", "w", stdout);
    printBoard(&board);
    freopen("/dev/tty", "w", stdout);

    FILE* fp = fopen("test_print_board_output.txt", "r");
    if (!fp) {
        printf("Failed to open test_print_board_output.txt\n");
        return;
    }

    const char *expected[] = {
        "    1   2   3   4   5   6   7   8   9   \n",
        "\n",
        "1   X |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "2     |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "3     |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "4     |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "5     |   |   |   |   |   | O |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "6     |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "7     |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "8     |   |   |   |   |   |   |   |  \n",
        "    - + - + - + - + - + - + - + - + - \n",
        "9     |   |   |   |   |   |   |   | X\n",
        "\n", 
        NULL
        };

    char buffer[100];
    int line = 0;
    int pass = 1;

    while(fgets(buffer, sizeof(buffer), fp) != NULL) {

        if (expected[line] == NULL) break;

        if (strcmp(buffer, expected[line]) != 0) {
            printf("Error: Mismatch on line %d:\nExpected: \"%s\"\nGot:      \"%s\"\n", line + 1, expected[line], buffer);
            pass = 0;
        }
        line++;
    }
    fclose(fp);

    assert(pass == TRUE);
    remove("test_print_board_output.txt");
}



void testPlaceMoveExpected(){

    Board board = __prepareBoard();
    assert(canApplyMove(5, 5, &board, PLAYER_X) == TRUE);

    // ４隅のエッジケースについてテスト
    assert(canApplyMove(1, 1, &board, PLAYER_X) == TRUE);
    
    assert(canApplyMove(BOARD_ROWS, 1, &board, PLAYER_X) == TRUE);

    assert(canApplyMove(1, BOARD_COLUMNS, &board, PLAYER_X) == TRUE);

    assert(canApplyMove(BOARD_ROWS, BOARD_COLUMNS, &board, PLAYER_X) == TRUE);
}


void testPlaceMoveFailedAlreadyMarked() {
    Board board = __prepareBoard();
    board.cells[5][5] = PLAYER_X;

    assert(canApplyMove(5, 5, &board, PLAYER_X) == FALSE);
}

void testPlaceMoveFailedOutOfRange() {
    Board board = __prepareBoard();
    assert(canApplyMove(BOARD_ROWS + 1, BOARD_COLUMNS, &board, PLAYER_X) == FALSE);
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
    assert(isValidMove(row, col, &board, PLAYER_X) == TRUE);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    assert(isValidMove(rowEdgeBegin, colEdgeBegin, &board, PLAYER_X) == TRUE);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    assert(isValidMove(rowEdgeEnd, colEdgeEnd, &board, PLAYER_X) == TRUE);

}

void testValidateInputFailedOutOfRange()
 {
    printf("000000");

    Board board = __prepareBoard();

    int zeroRow = 0;
    int zeroCol = 0;

    assert(isValidMove(zeroRow, zeroCol, &board, PLAYER_X) == FALSE);
    
    int negativeInt = -1;
    int randInt = 4;
    assert(isValidMove(negativeInt, randInt, &board, PLAYER_X) == FALSE);

    int ToobigInt = 123490;
    assert(isValidMove(ToobigInt, randInt, &board, PLAYER_X) == FALSE);
 }

 void testValidateInputFailedNotEmpty() {
     Board board = __prepareBoard();

     int row = 5;
     int col = 5;

     board.cells[row][col] = PLAYER_O;
    assert(isValidMove(row, col, &board, PLAYER_X) == FALSE);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    board.cells[rowEdgeBegin][colEdgeBegin] = PLAYER_O;
    assert(isValidMove(rowEdgeBegin, colEdgeBegin, &board, PLAYER_X) == FALSE);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    board.cells[rowEdgeEnd][colEdgeEnd] = PLAYER_O;
    assert(isValidMove(rowEdgeEnd, colEdgeEnd, &board, PLAYER_X) == FALSE);
 }

void runPlaysTests() {
    printf("Start runPlaysTests...\n");
    testPrintBoard();
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

