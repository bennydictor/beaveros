#include <heap.h>
#include <malloc.h>
#include <debug.h>

min_heap_t *make_heap(int (*cmp)(void *, void *)) {
    min_heap_t *h = malloc(sizeof(min_heap_t));
    h->size = 0;
    h->capacity = 1;
    h->data = malloc(sizeof(void *));
    h->cmp = cmp;
    return h;
}

void destroy_heap(min_heap_t *h) {
    free(h->data);
    free(h);
}

void sift_down(min_heap_t *h, size_t item) {
    while (item * 2 < h->size) {
        if (item * 2 + 1 < h->size &&
            h->cmp(h->data[item * 2 + 1], h->data[item * 2]) < 0) {
            if (h->cmp(h->data[item * 2 + 1], h->data[item]) < 0) {
                void *t = h->data[item];
                h->data[item] = h->data[item * 2 + 1];
                h->data[item * 2 + 1] = t;
                item = item * 2 + 1;
            } else {
                break;
            }
        } else {
            if (h->cmp(h->data[item * 2], h->data[item]) < 0) {
                void *t = h->data[item];
                h->data[item] = h->data[item * 2];
                h->data[item * 2] = t;
                item *= 2;
            } else {
                break;
            }
        }
    }
}


void sift_up(min_heap_t *h, size_t item) {
    while (h->cmp(h->data[item], h->data[item / 2]) < 0) {
        void *t = h->data[item];
        h->data[item] = h->data[item / 2];
        h->data[item / 2] = t;
        item /= 2;
    }
}

void *heap_pop(min_heap_t *h) {
    spinlock_lock(&h->lock);
    void *ret = h->data[0];
    h->data[0] = h->data[--(h->size)];
    sift_down(h, 0);
    spinlock_unlock(&h->lock);
    return ret;
}

void heap_push(min_heap_t *h, void *item) {
    spinlock_lock(&h->lock);
    if (h->size + 1 > h->capacity) {
        h->capacity *= 2;
        h->data = realloc(h->data,
                          h->capacity * sizeof(void *));
    }
    h->data[h->size] = item;
    sift_up(h, (h->size)++);
    spinlock_unlock(&h->lock);
}
