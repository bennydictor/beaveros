#include <memory/mapper.h>
#include <cpu.h>
#include <terminate.h>
#include <string.h>

#define BITS(X, L, H) ((((uint64_t) (X)) >> (L)) & ((1 << ((H) - (L))) - 1))

typedef struct {
    uint64_t size;
    void *next;
} page_header_t;

extern void *_phys_window_pt;
extern void *_phys_window_pages;
extern void *_phys_window_size;
page_table_entry_t *const phys_window_pt = (void *) &_phys_window_pt;
uint64_t const phys_window_size = (uint64_t) &_phys_window_size;
void *_phys_window;

static page_table_entry_t *pml4;
static page_header_t *first_free_block;

void phys_look(void *phys) {
    uint64_t aligned_phys = PAGE_ADDR_BITS & (uint64_t) phys;
    for (uint64_t i = 0; i < phys_window_size; ++i) {
        phys_window_pt[512 - phys_window_size + i] =
            (uint64_t) (aligned_phys + (i << 12)) | PAGE_P_BIT | PAGE_RW_BIT;
        asm volatile ("invlpg (%0)"::"r"((void *) &_phys_window_pages + (i << 12)));
    }
    _phys_window = (uint64_t) phys - aligned_phys + (void *) &_phys_window_pages;
}

void mapper_init(void) {
    pml4 = (void *) rdcr3();
}

void add_phys_mem(void *phys_mem_start, size_t length) {
    phys_look(phys_mem_start);
    PHYS_WINDOW(page_header_t)->size = length / PAGE_SIZE;
    PHYS_WINDOW(page_header_t)->next = first_free_block;
    first_free_block = phys_mem_start;
}

static void free_page(page_header_t *page) {
    phys_look(page);
    PHYS_WINDOW(page_header_t)->size = 1;
    PHYS_WINDOW(page_header_t)->next = first_free_block;
    first_free_block = page;
}

static void *alloc_page(void) {
    if (first_free_block == NULL) {
        PANIC("no memory availible");
    }
    void *ret = first_free_block;
    phys_look(first_free_block);
    if (PHYS_WINDOW(page_header_t)->size == 1) {
        first_free_block = PHYS_WINDOW(page_header_t)->next;
    } else {
        page_header_t header = *PHYS_WINDOW(page_header_t);
        --header.size;
        first_free_block = (void *) first_free_block + PAGE_SIZE;
        phys_look(first_free_block);
        *PHYS_WINDOW(page_header_t) = header;
    }

    return ret;
}

static void *calloc_page(void) {
    void *ret = alloc_page();
    phys_look(ret);
    memset(PHYS_WINDOW(void), 0, PAGE_SIZE);
    return ret;
}

static void unmap_page(void *virt) {
    phys_look(pml4);
    page_table_entry_t *pml4e = PHYS_WINDOW(page_table_entry_t) +
            BITS(virt, 39, 48);

    if (!(*pml4e & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pdp = (void *) (*pml4e & PAGE_ADDR_BITS);
    phys_look(pdp);
    page_table_entry_t *pdpe = PHYS_WINDOW(page_table_entry_t) +
            BITS(virt, 30, 39);

    if (!(*pdpe & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pd = (void *) (*pdpe & PAGE_ADDR_BITS);
    phys_look(pd);
    page_table_entry_t *pde = PHYS_WINDOW(page_table_entry_t) +
            BITS(virt, 21, 30);

    if (!(*pde & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pt = (void *) (*pde & PAGE_ADDR_BITS);
    phys_look(pt);
    page_table_entry_t *pte = PHYS_WINDOW(page_table_entry_t) +
            BITS(virt, 12, 21);

    if (*pte & PAGE_P_BIT) {
        if (*pte & PAGE_F_BIT) {
            free_page((void *) (*pte & PAGE_ADDR_BITS));
        }
        *pte &= ~PAGE_P_BIT;
    }
}

void map_page(void *virt, void *phys, uint64_t flags) {
    /* check sign extend */
    ASSERT(BITS(virt, 47, 64) == 0 || BITS(virt, 47, 64) == 0x1ffff);
    /* check page is aligned */
    ASSERT(((uint64_t) virt & (PAGE_SIZE - 1)) == 0);
    /* check page is alighed and address is 52-bit */
    ASSERT(((uint64_t) phys & ~PAGE_ADDR_BITS) == 0);

    if (!(flags & PAGE_P_BIT)) {
        unmap_page(virt);
        goto invlpg;
    }

    phys_look(pml4);
    page_table_entry_t *pml4e =
            PHYS_WINDOW(page_table_entry_t) + BITS(virt, 39, 48);
    *pml4e |= flags & PML4_FLAGS_MASK;

    if (!(*pml4e & PAGE_P_BIT)) {
        *pml4e = (uint64_t) calloc_page();
    }
    page_table_entry_t *pdp = (void *) (*pml4e & PAGE_ADDR_BITS);
    phys_look(pdp);
    page_table_entry_t *pdpe =
            PHYS_WINDOW(page_table_entry_t) + BITS(virt, 30, 39);
    *pdpe |= flags & PDP_FLAGS_MASK;

    if (!(*pdpe & PAGE_P_BIT)) {
        *pdpe = (uint64_t) calloc_page();
    }
    page_table_entry_t *pd = (void *) (*pdpe & PAGE_ADDR_BITS);
    phys_look(pd);
    page_table_entry_t *pde =
            PHYS_WINDOW(page_table_entry_t) + BITS(virt, 21, 30);
    *pde |= flags & PD_FLAGS_MASK;

    if (!(*pde & PAGE_P_BIT)) {
        *pde = (uint64_t) calloc_page();
    }
    page_table_entry_t *pt = (void *) (*pde & PAGE_ADDR_BITS);

    phys_look(pt);
    page_table_entry_t *pte =
            PHYS_WINDOW(page_table_entry_t) + BITS(virt, 12, 21);

    if (phys == MAP_ANON) {
        if (*pte & PAGE_P_BIT) {
            phys = (void *) (*pte & PAGE_ADDR_BITS);
        } else {
            phys = alloc_page();
        }
    } else {
        if ((phys != (void *) (*pte & PAGE_ADDR_BITS)) && (*pte & PAGE_F_BIT)) {
            free_page((void *) (*pte & PAGE_ADDR_BITS));
        }
    }
    phys_look(pt);
    *pte = flags & PT_FLAGS_MASK;
    *pte |= (uint64_t) phys;

invlpg:
    asm volatile ("invlpg (%0)"::"r" (virt));
}
