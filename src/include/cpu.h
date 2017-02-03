#ifndef BEAVER_CPU_H
#define BEAVER_CPU_H

#include <stdint.h>

#define IA32_FS_BASE (0xC0000100)
#define IA32_GS_BASE (0xC0000101)
#define CPUID_1_EDX_APIC_BIT (1 << 9)

uint64_t rdcr0(void);
void wrcr0(uint64_t);


uint64_t rdcr2(void);
void wrcr2(uint64_t);

uint64_t rdcr3(void);
void wrcr3(uint64_t);

uint64_t rdcr4(void);
void wrcr4(uint64_t);

uint64_t rdmsr(uint64_t number);
void wrmsr(uint64_t number, uint64_t value);

void *fsbase();
void *gsbase();

void cpuid(uint32_t fn_num, uint32_t arg,
        uint32_t *eax,
        uint32_t *ebx,
        uint32_t *ecx,
        uint32_t *edx);

#endif /* BEAVER_CPU_H */
