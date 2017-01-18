#ifndef BEAVER_LONG_MODE_H
#define BEAVER_LONG_MODE_H

#include <stdbool.h>
#include <stdint.h>

bool check_cpuid(void);
bool check_long_mode(void);
void enable_long_mode(uint64_t kernel_entry,
        uint32_t multiboot, void *used_mem);

#endif /* BEAVER_LONG_MODE_H */
