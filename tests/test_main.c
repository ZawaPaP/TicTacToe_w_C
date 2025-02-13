#include <stdio.h>
#include "test_utils.h"
#include "test_queue.h"
#include "test_board.h"
#include "test_plays.h"
#include "test_game.h"
#include "test_cpu.h"

int main() {
    printf("\nStart running the test...\n\n");
    runUtilsTests();
    runQueueTests();
    runBoardTests();
    runPlaysTests();
    runGameTests();
    runCPUTests();

    printf("\nFinished all tests.\n\n");
    return 0;
}
