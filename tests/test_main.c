#include <stdio.h>
#include "test_board.h"
#include "test_plays.h"

int main() {
    printf("\nStart running the test...\n\n");

    runBoardTests();
    runPlaysTests();

    printf("\nFinished all tests.\n\n");
    return 0;
}
