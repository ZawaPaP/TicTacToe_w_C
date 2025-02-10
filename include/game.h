#ifndef GAME_H
#define GAME_H
#include "constants.h"

void playGame();
void printGameStatus(int turnCounts, char player);
int hasWinner(Board *board, char playerMark);
#endif
