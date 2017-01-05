#include <paging.h>
#include <string.h>
#include <stdbool.h>

extern void *loader_end;
static void *used_memory = &loader_end;
page_table_entry_t *pml4;

#define PAGE_SIZE_BITS 12 // 4K
#define PAGE_SIZE (1 << PAGE_SIZE_BITS)
#define PAGE_TABLE_SIZE 512

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

void *get_used_memory(void) {
    return used_memory;
}

void *new_phys_page(void) {
    void *ret = used_memory;
    used_memory += PAGE_SIZE;
    return ret;
}

void *new_phys_zero_page(void) {
    void *ret = used_memory;
    memset(ret, 0, PAGE_SIZE);
    used_memory += PAGE_SIZE;
    return ret;
}

#define BITS(X, L, H) (((X) >> (L)) & ((1 << ((H) - (L))) - 1))

void map_page(uint64_t virt, uint64_t phys, uint64_t flags) {
    flags |= 1;
    if (!pml4) {
        pml4 = new_phys_zero_page();
    }
    uint16_t pml4_off = BITS(virt, 39, 48);
    page_table_entry_t *pml4e = pml4 + pml4_off;
    *(uint64_t *) pml4e |= flags & PML4_FLAGS;

    if (!pml4e->present) {
        pml4e->address = (uint64_t) (uint32_t) new_phys_zero_page() >> PAGE_SIZE_BITS;
    }
    page_table_entry_t *pdp = (void *) (uint32_t) (pml4e->address << PAGE_SIZE_BITS);
    uint16_t pdp_off = BITS(virt, 30, 39);
    page_table_entry_t *pdpe = pdp + pdp_off;
    *(uint64_t *) pdpe |= flags & PDP_FLAGS;

    if (!pdpe->present) {
        pdpe->address = (uint64_t) (uint32_t) new_phys_zero_page() >> PAGE_SIZE_BITS;
        pdpe->present = 1;
        pdpe->read_write = 1;
    }
    page_table_entry_t *pd = (void *) (uint32_t) (pdpe->address << PAGE_SIZE_BITS);
    uint16_t pd_off = BITS(virt, 21, 30);
    page_table_entry_t *pde = pd + pd_off;
    *(uint64_t *) pde |= flags & PD_FLAGS;

    if (!pde->present) {
        pde->address = (uint64_t) (uint32_t) new_phys_zero_page() >> PAGE_SIZE_BITS;
        pde->present = 1;
        pde->read_write = 1;
    }
    page_table_entry_t *pt = (void *) (uint32_t) (pde->address << PAGE_SIZE_BITS);
    uint16_t pt_off = BITS(virt, 12, 21);
    page_table_entry_t *pte = pt + pt_off;
    *(uint64_t *) pte |= flags & PT_FLAGS;

    pte->address = phys >> PAGE_SIZE_BITS;
    pte->present = 1;
    pte->read_write = 1;
}

void setup_identity_paging(uint64_t addr) {
    for (uint64_t i = 0; i < addr; i += PAGE_SIZE) {
        map_page(i, i, PAGE_RW_BIT);
    }
}
