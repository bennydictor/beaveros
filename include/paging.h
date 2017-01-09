#ifndef BEAVER_PAGING_H
#define BEAVER_PAGING_H

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

#define PML4_FLAGS_MASK 0xfff0000000000e3fULL
#define PDP_FLAGS_MASK  0xfff0000000000e3fULL
#define PD_FLAGS_MASK   0xfff0000000000e3fULL
#define PT_FLAGS_MASK   0xfff0000000000fffULL

#define PAGE_SIZE_BITS  12U // 4K
#define PAGE_SIZE       (1U << PAGE_SIZE_BITS)
#define PAGE_TABLE_SIZE 512U

typedef uint64_t page_table_entry_t;

extern page_table_entry_t *pml4;

void extend_used_memory(void *);
void *get_used_memory(void);
void *new_phys_page(void);
void *new_phys_zero_page(void);
void map_page(uint64_t, uint64_t, uint64_t);
void setup_identity_paging(void *);

#endif // BEAVER_PAGING_H