#ifndef QUEUE_H
#define QUEUE_H

#include "constants.h"

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position *positions;
    int head;
    int tail;
    int size;
    int capacity;
} Queue;

void initQueue(Queue *q);
int isEmpty(Queue *q);
int isFull(Queue *q);
void push(Queue *q, Position p);
Position pop(Queue *q);
void freeQueue(Queue *q);

#endif
