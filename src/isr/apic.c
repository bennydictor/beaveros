#include <isr/apic.h>
#include <isr/idt.h>
#include <cpu.h>
#include <stdint.h>
#include <terminate.h>
#include <mapper.h>
#include <io/port.h>

extern void *_apic_page;
static uint32_t *const apic_page = (uint32_t *) &_apic_page;

/* TODO: spurious isr */
static void spurious_isr(interrupt_frame_t frame __attribute__ ((unused))) {
    printf("spurious!!\n");
}

void apic_init(void) {
    /* check if APIC is available */
    uint32_t edx;
    cpuid(1, 0, NULL, NULL, NULL, &edx);
    if (!(edx & CPUID_1_EDX_APIC_BIT)) {
        PANIC("no APIC available");
    }

    /* Disable PIC 8259 */
    for (uint8_t pic_irq = 0; pic_irq < 8; ++pic_irq) {
        uint16_t val = inb(PORT_PIC8259_MASTER_DATA);
        val |= 1 << pic_irq;
        outb(PORT_PIC8259_MASTER_DATA, val);
    }
    for (uint8_t pic_irq = 8; pic_irq < 16; ++pic_irq) {
        uint16_t val = inb(PORT_PIC8259_SLAVE_DATA);
        val |= 1 << pic_irq;
        outb(PORT_PIC8259_SLAVE_DATA, val);
    }

    /* Map APIC's page */
    uint64_t msr = rdmsr(APIC_BASE_MSR);
    uint64_t apic_phys_page = msr & PAGE_ADDR_BITS;
    map_page(apic_page, (void *) apic_phys_page,
            PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT);

    /* Reset APIC */
    wrapic(APIC_DFR_REGISTER, APIC_DFR_CLUSTER);
    uint32_t ldr = rdapic(APIC_LDR_REGISTER);
    ldr &= 0x00ffffff;
    ldr |= 0x01;
    wrapic(APIC_LDR_REGISTER, ldr);
    wrapic(APIC_TMR_LVT_REGISTER, APIC_TMR_LVT_DISABLE);
    wrapic(APIC_PERF_LVT_REGISTER, APIC_PERF_LVT_NMI);
    wrapic(APIC_LINT0_LVT_REGISTER, APIC_LINT0_LVT_DISABLE);
    wrapic(APIC_LINT1_LVT_REGISTER, APIC_LINT1_LVT_DISABLE);
    wrapic(APIC_TPR_REGISTER, 0);

    install_isr(spurious_isr, SPURIOUS_VECTOR);
}

void wrapic(uint16_t number, uint32_t value) {
    apic_page[number / sizeof(uint32_t)] = value;
}

uint32_t rdapic(uint16_t number) {
    return apic_page[number / sizeof(uint32_t)];
}
