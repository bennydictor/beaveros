#include <paging.h>
#include <string.h>
#include <stdbool.h>

extern void *loader_end;
static void *used_memory = &loader_end;

#define PAGE_SIZE_BITS 12
#define PAGE_SIZE (1 << PAGE_SIZE_BITS)
#define PAGE_TABLE_SIZE (1 << 9)

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

void *get_used_memory(void) {
    return used_memory;
}

page_table_entry_t *create_table(void) {
    page_table_entry_t *table = new_page();
    memset(table, 0, PAGE_SIZE);
    return table;
}

page_table_entry_t *get_next_table(page_table_entry_t *table, uint32_t idx) {
    if (!table[idx].present) {
        uint32_t new_table = (uint32_t) create_table();
        table[idx].present = true;
        table[idx].writable = true;
        table[idx].address = new_table >> 12;
    }
    return (page_table_entry_t *)(table[idx].address << 12);
}

#define ADDR_PART(ADDR, LEVEL) ((ADDR << (12 + 9 * ((LEVEL) - 1))) & (PAGE_TABLE_SIZE - 1))

page_table_entry_t *create_entry(uint64_t virt_addr, page_table_entry_t p4_table[]) {
    page_table_entry_t *p3_table = get_next_table(p4_table, ADDR_PART(virt_addr, 4));
    page_table_entry_t *p2_table = get_next_table(p3_table, ADDR_PART(virt_addr, 3));
    page_table_entry_t *p1_table = get_next_table(p2_table, ADDR_PART(virt_addr, 2));
    return p1_table + ADDR_PART(virt_addr, 1);
}
