#include <paging.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BITS(X, L, H) (((X) >> (L)) & ((1 << ((H) - (L))) - 1))
#define ALIGN_TO_NEXT_PAGE(X) ((void *) (((((uint32_t) (X)) - 1U) & \
        ~(PAGE_SIZE - 1)) + PAGE_SIZE))

extern void *loader_end;
static void *used_memory = &loader_end;
page_table_entry_t *pml4;

void extend_used_memory(void *new_addr) {
    new_addr = ALIGN_TO_NEXT_PAGE(new_addr);
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
    void *ret = new_phys_page();
    memset(ret, 0, PAGE_SIZE);
    return ret;
}

void map_page(uint64_t virt, uint64_t phys, uint64_t flags) {
    /* check sign extend */
    ASSERT(BITS(virt, 47, 64) == 0 || BITS(virt, 47, 64) == 0x1ffff);
    /* check page is aligned */
    ASSERT((virt & (PAGE_SIZE - 1)) == 0);
    /* check page is alighed and address is 52-bit */
    ASSERT((phys & ~PAGE_ADDR_BITS) == 0);

    if (!pml4) {
        pml4 = new_phys_zero_page();
    }
    page_table_entry_t *pml4e = pml4 + BITS(virt, 39, 48);

    if (!(*pml4e & PAGE_P_BIT)) {
        *pml4e = (uint32_t) new_phys_zero_page();
    }
    page_table_entry_t *pdp = (void *) (uint32_t) (*pml4e & PAGE_ADDR_BITS);
    page_table_entry_t *pdpe = pdp + BITS(virt, 30, 39);

    if (!(*pdpe & PAGE_P_BIT)) {
        *pdpe = (uint32_t) new_phys_zero_page();
    }
    page_table_entry_t *pd = (void *) (uint32_t) (*pdpe & PAGE_ADDR_BITS);
    page_table_entry_t *pde = pd + BITS(virt, 21, 30);

    if (!(*pde & PAGE_P_BIT)) {
        *pde = (uint32_t) new_phys_zero_page();
    }
    page_table_entry_t *pt = (void *) (uint32_t) (*pde & PAGE_ADDR_BITS);
    page_table_entry_t *pte = pt + BITS(virt, 12, 21);

    *pte = phys;
    flags |= PAGE_P_BIT;
    *pml4e |= flags & PML4_FLAGS_MASK;
    *pdpe |= flags & PDP_FLAGS_MASK;
    *pde |= flags & PD_FLAGS_MASK;
    *pte |= flags & PT_FLAGS_MASK;
}

void setup_identity_paging(void *addr) {
    for (uint64_t i = 0; i < (uint64_t) (uint32_t) addr; i += PAGE_SIZE) {
        map_page(i, i, PAGE_RW_BIT | PAGE_G_BIT);
    }
}
