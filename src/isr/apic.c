#include <isr/apic.h>
#include <isr/idt.h>
#include <cpu.h>
#include <stdint.h>
#include <terminate.h>
#include <memory/mapper.h>
#include <io/port.h>

extern void *_apic_page;
static uint32_t *const apic_page = (uint32_t *) &_apic_page;
static char x2apic;

/* TODO: spurious isr */
static void spurious_isr(interrupt_frame_t *frame __attribute__ ((unused))) {
    printf("spurious!!\n");
}

static void wrapic(uint16_t number, uint32_t value) {
    apic_page[number / sizeof(uint32_t)] = value;
}

static uint32_t rdapic(uint16_t number) {
    return apic_page[number / sizeof(uint32_t)];
}

void apic_init(void) {
    /* check if APIC is available */
    uint32_t edx, ecx;
    cpuid(1, 0, NULL, NULL, &ecx, &edx);
    if (!(edx & CPUID_1_EDX_APIC_BIT)) {
        PANIC("no APIC available");
    }
    x2apic = !!(ecx & CPUID_1_ECX_X2APIC_BIT);

    /* Disable PIC 8259 */
    outb(PORT_PIC8259_MASTER_DATA, 0xff);
    outb(PORT_PIC8259_SLAVE_DATA, 0xff);

    /* Map APIC's page */
    uint64_t msr = rdmsr(APIC_BASE_MSR);
    if (x2apic) {
        wrmsr(APIC_BASE_MSR, msr | APIC_X2APIC_MODE_BIT);
    }
    uint64_t apic_phys_page = msr & PAGE_ADDR_BITS;
    map_page(apic_page, (void *) apic_phys_page,
            PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT);

    /* Reset APIC */
    if (x2apic) {
        wrmsr(APIC_TMR_LVT_MSR, APIC_TMR_LVT_DISABLE);
        wrmsr(APIC_PERF_LVT_MSR, APIC_PERF_LVT_NMI);
        wrmsr(APIC_LINT0_LVT_MSR, APIC_LINT0_LVT_DISABLE);
        wrmsr(APIC_LINT1_LVT_MSR, APIC_LINT1_LVT_DISABLE);
        wrmsr(APIC_TPR_MSR, 0);
    } else {
        wrapic(APIC_DFR_REGISTER, APIC_DFR_FLAT);
        uint32_t ldr = rdapic(APIC_LDR_REGISTER);
        ldr &= 0x00ffffff;
        ldr |= 0x01;
        wrapic(APIC_LDR_REGISTER, ldr);
        wrapic(APIC_TMR_LVT_REGISTER, APIC_TMR_LVT_DISABLE);
        wrapic(APIC_PERF_LVT_REGISTER, APIC_PERF_LVT_NMI);
        wrapic(APIC_LINT0_LVT_REGISTER, APIC_LINT0_LVT_DISABLE);
        wrapic(APIC_LINT1_LVT_REGISTER, APIC_LINT1_LVT_DISABLE);
        wrapic(APIC_TPR_REGISTER, 0);
    }
    install_isr(spurious_isr, SPURIOUS_VECTOR);
}

void apic_set_initial_count(uint64_t count) {
    if (x2apic) {
        wrmsr(APIC_TMR_INITCNT_MSR, count);
    } else {
        wrapic(APIC_TMR_INITCNT_REGISTER, count);
    }
}

void apic_configure_timer(uint8_t isr, char periodic, uint8_t divider) {
    if (x2apic) {
        wrapic(APIC_TMR_LVT_MSR, isr | (periodic << 17));
        wrapic(APIC_TMR_DIV_MSR, divider);
    } else {
        wrapic(APIC_TMR_LVT_REGISTER, isr | (periodic << 17));
        wrapic(APIC_TMR_DIV_REGISTER, divider);
    }
}

void apic_eoi(void) {
    if (x2apic) {
        wrmsr(APIC_EOI_MSR, 0);
    } else {
        wrapic(APIC_EOI_REGISTER, 0);
    }
}
