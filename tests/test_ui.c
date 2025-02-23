#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ui.h"
#include "test_utils.h"
#include "test_ui.h"

void testPrintBoard(TestResults* results) {
    test_begin("PrintBoard");

    Board board;
    initBoard(&board);
    board.cells[1][1] = PLAYER_X;
    board.cells[5][7] = PLAYER_O;
    board.cells[9][9] = PLAYER_X;

    FILE* fp = fopen("test_print_board_output.txt", "w");
    FILE* stdout_backup = stdout;
    stdout = fp;
    printBoard(&board);
    stdout = stdout_backup;
    fclose(fp);

    fp = fopen("test_print_board_output.txt", "r");
    if (!fp) {
        test_assert(FALSE, "Failed to open test output file", results);
        test_end("PrintBoard");
        return;
    }

    const char *expected[] = {
        "\t1   2   3   4   5   6   7   8   9   \n",
        "\n",
        "1\tX |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "2\t  |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "3\t  |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "4\t  |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "5\t  |   |   |   |   |   | O |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "6\t  |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "7\t  |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "8\t  |   |   |   |   |   |   |   |  \n",
        "\t- + - + - + - + - + - + - + - + -\n",
        "9\t  |   |   |   |   |   |   |   | X\n",
        "\n", 
        NULL
    };

    char buffer[100];
    int line = 0;
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (expected[line] == NULL) break;
        test_assert(strcmp(buffer, expected[line]) == 0,
                   "Board output should match expected format", results);
        line++;
    }

    fclose(fp);
    remove("test_print_board_output.txt");
    
    test_end("PrintBoard");
}

void testPlaceMoveExpected(TestResults* results) {
    test_begin("PlaceMoveExpected");

    Board board = __prepareBoard();
    test_assert(canApplyMove(5, 5, &board, PLAYER_X) == TRUE,
                "Should allow move to empty center position", results);

    test_assert(canApplyMove(1, 1, &board, PLAYER_X) == TRUE,
                "Should allow move to top-left corner", results);
    
    test_assert(canApplyMove(BOARD_ROWS, 1, &board, PLAYER_X) == TRUE,
                "Should allow move to bottom-left corner", results);

    test_assert(canApplyMove(1, BOARD_COLUMNS, &board, PLAYER_X) == TRUE,
                "Should allow move to top-right corner", results);

    test_assert(canApplyMove(BOARD_ROWS, BOARD_COLUMNS, &board, PLAYER_X) == TRUE,
                "Should allow move to bottom-right corner", results);

    test_end("PlaceMoveExpected");
}

void testPlaceMoveFailedAlreadyMarked(TestResults* results) {
    Board board = __prepareBoard();
    board.cells[5][5] = PLAYER_X;

    test_assert(canApplyMove(5, 5, &board, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);
}

void testPlaceMoveFailedOutOfRange(TestResults* results) {
    Board board = __prepareBoard();
    test_assert(canApplyMove(BOARD_ROWS + 1, BOARD_COLUMNS, &board, PLAYER_X) == FALSE,
                "Should not allow move out of range", results);
}

void testGetInputExpectedStr(TestResults* results) {
    test_begin("GetInputExpectedStr");

    int x, y;
    char input[] = "1, 2\n";
    FILE* stdin_backup = stdin;
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == TRUE,
                "Should accept valid input format '1, 2'", results);
    test_assert(x == 1 && y == 2,
                "Should correctly parse coordinates", results);

    stdin = stdin_backup;
    
    test_end("GetInputExpectedStr");
}

void testGetInputWithoutComma(TestResults* results) {
    test_begin("GetInputWithoutComma");

    int x = 0, y = 0;
    char input[] = "1 2\n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == TRUE,
                "Should accept valid input format '1 2'", results);
    test_assert(x == 1 && y == 2,
                "Should correctly parse coordinates", results);

    stdin = stdin_backup;
    
    test_end("GetInputWithoutComma");
}

void testGetInputWithSpace(TestResults* results) {
    test_begin("GetInputWithSpace");

    int x = 0, y = 0;
    char input[] = "1 , 2 \n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == TRUE,
                "Should accept valid input format '1 , 2 '", results);
    test_assert(x == 1 && y == 2,
                "Should correctly parse coordinates", results);

    stdin = stdin_backup;
    
    test_end("GetInputWithSpace");
}

void testGetInputWithoutSpace(TestResults* results) {
    test_begin("GetInputWithoutSpace");

    int x = 0, y = 0;
    char input[] = "1,2\n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == TRUE,
                "Should accept valid input format '1,2'", results);
    test_assert(x == 1 && y == 2,
                "Should correctly parse coordinates", results);

    stdin = stdin_backup;
    
    test_end("GetInputWithoutSpace");
}

void testGetInputFailedWithAlphabet(TestResults* results) {
    test_begin("GetInputFailedWithAlphabet");

    int x = 0, y = 0;
    char input[] = "a b\n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == FALSE,
                "Should not accept input with alphabets", results);
    test_assert(x == FALSE && y == FALSE,
                "Should return FALSE for invalid input", results);

    stdin = stdin_backup;
    
    test_end("GetInputFailedWithAlphabet");
}

