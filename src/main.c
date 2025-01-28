#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "board.h"

int main() {
    startGame();

    Board *board = (Board *)malloc(sizeof(Board));
    initBoard(board);

    int i = 3;
    int j = 1;
    printBoard(board);
    printGameStatus(i, j);
    free(board);
    return 0;
}
