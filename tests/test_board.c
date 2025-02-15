#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "test_utils.h"
#include "test_board.h"


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
    if (pass) {
        printf("test testPrintBoard success.\n");
    } else {
        printf("test Failed.\n");
    }
    remove("test_print_board_output.txt");
}

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

    lineInfo line1 = {.startIdx = -2, .endIdx = 1, .hasGap = FALSE};
    lineInfo line2 = {.startIdx = -1, .endIdx = 2, .hasGap = FALSE};
    
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
    
    lineInfo line3 = {.startIdx = -1, .endIdx = 2, .hasGap = FALSE};
    lineInfo line4 = {.startIdx = -2, .endIdx = 1, .hasGap = FALSE};
    
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
    
    lineInfo line5 = {.startIdx = -4, .endIdx = 0, .hasGap = TRUE};
    lineInfo line6 = {.startIdx = -3, .endIdx = 1, .hasGap = TRUE};
    
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
    
    lineInfo line7 = {.startIdx = -3, .endIdx = 0, .hasGap = TRUE};
    lineInfo line8 = {.startIdx = 0, .endIdx = 3, .hasGap = TRUE};
    
    assert(__isSameLine(&board, &line7, &line8, 2, 5, 0, 1) == FALSE);

    printf("All testIsSameLine passed!\n");
}


void runBoardTests() {
    printf("Start runBoardTests...\n");
    testPrintBoard();
    testInitBoard();
    testIsSameLine();
    printf("Finished runBoardTests.\n");
}
