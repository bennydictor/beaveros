#ifndef BEAVER_CPU_H
#define BEAVER_CPU_H

#include <stdint.h>

#define APIC_BASE_MSR           0x1b
#define APIC_BASE_ENABLE_BIT    (1 << 11)

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

void cpuid(uint32_t fn_num, uint32_t arg,
        uint32_t *eax,
        uint32_t *ebx,
        uint32_t *ecx,
        uint32_t *edx);

#endif /* BEAVER_CPU_H */
