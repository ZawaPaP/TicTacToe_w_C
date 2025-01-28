#include <stdio.h>

void startGame() {
    printf("\n        TicTacToe Game Started!\n\n");
}

void printGameStatus(int i, int j) {
    char str[8];

    printf("Turn %d\n", i);
    printf("Player %d, please input your hand: ", j);
    
    fgets(str, sizeof(str), stdin);
    puts(str);
}

void printGameResult() {

}