#include <isr/idt.h>
#include <string.h>
#include <malloc.h>
#include <mapper.h>
#include <sched.h>
#include <cpu.h>
#include <queue.h>

struct __task {
    interrupt_frame_t saved_state;
    void *stack;
    int state;
};

typedef struct __processor_local_state {
    struct __processor_local_state *self;
    task_t *current_task;
} processor_local_state_t;

static queue_t task_queue;

void task_switch_isr(interrupt_frame_t *i) {
    PLS->current_task->saved_state = *i;
    if (PLS->current_task->state == TASK_STATE_RUNNING) {
        PLS->current_task->state = TASK_STATE_IN_QUEUE;
    }
    enqueue(&task_queue, PLS->current_task);
    for (;;) {
        PLS->current_task = dequeue(&task_queue);
        if (!PLS->current_task) {
            PANIC("NOTHING TO DO"); /* FIXME */
        }
        if (PLS->current_task->state == TASK_STATE_IN_QUEUE) {
            break;
        }
        if (PLS->current_task->state == TASK_STATE_FROZEN) {
            enqueue(&task_queue, PLS->current_task);
        }
    }
    PLS->current_task->state = TASK_STATE_RUNNING;
    *i = PLS->current_task->saved_state;
}

void terminate_task(task_t *task) {
    task->state = TASK_STATE_TERMINATED;
    if (task == PLS->current_task) {
        yield();
        __builtin_unreachable();
    }
}

__attribute__((noreturn))
void main_loop() {
    processor_local_state_t *pls = calloc(sizeof(processor_local_state_t), 1);
    pls->self = pls;
    wrmsr(IA32_GS_BASE, (uint64_t)pls);
    install_isr(task_switch_isr, 0x42);
    task_t *t = start_task(NULL, NULL, 0);
    PLS->current_task = t;
    terminate_task(t);
    __builtin_unreachable();
}


task_t *start_task(void(*start)(void*), void *context, int ring) {
    if (ring) {
        PANIC("Not yet"); /* FIXME */
    }
    task_t *task = calloc(sizeof(task_t), 1);
    task->stack = malloc(PAGE_SIZE);
    task->saved_state.rsp = (uint64_t) task->stack + PAGE_SIZE;
    task->saved_state.rdi = (uint64_t) context;
    task->saved_state.rip = (uint64_t) start;
    task->saved_state.cs = 0x08;
    task->saved_state.ss = 0x10;
    enqueue(&task_queue, task);
    task->state = TASK_STATE_IN_QUEUE;
    return task;
}

void yield() {
    asm volatile ("int $0x42");
}
