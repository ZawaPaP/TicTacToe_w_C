#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WIN_LENGTH 5

#define BOARD_ROWS 9
#define BOARD_COLUMNS 9

#define PLAYER_X 'X'
#define PLAYER_O 'O'
#define EMPTY_CELL ' '

#define QUEUE_SIZE 100

#define NEGA_MAX_DEPTH 2

#define WIN_POINTS 999999
#define OPEN_FOUR_POINTS 5000
#define CLOSED_FOUR_POINTS 500
#define OPEN_THREE_POINTS 500
#define CLOSED_THREE_POINTS 100
#define OPEN_TWO_POINTS 100
#define CLOSED_TWO_POINTS 25

typedef enum 
{
    TRUE = 1,
    FALSE = 0
} BOOL;

typedef enum
{
    PLAYER_PLAYER = 1,
    PLAYER_CPU = 2,
    CPU_CPU = 3
} MODE;

typedef enum
{
    OPEN = 0,
    SEMI_CLOSE = 1,
    CLOSED = 2
} EDGE_STATUS;

#endif
