#ifndef BEAVER_REGISTERS_H
#define BEAVER_REGISTERS_H

#include <stdint.h>

uint64_t rdcr0(void);
uint64_t wrcr0(uint64_t);

uint64_t rdcr2(void);
uint64_t wrcr2(uint64_t);

uint64_t rdcr3(void);
uint64_t wrcr3(uint64_t);

uint64_t rdcr4(void);
uint64_t wrcr4(uint64_t);

#endif /* BEAVER_REGISTERS_H */
