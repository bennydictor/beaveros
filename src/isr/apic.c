#include <isr/apic.h>
#include <cpu.h>
#include <stdint.h>
#include <terminate.h>
#include <io/port.h>

void apic_init(void) {
    /* check if APIC is available */
    uint32_t edx;
    cpuid(1, 0, NULL, NULL, NULL, &edx);
    if (!(edx & CPUID_1_EDX_APIC_BIT)) {
        PANIC("no APIC available");
    }

    uint64_t msr = rdmsr(APIC_BASE_MSR);
    if (!(msr & APIC_BASE_ENABLE_BIT)) {
        PANIC("APIC is not enabled\nReboot and try again");
    }

    /* Disable PIC 8259 */
    outb(PORT_PIC8259_MASTER_COMMAND, 0xff);
    outb(PORT_PIC8259_SLAVE_COMMAND, 0xff);
}
