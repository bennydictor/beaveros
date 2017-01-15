#ifndef BEAVER_MAPPER_H
#define BEAVER_MAPPER_H

#include <stdint.h>

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

#define PAGE_SIZE       0x1000

#define MAP_ANON        NULL

#define PHYS_WINDOW ((void *) 0xfffffffffffff000ULL)
#define PHYS_WINDOW_PT ((page_table_entry_t *) 0xffffffffffffe000ULL)
#define LOOK(X) ({ \
    ASSERT(((uint64_t) (X) & ~PAGE_ADDR_BITS) == 0); \
    PHYS_WINDOW_PT[511] = (uint64_t) (X) | PAGE_P_BIT | PAGE_RW_BIT; \
    asm volatile ("invlpg 0xfffffffffffff000"); \
    PHYS_WINDOW; \
})

typedef uint64_t page_table_entry_t;
typedef uint16_t pid_t;

void mapper_init(uint64_t phys_mem, uint64_t used_mem);
void map_page(pid_t pid, void *virt, void *phys, uint64_t flags);
void activate_map(pid_t pid);

#endif /* BEAVER_MAPPER_H */
