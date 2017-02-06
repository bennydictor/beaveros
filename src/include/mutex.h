#ifndef BEAVER_SYNC_H
#define BEAVER_SYNC_H

#include <stdint.h>
#include <io/wait.h>

typedef struct {
    queue_t waiting_tasks;
    spinlock_t lock;
    char locked;
} mutex_t;

#endif /* BEAVER_SYNC_H */
