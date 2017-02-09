#ifndef BEAVER_CPU_H
#define BEAVER_CPU_H

#include <stdint.h>

#define IA32_FS_BASE                 (0xC0000100)
#define IA32_GS_BASE                 (0xC0000101)
#define CPUID_1_EDX_APIC_BIT         (1 << 9)
#define CPUID_1_ECX_X2APIC_BIT       (1 << 21)
#define CPUID_1_ECX_XSAVE_BIT        (1 << 26)
#define CPUID_0D_1_EAX_XSAVEOPT_BIT  (1 << 0)
#define CR0_MONITOR_COPROCESSOR      (1 << 1)
#define CR0_TASK_SWITCHED            (1 << 3)
#define CR4_OSFXSR                   (1 << 9)
#define CR4_OSXMMEXCPT               (1 << 10)
#define CR4_OSXSAVE                  (1 << 18)
#define RFLAGS_INTERRUPT_FLAG        (1 << 9)
#define MASK_INTERRUPTS(...)         ({uint64_t _flags = cli_save(); __VA_ARGS__; rflags_restore(_flags);})

void **get_rsp0p(void);

uint64_t rdcr0(void);
void wrcr0(uint64_t);

uint64_t rdcr2(void);
void wrcr2(uint64_t);

uint64_t rdcr3(void);
void wrcr3(uint64_t);

uint64_t rdcr4(void);
void wrcr4(uint64_t);

uint64_t rdtsc(void);

uint64_t rdmsr(uint64_t number);
void wrmsr(uint64_t number, uint64_t value);

void rflags_restore(uint64_t flags);
uint64_t cli_save(void);

void fxsave(void *save_region);
void fxrstor(void *save_region);

void xsetbv(uint32_t number, uint64_t value);
void xsave(void *save_region, uint64_t instruction_mask);
void xrstor(void *save_region, uint64_t instruction_mask);

void *fsbase();
void *gsbase();

void cpuid(uint32_t fn_num, uint32_t arg,
        uint32_t *eax,
        uint32_t *ebx,
        uint32_t *ecx,
        uint32_t *edx);

#endif /* BEAVER_CPU_H */
