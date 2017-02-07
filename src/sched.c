#include <string.h>
#include <malloc.h>
#include <mapper.h>
#include <sched.h>
#include <cpu.h>
#include <heap.h>
#include <isr/apic.h>

#define SAVE_MODE_FXSAVE_LAZY    0
#define SAVE_MODE_XSAVE_EAGER    1
#define SAVE_MODE_XSAVEOPT_EAGER 2
#define BASE_TIME_QUANTUM        5000
#define APIC_TIMER_VECTOR        0x43
#define YIELD_VECTOR             0x42
#define TO_STRING_(x)            #x
#define TO_STRING(x)             TO_STRING_(x)


typedef struct processor_local_state {
    struct processor_local_state *self;
    task_t *current_task;
    task_t *sse_state_owner;
    int must_yield;
} processor_local_state_t;

static int extended_state_save_mode;
static uint32_t extended_state_size;
static uint64_t enabled_extended_states;

static int pls_availible;
static spinlock_t queue_lock;
static min_heap_t* task_queue;
static uint64_t total_vtime;
static uint64_t num_tasks;

task_t *get_current_task() {
    return PLS->current_task;
}

void task_switch_isr(interrupt_frame_t *i) {
    if (extended_state_save_mode == SAVE_MODE_FXSAVE_LAZY) {
        wrcr0(rdcr0() | CR0_TASK_SWITCHED);
    } else if (extended_state_save_mode == SAVE_MODE_XSAVE_EAGER) {
        xsave(PLS->current_task->processor_extended_state, enabled_extended_states);
    }
    PLS->current_task->saved_state = *i;
    if (PLS->current_task->state == TASK_STATE_RUNNING) {
        PLS->current_task->state = TASK_STATE_IN_QUEUE;
    }
    uint64_t vtime_delta = rdtsc() - PLS->current_task->started_at;
    PLS->current_task->vtime += vtime_delta;
    __sync_add_and_fetch(&total_vtime, vtime_delta);
    if (queue_lock == (uint64_t)PLS->self) {
        PLS->must_yield = 1;
        goto out;
    }
    spinlock_lock(&queue_lock, 1);
    heap_push(task_queue, PLS->current_task);
    for (;;) {
        PLS->current_task = heap_pop(task_queue);
        if (!PLS->current_task) {
            PANIC("NOTHING TO DO"); /* FIXME */
        }
        if (PLS->current_task->state == TASK_STATE_IN_QUEUE) {
            break;
        }
    }
    spinlock_unlock(&queue_lock);
    PLS->current_task->state = TASK_STATE_RUNNING;
    *i = PLS->current_task->saved_state;
    if (extended_state_save_mode == SAVE_MODE_XSAVE_EAGER) {
        xrstor(PLS->current_task->processor_extended_state, enabled_extended_states);
    }
    PLS->current_task->started_at = rdtsc();
 out:
    wrapic(APIC_TMR_INITCNT_REGISTER,
           BASE_TIME_QUANTUM * (20 - PLS->current_task->nice));
}

void apic_timer_fired_isr(interrupt_frame_t *i) {
    task_switch_isr(i);
    wrapic(APIC_EOI_REGISTER, 0);
}

void terminate_task(task_t *task) {
    task->state = TASK_STATE_TERMINATED;
    __sync_sub_and_fetch(&total_vtime, task->vtime);
    __sync_sub_and_fetch(&num_tasks, 1);
    if (task == PLS->current_task) {
        yield();
        __builtin_unreachable();
    }
}

void save_extended_state_isr(interrupt_frame_t *i __attribute__((unused))) {
    if (extended_state_save_mode == SAVE_MODE_FXSAVE_LAZY) {
        wrcr0(rdcr0() & ~CR0_TASK_SWITCHED);
        if (PLS->sse_state_owner->state != TASK_STATE_TERMINATED) {
            fxsave(PLS->sse_state_owner->processor_extended_state);
        }
        PLS->sse_state_owner = PLS->current_task;
        fxrstor(PLS->current_task->processor_extended_state);
    }
}

