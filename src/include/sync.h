#ifndef BEAVER_SYNC_H
#define BEAVER_SYNC_H

#include <stdint.h>
#include <io/wait.h>

typedef uint64_t spinlock_t;

__attribute__((always_inline))
inline void spinlock_lock(spinlock_t *lock) {
    while(__sync_lock_test_and_set(lock, 1)) {
        io_wait_longer();
    }
}

__attribute__((always_inline))
inline void spinlock_unlock(spinlock_t *lock) {
    __sync_lock_release(lock);
}
#endif /* BEAVER_SYNC_H */
