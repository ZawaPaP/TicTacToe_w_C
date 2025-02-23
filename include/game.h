#ifndef GAME_H
#define GAME_H
#include "constants.h"

void
runGameLoop();
void initGame(Game *game, MODE mode);
void switchPlayer(Game *game);
BOOL isGameOver(Board *board, int *row, int *col, char player);
BOOL isValidMove(Board *board, int row, int col, char playerMark);
#endif
