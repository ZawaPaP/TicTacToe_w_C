#ifndef GAME_H
#define GAME_H
#include "constants.h"

void manageGame();
char getWinner(Board *board);
BOOL hasWinner(Board *board, char playerMark);
#endif
