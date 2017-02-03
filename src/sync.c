#include <queue.h>
#include <sync.h>
#include <sched.h>

struct __mutex {
    queue_t waiting_tasks;
    spinlock_t lock;
    uint64_t locked;
};

void mutex_lock(mutex_t *mutex) {
    while (1) {
        spinlock_lock(&mutex->lock);
        if (mutex->locked) {
            task_t *self = get_current_task();
            self->state = TASK_STATE_FROZEN;
            enqueue(&mutex->waiting_tasks, self);
            spinlock_unlock(&mutex->lock);
            yield();
        } else {
            mutex->locked = 1;
            __sync_synchronize();
            spinlock_unlock(&mutex->lock);
            return;
        }
    }
}

void mutex_unlock(mutex_t *mutex) {
    spinlock_lock(&mutex->lock);
    mutex->locked = 0;
    __sync_synchronize();
    for (;;) {
        task_t *t = dequeue(&mutex->waiting_tasks);
        if (t) {
            t->state = TASK_STATE_IN_QUEUE;
        } else {
            break;
        }
    }
    spinlock_unlock(&mutex->lock);
}
