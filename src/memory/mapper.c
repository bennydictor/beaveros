#include <mapper.h>
#include <memory/page_allocator.h>
#include <registers.h>
#include <assert.h>

#define BITS(X, L, H) (((X) >> (L)) & ((1 << ((H) - (L))) - 1))

page_table_entry_t *pml4;
static page_table_entry_t *kernel_pml4;
static uint64_t phys_mem_size;

void mapper_init(uint64_t phys_memory_size) {
    kernel_pml4 = (void *) get_cr(3);
    phys_mem_size = phys_memory_size;
    activate_map(KERNEL_PID);
}

void unmap_page(pid_t pid, void *virt) {
    uint64_t virt_addr = (uint64_t) virt;
    /* process table is not implemented yet */
    ASSERT(pid == KERNEL_PID);
    /* check sign extend */
    ASSERT(BITS(virt_addr, 47, 64) == 0 ||
            BITS(virt_addr, 47, 64) == 0x1ffff);
    /* check page is aligned */
    ASSERT((virt_addr & (PAGE_SIZE - 1)) == 0);

    page_table_entry_t *pml4e = pml4 + BITS(virt_addr, 39, 48);

    if (!(*pml4e & PAGE_P_BIT)) {
        return;
    }

    page_table_entry_t *pdp = (void *) (*pml4e & PAGE_ADDR_BITS);
    page_table_entry_t *pdpe = pdp + BITS(virt_addr, 30, 39);

    if (!(*pdpe & PAGE_P_BIT)) {
        return;
    }

    page_table_entry_t *pd = (void *) (*pdpe & PAGE_ADDR_BITS);
    page_table_entry_t *pde = pd + BITS(virt_addr, 21, 30);

    if (!(*pde & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pt = (void *) (*pde & PAGE_ADDR_BITS);
    page_table_entry_t *pte = pt + BITS(virt_addr, 12, 21);
    *pte &= ~PAGE_P_BIT;
}

void map_page(pid_t pid, void *virt, void *phys, uint64_t flags) {
    uint64_t virt_addr = (uint64_t) virt;
    uint64_t phys_addr = (uint64_t) phys;
    /* process table is not implemented yet */
    ASSERT(pid == KERNEL_PID);
    /* check sign extend */
    ASSERT(BITS(virt_addr, 47, 64) == 0 ||
            BITS(virt_addr, 47, 64) == 0x1ffff);
    /* check page is aligned */
    ASSERT((virt_addr & (PAGE_SIZE - 1)) == 0);
    /* check page is alighed and address is 52-bit */
    ASSERT((phys_addr & ~PAGE_ADDR_BITS) == 0);
    /* check page is available */
    ASSERT(phys_addr < phys_mem_size);

    if (virt != PHYS_LOOK && phys == NULL) {
        phys_addr = alloc_page();
    }

    page_table_entry_t *pml4e = pml4 + BITS(virt_addr, 39, 48);

    if (!(*pml4e & PAGE_P_BIT)) {
        *pml4e = calloc_page();
    }
    page_table_entry_t *pdp = (void *) (*pml4e & PAGE_ADDR_BITS);
    page_table_entry_t *pdpe = pdp + BITS(virt_addr, 30, 39);

    if (!(*pdpe & PAGE_P_BIT)) {
        *pdpe = calloc_page();
    }
    page_table_entry_t *pd = (void *) (*pdpe & PAGE_ADDR_BITS);
    page_table_entry_t *pde = pd + BITS(virt_addr, 21, 30);

    if (!(*pde & PAGE_P_BIT)) {
        *pde = calloc_page();
    }
    page_table_entry_t *pt = (void *) (*pde & PAGE_ADDR_BITS);
    page_table_entry_t *pte = pt + BITS(virt_addr, 12, 21);

    *pte = phys_addr;
    flags |= PAGE_P_BIT;
    *pml4e |= flags & PML4_FLAGS_MASK;
    *pdpe |= flags & PDP_FLAGS_MASK;
    *pde |= flags & PD_FLAGS_MASK;
    *pte |= flags & PT_FLAGS_MASK;
}

void activate_map(pid_t pid) {
    ASSERT(pid == KERNEL_PID); /* process table is not implemented yet */
    if (pid == 0) {
        pml4 = kernel_pml4;
    } else {
        /* look in process table */
    }
    /* load pml4 in CR3 */
}
