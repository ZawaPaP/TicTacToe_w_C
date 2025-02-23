#ifndef UI_H
#define UI_H
#include "constants.h"
#include "board.h"


BOOL isValidMoveInput(int *x, int *y);
BOOL handlePlayerInput(int *row, int *col);
void displayThanksMessage(void);
void printBoard(Game *game);
void announceResult(const Game *game);
void printGameStatus(int turnCounts, char player);
BOOL askForRematch(void);
MODE selectGameMode();
#endif
