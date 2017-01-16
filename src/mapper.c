#include <mapper.h>
#include <registers.h>
#include <assert.h>
#include <panic.h>
#include <string.h>

#define BITS(X, L, H) ((((uint64_t) (X)) >> (L)) & ((1 << ((H) - (L))) - 1))

typedef struct {
    uint64_t size;
    void *next;
} page_header_t;

static page_table_entry_t *pml4;
static page_header_t *first_free_block;

void mapper_init(uint64_t phys_mem, uint64_t used_mem) {
    pml4 = (void *) get_cr(3);
    first_free_block = (void *) used_mem;

    page_header_t *window = LOOK(first_free_block);
    window->size = (phys_mem - used_mem) / PAGE_SIZE;
    window->next = NULL;
}

static void *alloc_page(void) {
    if (first_free_block == NULL) {
        PANIC("no memory availible");
    }
    void *ret = first_free_block;
    page_header_t *window = LOOK(first_free_block);
    if (window->size == 1) {
        first_free_block = window->next;
    } else {
        page_header_t header = *window;
        --header.size;
        first_free_block = (void *) first_free_block + PAGE_SIZE;
        window = LOOK(first_free_block);
        *window = header;
    }

    return ret;
}

static void *calloc_page(void) {
    void *ret = alloc_page();
    memset(LOOK(ret), 0, PAGE_SIZE);
    return ret;
}

static void free_page(page_header_t *page) {
    page_header_t *window = LOOK(page);
    window->size = 1;
    window->next = first_free_block;
    first_free_block = page;
}

static void unmap_page(void *virt) {
    page_table_entry_t *window = LOOK(pml4);
    page_table_entry_t *pml4e = window + BITS(virt, 39, 48);

    if (!(*pml4e & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pdp = (void *) (*pml4e & PAGE_ADDR_BITS);
    window = LOOK(pdp);
    page_table_entry_t *pdpe = pdp + BITS(virt, 30, 39);

    if (!(*pdpe & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pd = (void *) (*pdpe & PAGE_ADDR_BITS);
    window = LOOK(pd);
    page_table_entry_t *pde = pd + BITS(virt, 21, 30);

    if (!(*pde & PAGE_P_BIT)) {
        return;
    }
    page_table_entry_t *pt = (void *) (*pde & PAGE_ADDR_BITS);
    window = LOOK(pt);
    page_table_entry_t *pte = pt + BITS(virt, 12, 21);

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

    page_table_entry_t *window = LOOK(pml4);
    page_table_entry_t *pml4e = window + BITS(virt, 39, 48);
    *pml4e |= flags & PML4_FLAGS_MASK;

    if (!(*pml4e & PAGE_P_BIT)) {
        *pml4e = (uint64_t) calloc_page();
    }
    page_table_entry_t *pdp = (void *) (*pml4e & PAGE_ADDR_BITS);
    window = LOOK(pdp);
    page_table_entry_t *pdpe = window + BITS(virt, 30, 39);
    *pdpe |= flags & PDP_FLAGS_MASK;

    if (!(*pdpe & PAGE_P_BIT)) {
        *pdpe = (uint64_t) calloc_page();
    }
    page_table_entry_t *pd = (void *) (*pdpe & PAGE_ADDR_BITS);
    window = LOOK(pd);
    page_table_entry_t *pde = window + BITS(virt, 21, 30);
    *pde |= flags & PD_FLAGS_MASK;

    if (!(*pde & PAGE_P_BIT)) {
        *pde = (uint64_t) calloc_page();
    }
    page_table_entry_t *pt = (void *) (*pde & PAGE_ADDR_BITS);

    window = LOOK(pt);
    page_table_entry_t *pte = window + BITS(virt, 12, 21);

    if (phys == MAP_ANON) {
        if (*pte & PAGE_P_BIT) {
            phys = (void *) (*pte & PAGE_ADDR_BITS);
        } else {
            phys = alloc_page();
            LOOK(pt);
        }
    } else {
        if ((phys != (void *) (*pte & PAGE_ADDR_BITS)) && (*pte & PAGE_F_BIT)) {
            free_page((void *) (*pte & PAGE_ADDR_BITS));
        }
    }
    *pte = flags & PT_FLAGS_MASK;
    *pte |= (uint64_t) phys;

invlpg:
    asm volatile ("invlpg (%0)"::"r" (virt));
}
