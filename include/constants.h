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

// ライン長さスコア
#define WIN_POINTS 999999
#define OPEN_FOUR_POINTS 5000
#define CLOSED_FOUR_POINTS 500
#define OPEN_THREE_POINTS 500
#define CLOSED_THREE_POINTS 100
#define OPEN_TWO_POINTS 100
#define CLOSED_TWO_POINTS 25

// ポジショニングスコア
#define CENTER_START_ROW (BOARD_ROWS / 3)
#define CENTER_END_ROW ((BOARD_ROWS * 2) / 3)
#define CENTER_START_COL (BOARD_COLUMNS / 3)
#define CENTER_END_COL ((BOARD_COLUMNS * 2) / 3)
#define CENTER_MULTIPLIER 2

typedef enum 
{
    TRUE = 1,
    FALSE = 0
} BOOL;

typedef struct {
    char cells[BOARD_ROWS + 1][BOARD_COLUMNS + 1];
    int lastRow, lastCol;
} Board;

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

typedef struct {
    int r;
    int c;
} Cell;

// 方向を表す構造体
typedef struct
{
    int dx;  // x方向の変化量
    int dy;  // y方向の変化量
} Direction;

typedef struct {
    Cell start;
    Cell end;
    int length;
    Direction dir[2];
} LineIdx;

typedef struct {
    int startIdx;
    int endIdx;
    BOOL hasGap;
} LineInfo;

typedef struct {
    LineInfo lines[10];
    int count;
} LineInfoArray;

// gap込みで長さを測ると、gapの位置によって、同じラインが最大２パターンの長さを持つ。
typedef struct {
    LineIdx lines[2];  // 最大2パターンを格納
    int pattern; // 見つかったパターンの数（1または2）
} LinePatterns;

#endif
