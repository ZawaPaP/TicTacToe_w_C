#include <stdio.h>
#include "test_utils.h"
#include "test_queue.h"
#include "test_board.h"
#include "test_ui.h"
#include "test_game.h"
#include "test_cpu.h"

int main() {
    fprintf(stderr, "Starting All Tests...\n\n");
    runUtilsTests();
    runQueueTests();
    runBoardTests();
    runUiTests();
    runGameTests();
    runCPUTests();

   fprintf(stderr, "\nAll Tests Completed.\n");
    return 0;
}
