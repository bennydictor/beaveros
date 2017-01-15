#include <memory/page_allocator.h>
#include <mapper.h>
#include <panic.h>
#include <string.h>
#include <io/printf.h>

static uint64_t first_block;
static uint64_t current_pool_size;

static inline void mount_phys_page(uint64_t addr) {
    map_page(KERNEL_PID, PHYS_LOOK, (void *) addr, PAGE_RW_BIT);
}

static inline void unmount_phys_page(void) {
    unmap_page(KERNEL_PID, PHYS_LOOK);
}

void init_page_allocator() {
    unmount_phys_page();
    first_block = PAGE_BLOCK_NO_BLOCK;
    current_pool_size = 0;
}

void add_page_block(uint64_t block_begin, uint64_t block_size) {
    mount_phys_page(block_begin);
    printf("Breakpoint #1");
    page_block_header_t *header = PHYS_LOOK;
    header->next_block = first_block;
    header->size = block_size;
    first_block = block_begin;
    unmount_phys_page();
}

uint64_t alloc_page(void) {
    if (current_pool_size == 0) {
        PANIC("no free pages");
    }

    uint64_t addr = first_block;
    mount_phys_page(first_block);
    page_block_header_t *header = PHYS_LOOK;
    uint64_t block_size = header->size;
    uint64_t next_block = header->next_block;

    if (block_size > 1) {
        first_block += PAGE_SIZE;
        mount_phys_page(first_block);
        header->size = block_size - 1;
        header->next_block = next_block;
    } else {
        first_block = next_block;
    }
    --current_pool_size;
    unmount_phys_page();
    return addr;
}

uint64_t calloc_page(void) {
    uint64_t addr = alloc_page();
    mount_phys_page(addr);
    memset(PHYS_LOOK, 0, PAGE_SIZE);
    unmount_phys_page();
    return addr;
}

void free_page(uint64_t phys_addr) {
    add_page_block(phys_addr, 1);
}

