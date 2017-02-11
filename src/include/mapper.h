#ifndef BEAVER_MAPPER_H
#define BEAVER_MAPPER_H

#include <stdint.h>
#include <terminate.h>

#define PAGE_P_BIT      (1ULL << 0)
#define PAGE_RW_BIT     (1ULL << 1)
#define PAGE_US_BIT     (1ULL << 2)
#define PAGE_PWT_BIT    (1ULL << 3)
#define PAGE_PCD_BIT    (1ULL << 4)
#define PAGE_A_BIT      (1ULL << 5)
#define PAGE_D_BIT      (1ULL << 6)
#define PAGE_PAT_BIT    (1ULL << 7)
#define PAGE_G_BIT      (1ULL << 8)
#define PAGE_AVL_BITS   0x7ff0000000000e00ULL
#define PAGE_ADDR_BITS  0x000ffffffffff000ULL
#define PAGE_NX_BIT     (1ULL << 63)

#define PAGE_F_BIT      (1ULL << 9) /* Free on unmap */

#define PML4_FLAGS_MASK 0xfff0000000000e3fULL
#define PDP_FLAGS_MASK  0xfff0000000000e3fULL
#define PD_FLAGS_MASK   0xfff0000000000e3fULL
#define PT_FLAGS_MASK   0xfff0000000000fffULL

#define PAGE_SIZE_BITS  12
#define PAGE_SIZE       (1 << PAGE_SIZE_BITS)
#define PAGE_TABLE_SIZE 512

typedef uint64_t page_table_entry_t;

#ifndef BEAVER_MAPPER_H_ONLY_PAGING

#include <stddef.h>

#define MAP_ANON        NULL

#define PHYS_WINDOW(TYPE) ((TYPE *) phys_window_addr)
#define PHYS_LOOK(X) ({ \
    phys_window_addr = (void *) 0xffffffffffffe000 + (((uint64_t) X) \
            & ~PAGE_ADDR_BITS); \
    phys_window_pt[510] = \
            (((uint64_t) X) & PAGE_ADDR_BITS) | \
                PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT; \
    phys_window_pt[511] = \
            ((((uint64_t) X) & PAGE_ADDR_BITS) + 0x1000) | \
                PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT; \
    asm volatile ("invlpg (%0)"::"r"(0xffffffffffffe000)); \
    asm volatile ("invlpg (%0)"::"r"(0xfffffffffffff000)); \
})

extern void *phys_window_addr;
extern page_table_entry_t *const phys_window_pt;

void mapper_init(void);
void add_phys_mem(void *phys_mem_start, size_t length);
void map_page(void *virt, void *phys, uint64_t flags);

#endif /* BEAVER_MAPPER_H_ONLY_PAGING */

#endif /* BEAVER_MAPPER_H */
