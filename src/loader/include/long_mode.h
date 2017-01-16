#ifndef BEAVER_LONG_MODE_H
#define BEAVER_LONG_MODE_H

#include <stdint.h>

void enable_long_mode(uint64_t kernel_entry,
        uint32_t multiboot, void *used_mem);

#endif /* BEAVER_LONG_MODE_H */
