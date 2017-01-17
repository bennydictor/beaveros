#include <apic.h>
#include <cpuid.h>
#include <stdint.h>
#include <terminate.h>

#define CPUID_FLAG_APIC (1 << 9)

bool check_apic(void) {
    uint32_t registers[4];
    cpuid(1, registers);
    return registers[3] & CPUID_FLAG_APIC;
}

void apic_init(void) {
    if (!check_apic()) {
        PANIC("no APIC available");
    }
}
