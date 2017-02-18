#ifndef BEAVER_QUEUE_H
#define BEAVER_QUEUE_H

#include <threading/spinlock.h>

typedef struct {
    void *head;
    void *tail;
} queue_item_t;

typedef struct {
    queue_item_t *head;
    queue_item_t *tail;
} queue_t;

void enqueue(queue_t *q, void *e);
void *dequeue(queue_t *q);

#endif /* BEAVER_QUEUE_H */
