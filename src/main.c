#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "board.h"
#include "plays.h"

int main() {
    startGame();

    Board *board = (Board *)malloc(sizeof(Board));
    initBoard(board);

    printBoard(board);
    //printGameStatus(i, j);

    int x, y;
    getInput(&x, &y);
    free(board);
    return 0;
}
