#include <isr/apic.h>
#include <cpuid.h>
#include <stdint.h>
#include <terminate.h>
#include <io/port.h>

#define CPUID_FLAG_APIC (1 << 9)

void apic_init(void) {
    outb(PORT_PIC8259_MASTER_COMMAND, 0xff);           /* Disable PIC 8259 */
    outb(PORT_PIC8259_SLAVE_COMMAND, 0xff);
    uint32_t registers[4];
    cpuid(1, registers);
    if (!(registers[3] & CPUID_FLAG_APIC)) {
        PANIC("no APIC available");
    }
}
