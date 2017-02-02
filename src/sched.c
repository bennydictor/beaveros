#include <isr/idt.h>
#include <string.h>
#include <malloc.h>
#include <mapper.h>
#include <sched.h>


struct __task {
    interrupt_frame_t saved_state;
    void *stack;
    int state;
};

static task_t *current_task;

typedef struct {
    void *head;
    void *tail;
} queue_item_t;

typedef struct {
    queue_item_t *head;
    queue_item_t *tail;
} queue_t;

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

static queue_t task_queue;

void task_switch_isr(interrupt_frame_t *i) {
    current_task->saved_state = *i;
    current_task->state = TASK_STATE_IN_QUEUE;
    enqueue(&task_queue, current_task);
    current_task = dequeue(&task_queue);
    if (!current_task) {
        PANIC("NOTHING TO DO"); /* FIXME */
    }
    current_task->state = TASK_STATE_RUNNING;
    *i = current_task->saved_state;
}

void terminate_task(task_t *task) {
    task->state = TASK_STATE_TERMINATED;
    if (task == current_task) {
        yield();
        __builtin_unreachable();
    }
}

__attribute__((noreturn))
void main_loop() {
    install_isr(task_switch_isr, 0x42);
    task_t *t = start_task(NULL, NULL, 0);
    current_task = t;
    for (;;) yield();
    terminate_task(t);
    __builtin_unreachable();
}


task_t *start_task(void(*start)(void*), void *context, int ring) {
    if (ring) {
        PANIC("Not yet"); /* FIXME */
    }
    task_t *task = malloc(sizeof(task_t));
    memset(task, 0, sizeof(task_t));
    task->stack = malloc(PAGE_SIZE);
    task->saved_state.rsp = (uint64_t) task->stack + PAGE_SIZE;
    task->saved_state.rdi = (uint64_t) context;
    task->saved_state.rip = (uint64_t) start;
    task->saved_state.cs = 0x08;
    task->saved_state.ss = 0x10;
    enqueue(&task_queue, task);

    return task;
}

void yield() {
    asm volatile ("int $0x42");
}
