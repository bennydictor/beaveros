#ifndef BEAVER_SCHED_H
#define BEAVER_SCHED_H

#define TASK_STATE_RUNNING 0
#define TASK_STATE_IN_QUEUE 1
#define TASK_STATE_TERMINATED 2
#define TASK_STATE_FROZEN 3
struct __task;
typedef struct __task task_t;
void terminate_task(task_t *task);
__attribute__((noreturn))
void main_loop();
task_t *start_task(void(*start)(void*), void *context, int ring);
void yield();
#define PLS ((processor_local_state_t*)gsbase())

#endif /* BEAVER_SCHED_H */
