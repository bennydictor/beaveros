#ifndef BEAVER_ISR_APIC_H
#define BEAVER_ISR_APIC_H

#include <stdint.h>

#define APIC_BASE_MSR               0x1b
#define APIC_BASE_ENABLE_BIT        (1 << 11)
#define APIC_X2APIC_MODE_BIT        (1 << 10)

#define TMR_VECTOR                  0x20
#define SPURIOUS_VECTOR             0x27

#define APIC_TPR_REGISTER           0x080
#define APIC_TPR_MSR                0x808

#define APIC_EOI_REGISTER           0x0b0
#define APIC_EOI_MSR                0x80b

#define APIC_LDR_REGISTER           0x0d0

#define APIC_DFR_REGISTER           0x0e0
#define APIC_DFR_FLAT               0xffffffff

#define APIC_SPURIOUS_LVT_REGISTER  0x0f0
#define APIC_SPURIOUS_LVT_ENABLE    (1 << 8)

#define APIC_TMR_LVT_REGISTER       0x320
#define APIC_TMR_LVT_MSR            0x832
#define APIC_TMR_LVT_DISABLE        (1 << 16)
#define APIC_TMR_LVT_PERIODIC       (1 << 17)
#define APIC_TMR_INITCNT_REGISTER   0x380
#define APIC_TMR_INITCNT_MSR        0x838
#define APIC_TMR_CURCNT_REGISTER    0x390
#define APIC_TMR_DIV_REGISTER       0x3e0
#define APIC_TMR_DIV_MSR            0x83e
#define APIC_TMR_DIV_DIV16          3

#define APIC_PERF_LVT_MSR           0x834
#define APIC_PERF_LVT_REGISTER      0x340
#define APIC_PERF_LVT_NMI           (1 << 10)

#define APIC_LINT0_LVT_MSR          0x835
#define APIC_LINT0_LVT_REGISTER     0x350
#define APIC_LINT0_LVT_DISABLE      (1 << 16)

#define APIC_LINT1_LVT_MSR          0x836
#define APIC_LINT1_LVT_REGISTER     0x360
#define APIC_LINT1_LVT_DISABLE      (1 << 16)

void apic_init(void);
void apic_set_initial_count(uint64_t count);
void apic_eoi(void);
void apic_configure_timer(uint8_t isr, char periodic, uint8_t divider);
#endif /* BEAVER_ISR_APIC_H */