int compare_tasks(void *a, void *b) {
    return ((task_t *) a)->vtime - ((task_t *) b)->vtime;
}

__attribute__((noreturn))
void main_loop() {
    wrcr0(rdcr0() | CR0_MONITOR_COPROCESSOR);
    wrcr4(rdcr4() | CR4_OSFXSR | CR4_OSXMMEXCPT);
    uint32_t ecx;
    cpuid(1, 0, NULL, NULL, &ecx, NULL);
    if (ecx & CPUID_1_ECX_XSAVE_BIT) {
        wrcr4(rdcr4() | CR4_OSXSAVE);
        uint32_t eax, edx;
        cpuid(0x0d, 1, &eax, NULL, NULL, NULL);
        if (eax & CPUID_0D_1_EAX_XSAVEOPT_BIT) {
            extended_state_save_mode = SAVE_MODE_XSAVEOPT_EAGER;
        } else {
            extended_state_save_mode = SAVE_MODE_XSAVE_EAGER;
        }
        cpuid(0x0d, 0, &eax, NULL, NULL, &edx);
        enabled_extended_states = (((uint64_t)edx) << 32) | eax;
        xsetbv(0, enabled_extended_states);
        cpuid(0x0d, 0, NULL, &extended_state_size, NULL, NULL);
    } else {
        extended_state_save_mode = SAVE_MODE_FXSAVE_LAZY;
        extended_state_size = 512;
    }
    processor_local_state_t *pls = calloc(sizeof(processor_local_state_t), 1);
    pls->self = pls;
    wrmsr(IA32_GS_BASE, (uint64_t)pls);
    pls_availible = 1;
    install_isr(save_extended_state_isr, NM_VECTOR);
    install_isr(task_switch_isr, YIELD_VECTOR);
    install_isr(apic_timer_fired_isr, APIC_TIMER_VECTOR);
    wrapic(APIC_TMR_LVT_REGISTER, APIC_TIMER_VECTOR);
    wrapic(APIC_TMR_DIV_REGISTER, APIC_TMR_DIV_DIV16);
    task_t *t = start_task(NULL, NULL, 0);
    PLS->current_task = t;
    PLS->sse_state_owner = t;
    terminate_task(t);
    __builtin_unreachable();
}

task_t *start_task(void(*start)(void*), void *context, int ring) {
    if (ring) {
        PANIC("Not yet"); /* FIXME */
    }
    task_t *task = calloc(sizeof(task_t), 1);
    task->processor_extended_state = memalign(64, extended_state_size);
    memset(task->processor_extended_state, 0, extended_state_size);
    task->stack = malloc(PAGE_SIZE);
    task->saved_state.rsp = (uint64_t) task->stack + PAGE_SIZE;
    task->saved_state.rdi = (uint64_t) context;
    task->saved_state.rip = (uint64_t) start;
    task->saved_state.cs = 0x08;
    task->saved_state.ss = 0x10;
    task->saved_state.rflags = RFLAGS_INTERRUPT_FLAG;
    task->state = TASK_STATE_IN_QUEUE;
    task->vtime = num_tasks ? total_vtime / num_tasks : 0;
    __sync_add_and_fetch(&num_tasks, 1);
    if (!task_queue) {
        task_queue = make_heap(compare_tasks);
    }
    if (pls_availible) {
        spinlock_lock(&queue_lock, (uint64_t)PLS->self);
        heap_push(task_queue, task);
        spinlock_unlock(&queue_lock);
        if (PLS->must_yield) {
            PLS->must_yield = 0;
            yield();
        }
    } else {
        heap_push(task_queue, task);
    }
    return task;
}

void yield() {
    asm volatile ("int $" TO_STRING(YIELD_VECTOR));
}
