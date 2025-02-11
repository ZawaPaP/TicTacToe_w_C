#ifndef GAME_H
#define GAME_H
#include "constants.h"

void runGameLoop();
char getWinner(Board *board);
BOOL hasWinner(Board *board, char playerMark);
#endif
