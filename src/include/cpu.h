#ifndef BEAVER_CPU_H
#define BEAVER_CPU_H

#include <stdint.h>

#define CPUID_1_EDX_APIC_BIT (1 << 9)

uint64_t rdcr0(void);
uint64_t wrcr0(uint64_t);

uint64_t rdcr2(void);
uint64_t wrcr2(uint64_t);

uint64_t rdcr3(void);
uint64_t wrcr3(uint64_t);

uint64_t rdcr4(void);
uint64_t wrcr4(uint64_t);

void cpuid(uint32_t fn_num, uint32_t arg,
        uint32_t *eax,
        uint32_t *ebx,
        uint32_t *ecx,
        uint32_t *edx);

#endif /* BEAVER_CPU_H */
