#include <string.h>
#include <malloc.h>
#include <mapper.h>
#include <sched.h>
#include <cpu.h>
#include <queue.h>
#include <isr/apic.h>

#define SAVE_MODE_FXSAVE_LAZY    0
#define SAVE_MODE_XSAVE_EAGER    1
#define SAVE_MODE_XSAVEOPT_EAGER 2
#define BASE_TIME_QUANTUM        500

typedef struct processor_local_state {
    struct processor_local_state *self;
    task_t *current_task;
    task_t *sse_state_owner;
} processor_local_state_t;

static int extended_state_save_mode;
static uint32_t extended_state_size;
static uint64_t enabled_extended_states;

static queue_t task_queue;

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
    enqueue(&task_queue, PLS->current_task);
    /* FIXME: deadlock if all tasks are frozen
     * Stop enqueuing frozen tasks, please */
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
    if (extended_state_save_mode == SAVE_MODE_XSAVE_EAGER) {
        xrstor(PLS->current_task->processor_extended_state, enabled_extended_states);
    }
    wrapic(APIC_TMR_INITCNT_REGISTER,
           BASE_TIME_QUANTUM * (20 - PLS->current_task->nice));
}

void apic_timer_fired_isr(interrupt_frame_t *i) {
    task_switch_isr(i);
    wrapic(APIC_EOI_REGISTER, 0);
}

void terminate_task(task_t *task) {
    task->state = TASK_STATE_TERMINATED;
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
    install_isr(save_extended_state_isr, NM_VECTOR);
    /* TODO: tAIMER_VECTROR and YUILD_VECTOE */
    install_isr(task_switch_isr, 0x42);
    install_isr(apic_timer_fired_isr, 0x43);
    wrapic(APIC_TMR_LVT_REGISTER, 0x43);
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
    enqueue(&task_queue, task);
    task->state = TASK_STATE_IN_QUEUE;
    return task;
}

void yield() {
    asm volatile ("int $0x42");
}
