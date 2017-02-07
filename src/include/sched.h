#ifndef BEAVER_SCHED_H
#define BEAVER_SCHED_H

#define TASK_STATE_RUNNING       0
#define TASK_STATE_IN_QUEUE      1
#define TASK_STATE_TERMINATED    2
#define TASK_STATE_FROZEN        3

#define PLS ((processor_local_state_t *) gsbase())

#include <isr/idt.h>

typedef struct {
    interrupt_frame_t saved_state;
    void *stack;
    void *processor_extended_state;
    uint64_t vtime;
    uint64_t started_at;
    int nice;
    int state;
} task_t;

void terminate_task(task_t *task);
__attribute__((noreturn))
void main_loop(void);
task_t *start_task(void (*start)(void*), void *context, int ring);
void yield(void);
task_t *get_current_task(void);

#endif /* BEAVER_SCHED_H */