void testGetInputFailedFloat(TestResults* results) {
    test_begin("GetInputFailedFloat");

    int x = 0, y = 0;
    char input[] = "3.5 2\n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == FALSE,
                "Should not accept floating point numbers", results);

    stdin = stdin_backup;
    
    test_end("GetInputFailedFloat");
}

void testGetInputFailedEmpty(TestResults* results) {
    test_begin("GetInputFailedEmpty");

    int x = 0, y = 0;
    char input[] = "\n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == FALSE,
                "Should not accept empty input", results);

    stdin = stdin_backup;
    
    test_end("GetInputFailedEmpty");
}

void testGetInputFailedEOF(TestResults* results) {
    test_begin("GetInputFailedEOF");

    int x = 0, y = 0;
    char input[] = "EOF";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == FALSE,
                "Should not accept EOF", results);

    stdin = stdin_backup;
    
    test_end("GetInputFailedEOF");
}

void testGetInputFailedTooLongInt(TestResults* results) {
    test_begin("GetInputFailedTooLongInt");

    int x = 0, y = 0;
    char input[] = "12345678901234567890, 12345678901234567890\n";
    FILE* stdin_backup = stdin;
    
    stdin = fmemopen(input, sizeof(input), "r");

    test_assert(isValidMoveInput(&x, &y) == FALSE,
                "Should not accept too long integers", results);

    stdin = stdin_backup;
    
    test_end("GetInputFailedTooLongInt");
}

void testValidateInputExpectedRange(TestResults* results) {
    test_begin("ValidateInputExpectedRange");

    printf("000000");

    Board board = __prepareBoard();

    int row = 5;
    int col = 5;
    test_assert(isValidMove(row, col, &board, PLAYER_X) == TRUE,
                "Should allow move in valid range", results);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    test_assert(isValidMove(rowEdgeBegin, colEdgeBegin, &board, PLAYER_X) == TRUE,
                "Should allow move to top-left corner", results);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    test_assert(isValidMove(rowEdgeEnd, colEdgeEnd, &board, PLAYER_X) == TRUE,
                "Should allow move to bottom-right corner", results);

    test_end("ValidateInputExpectedRange");
}

void testValidateInputFailedOutOfRange(TestResults* results) {
    test_begin("ValidateInputFailedOutOfRange");

    printf("000000");

    Board board = __prepareBoard();

    int zeroRow = 0;
    int zeroCol = 0;

    test_assert(isValidMove(zeroRow, zeroCol, &board, PLAYER_X) == FALSE,
                "Should not allow move in zero range", results);
    
    int negativeInt = -1;
    int randInt = 4;
    test_assert(isValidMove(negativeInt, randInt, &board, PLAYER_X) == FALSE,
                "Should not allow move in negative range", results);

    int ToobigInt = 123490;
    test_assert(isValidMove(ToobigInt, randInt, &board, PLAYER_X) == FALSE,
                "Should not allow move in too big range", results);

    test_end("ValidateInputFailedOutOfRange");
}

void testValidateInputFailedNotEmpty(TestResults* results) {
    test_begin("ValidateInputFailedNotEmpty");

    Board board = __prepareBoard();

    int row = 5;
    int col = 5;

    board.cells[row][col] = PLAYER_O;
    test_assert(isValidMove(row, col, &board, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);

    int rowEdgeBegin = 1;
    int colEdgeBegin = 1;
    board.cells[rowEdgeBegin][colEdgeBegin] = PLAYER_O;
    test_assert(isValidMove(rowEdgeBegin, colEdgeBegin, &board, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);

    int rowEdgeEnd = BOARD_ROWS;
    int colEdgeEnd = BOARD_COLUMNS;
    board.cells[rowEdgeEnd][colEdgeEnd] = PLAYER_O;
    test_assert(isValidMove(rowEdgeEnd, colEdgeEnd, &board, PLAYER_X) == FALSE,
                "Should not allow move to already marked position", results);

    test_end("ValidateInputFailedNotEmpty");
}

void runUiTests() {
    TestResults results = {0, 0, 0};
    test_suite_begin("UI Tests");
    
    suppress_output();
    
    testPrintBoard(&results);
    testPlaceMoveExpected(&results);
    testPlaceMoveFailedAlreadyMarked(&results);
    testGetInputExpectedStr(&results);
    testGetInputWithoutComma(&results);
    testGetInputWithSpace(&results);
    testGetInputWithoutSpace(&results);
    testGetInputFailedWithAlphabet(&results);
    testGetInputFailedFloat(&results);
    testGetInputFailedEmpty(&results);
    testGetInputFailedEOF(&results);
    testGetInputFailedTooLongInt(&results);
    testValidateInputExpectedRange(&results);
    testValidateInputFailedOutOfRange(&results);
    testValidateInputFailedNotEmpty(&results);
    
    restore_output();
    
    test_suite_end("UI Tests", &results);
}

