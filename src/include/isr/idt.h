#ifndef BEAVER_ISR_IDT_H
#define BEAVER_ISR_IDT_H

#include <stdint.h>

#define DE_VECTOR   0
#define DB_VECTOR   1
#define NMI_VECTOR  2
#define BP_VECTOR   3
#define OF_VECTOR   4
#define BR_VECTOR   5
#define UD_VECTOR   6
#define NM_VECTOR   7
#define DF_VECTOR   8
#define TS_VECTOR   10
#define NP_VECTOR   11
#define SS_VECTOR   12
#define GP_VECTOR   13
#define PF_VECTOR   14
#define MF_VECTOR   16
#define AC_VECTOR   17
#define MC_VECTOR   18
#define XF_VECTOR   19
#define SX_VECTOR   30

typedef struct {
    uint32_t error;
    uint32_t vector;
    uint64_t rip;
    uint16_t cs;
    uint16_t ing_1;
    uint32_t ign_2;
    uint64_t rflags;
    uint64_t rsp;
    uint16_t ss;
    uint16_t ign_3;
    uint32_t ign_4;
} __attribute__ ((packed)) interrupt_frame_t;

typedef void (*isr_t) (interrupt_frame_t);

void isr_init(void);
void install_isr(isr_t isr, uint8_t vector);

#endif /* BEAVER_ISR_IDT_H */
