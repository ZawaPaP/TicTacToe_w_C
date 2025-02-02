#include <stdio.h>
#include "test_queue.h"
#include "test_board.h"
#include "test_plays.h"
#include "test_game.h"

int main() {
    printf("\nStart running the test...\n\n");
    runQueueTests();
    runBoardTests();
    runPlaysTests();
    runGameTests();

    printf("\nFinished all tests.\n\n");
    return 0;
}
