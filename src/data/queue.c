#include <data/queue.h>
#include <memory/malloc.h>

void enqueue(queue_t *q, void *e) {
    queue_item_t *item = malloc(sizeof(queue_item_t));
    item->head = e;
    item->tail = NULL;
    if (q->head) {
        q->tail->tail = item;
    } else {
        q->head = item;
    }
    q->tail = item;
}

void *dequeue(queue_t *q) {
    queue_item_t *cell = q->head;
    if (!cell) {
        return NULL;
    }
    void *ret = cell->head;
    q->head = q->head->tail;
    free(cell);
    return ret;
}
