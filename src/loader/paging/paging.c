#include <paging.h>

extern void *end_loader;
static void *used_memory = &end_loader;

#define PAGE_SIZE_BITS 12
#define PAGE_SIZE (1 << PAGE_SIZE_BITS)

static void *align_address(void *addr, uint32_t align_bits) {
    uint32_t addr_val = (uint32_t) addr;
    --addr_val;
    addr_val >>= align_bits;
    ++addr_val;
    addr_val <<= align_bits;
    return (void *) addr_val;
}

void extend_used_memory(void *new_addr) {
    new_addr = align_address(new_addr, PAGE_SIZE_BITS); 
    if (new_addr > used_memory) {
        used_memory = new_addr;
    }
}

void *new_page(void) {
    void *ret = used_memory;
    used_memory += PAGE_SIZE;
    return ret;
}
