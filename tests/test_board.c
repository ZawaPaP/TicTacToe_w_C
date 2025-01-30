#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"
#include "test_board.h"


// This helper function is too much but keep and use it in the future.
/* static void placeRandomMarks(Board *board) {

    int place_marks = rand() % 11;

    if (place_marks > BOARD_ROWS * BOARD_COLUMNS) {
        printf("Error: too big mark counts.\n");
    }

    for (int i = 0; i < place_marks; i++) {
        int r = rand() % BOARD_ROWS;
        int c = rand() % BOARD_COLUMNS;
        
        board->cells[r][c] = (rand() % 2) ? PLAYER_X : PLAYER_O;
    }
} */

void testPrintBoard(){
    printf("Start testPrintBoard...\n");

    Board board;

    initBoard(&board);

    board.cells[0][0] = PLAYER_X;
    board.cells[4][6] = PLAYER_O;
    board.cells[8][8] = PLAYER_X;

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

    board.cells[0][0] = PLAYER_X;
    board.cells[4][6] = PLAYER_O;
    board.cells[8][8] = PLAYER_X;
    
    initBoard(&board);

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLUMNS; j++) {
            if (board.cells[i][j] != EMPTY_CELL) {
                printf("Error: %d, %d is not empty.\n", i, j);
                assert(board.cells[i][j] == EMPTY_CELL);
            }
        }
    }
    printf("test testInitBoard success.\n");
}

void runBoardTests() {
    printf("Start runBoardTests...\n");
    testPrintBoard();
    testInitBoard();
    printf("Finished runBoardTests.\n");
}
