#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void initQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    q->capacity = QUEUE_SIZE;
    q->positions = (Position *)malloc(sizeof(Position) * QUEUE_SIZE);
    if (q->positions == NULL)
        exit(1);
}

BOOL isEmpty(Queue *q){
    return (q->size == 0) ? TRUE : FALSE;
}

BOOL isFull(Queue *q){
    return (q->size == q->capacity) ? TRUE : FALSE;
}

void extendQueue(Queue *q) {
    int newCapacity = q->capacity * 2;
    Position *newPositions = (Position *)realloc(q->positions, sizeof(Position) * newCapacity);
    if (!newPositions) {
        printf("Error: Memory realloc failed.\n");
        freeQueue(q);
        exit(1);
    }

    // もしqueueが循環していた場合、循環分だけを配置し直し

    if (q->tail <= q->head) {
        for (int i = 0; i < q->tail; i++) {
            newPositions[q->capacity + i] = q->positions[i];
        }
        q->tail += q->capacity;
    }
    q->positions = newPositions;
    q->capacity = newCapacity;
}

void push(Queue *q, Position p){
    if (isFull(q)) {
        extendQueue(q);
    }
    q->positions[q->tail] = p;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
}

Position pop(Queue *q){
    if (isEmpty(q)) {
        exit(1);
    }
    Position front = q->positions[q->head];
    q->size--;
    q->head = (q->head + 1) % q->capacity;
    return front;
}

void freeQueue(Queue *q) {
    free(q->positions);
    q->positions = NULL;
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    q->capacity = 0;
}
