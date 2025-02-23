#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ui.h"
#include "../include/game.h"
#include "test_utils.h"
#include "test_ui.h"

void testPrintBoard(TestResults* results) {
    test_begin("PrintBoard");

    Game game;
    initGame(&game, PLAYER_PLAYER);

    game.board.cells[1][1] = PLAYER_X;
    game.board.cells[5][7] = PLAYER_O;
    game.board.cells[9][9] = PLAYER_X;

    game.moveCount = 3;
    game.moveHistory[2].move.r = 9;
    game.moveHistory[2].move.c = 9;
    game.moveHistory[2].player = PLAYER_X;

    FILE* fp = fopen("test_print_board_output.txt", "w");
    FILE* stdout_backup = stdout;
    stdout = fp;
    printBoard(&game);
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
        "9\t  |   |   |   |   |   |   |   | \x1b[32mX\x1b[39m\n", 
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



void runUiTests() {
    TestResults results = {0, 0, 0};
    test_suite_begin("UI Tests");
    
    suppress_output();

    testPrintBoard(&results);
    testGetInputExpectedStr(&results);
    testGetInputWithoutComma(&results);
    testGetInputWithSpace(&results);
    testGetInputWithoutSpace(&results);
    testGetInputFailedWithAlphabet(&results);
    testGetInputFailedFloat(&results);
    testGetInputFailedEmpty(&results);
    testGetInputFailedEOF(&results);
    testGetInputFailedTooLongInt(&results);
    
    restore_output();
    
    test_suite_end("UI Tests", &results);
}

