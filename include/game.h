#ifndef GAME_H
#define GAME_H

#define WIN_LENGTH 5

void playGame();
void printGameStatus(int turnCounts, char player);
int hasWinner(Board *board);
#endif
