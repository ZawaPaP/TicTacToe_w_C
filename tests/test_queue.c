#include <stdio.h>
#include <assert.h>
#include "../include/queue.h"
#include "test_queue.h"

void testQueueInitialization() {
    printf("Start testQueueInitialization...\n");
    Queue q;
    initQueue(&q);

    assert(q.size == 0);
    assert(q.head == 0);
    assert(q.tail == 0);
    assert(q.capacity == QUEUE_SIZE);

    freeQueue(&q);
    printf("testQueueInitialization passed.\n");
}

void testQueuePushPop() {
    printf("Start testQueuePushPop...\n");
    
    Queue q;
    initQueue(&q);

    Position p1 = {1, 2};
    Position p2 = {3, 4};
    Position p3 = {5, 6};

    push(&q, p1);
    push(&q, p2);
    push(&q, p3);

    assert(q.size == 3);

    Position out1 = pop(&q);
    assert(out1.x == 1 && out1.y == 2);
    assert(q.size == 2);

    Position out2 = pop(&q);
    assert(out2.x == 3 && out2.y == 4);
    assert(q.size == 1);

    Position out3 = pop(&q);
    assert(out3.x == 5 && out3.y == 6);
    assert(q.size == 0);
    assert(isEmpty(&q) == TRUE);

    freeQueue(&q);
    printf("testQueuePushPop passed.\n");
}

void testQueueExtend()
{
    printf("Start testQueueExtend...\n");

    Queue q;
    initQueue(&q);

    for (int i = 0; i < QUEUE_SIZE; i++) {
        push(&q, (Position){i, i * 2});
    }

    assert(q.size == QUEUE_SIZE);
    assert(q.capacity == QUEUE_SIZE);
    assert(isFull(&q) == TRUE);

    push(&q, (Position){100, 200});

    assert(q.size == QUEUE_SIZE + 1);
    assert(q.capacity == QUEUE_SIZE * 2);
    assert(isFull(&q) == FALSE);

    for (int i = 0; i <= QUEUE_SIZE; i++) {
        Position p = pop(&q);
        assert(p.x == i);
        assert(p.y == i * 2);
    }

    assert(isEmpty(&q) == TRUE);

    printf("Finished testQueueExtend...\n");
}

void runQueueTests(){
    printf("Start runQueueTests...\n");
    testQueueInitialization();
    testQueuePushPop();
    testQueueExtend();
    printf("Finished runQueueTests.\n");
}
