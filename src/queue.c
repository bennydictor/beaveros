#include <queue.h>
#include <malloc.h>

void enqueue(queue_t *q, void *e) {
    queue_item_t *item = malloc(sizeof(queue_item_t));
    item->head = e;
    item->tail = NULL;
    spinlock_lock(&q->lock);
    if (q->head) {
        q->tail->tail = item;
    } else {
        q->head = item;
    }
    q->tail = item;
    spinlock_unlock(&q->lock);
}

void *dequeue(queue_t *q) {
    spinlock_lock(&q->lock);
    queue_item_t *cell = q->head;
    if (!cell) {
        return NULL;
    }
    void *ret = cell->head;
    q->head = q->head->tail;
    spinlock_unlock(&q->lock);
    free(cell);
    return ret;
}
