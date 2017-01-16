#ifndef BEAVER_INTERRUPTS_H
#define BEAVER_INTERRUPTS_H

#include <stdint.h>

/* TODO: Better definition for isr_t? */
typedef void (*isr_t) (void);

void interrupts_init(void);
void install_isr(isr_t isr, uint8_t vector);

#endif
