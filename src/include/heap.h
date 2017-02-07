#ifndef BEAVER_HEAP_H
#define BEAVER_HEAP_H

#include <stddef.h>
#include <spinlock.h>

typedef struct {
    void **data;
    size_t size;
    size_t capacity;
    int (*cmp)(void *, void *);
} min_heap_t;

min_heap_t *make_heap(int (*cmp)(void *, void *));
void destroy_heap(min_heap_t *h);
void *heap_pop(min_heap_t *h);
void heap_push(min_heap_t *h, void *item);

#endif /* BEAVER_HEAP_H */
