#ifndef BEAVER_CPUID_H
#define BEAVER_CPUID_H

#include <stdint.h>

static inline void cpuid(uint32_t code, uint32_t registers[4]) {
    __asm__ __volatile__("cpuid" : "=a"(registers[0]), "=b"(registers[1]),
            "=c"(registers[2]), "=d"(registers[3]) : "a"(code));
}

#endif /* BEAVER_CPUID_H */
