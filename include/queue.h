#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 100

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position positions[QUEUE_SIZE];
    int head;
    int tail;
} Queue;

void initQueue(Queue *q);
int isEmpty(Queue *q);
int isFull(Queue *q);
void push(Queue *q, Position p);
Position pop(Queue *q);

#endif
