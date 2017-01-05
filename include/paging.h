#ifndef BEAVER_PAGING_H
#define BEAVER_PAGING_H

#include <stdint.h>

#define PAGE_RW_BIT     (1LL << 1)
#define PAGE_US_BIT     (1LL << 2)
#define PAGE_PWT_BIT    (1LL << 3)
#define PAGE_PCD_BIT    (1LL << 4)
#define PAGE_PAT_BIT    (1LL << 7)
#define PAGE_G_BIT      (1LL << 8)
#define PAGE_NX_BIT     (1LL << 63)

#define PML4_FLAGS      0xfff0000000000e3f
#define PDP_FLAGS       0xfff0000000000e3f
#define PD_FLAGS        0xfff0000000000e3f
#define PT_FLAGS        0xfff0000000000fff

typedef struct {
    uint64_t present : 1;
    uint64_t read_write : 1;
    uint64_t user_supervisor : 1;
    uint64_t page_level_writethrough : 1;
    uint64_t page_level_cache_disable : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t page_size : 1;
    uint64_t global : 1;
    uint64_t available_low : 3;
    uint64_t address : 40;
    uint64_t available_high : 11;
    uint64_t no_execute : 1;
} page_table_entry_t;

extern page_table_entry_t *pml4;

void extend_used_memory(void *);
void *get_used_memory(void);
void *new_phys_page(void);
void *new_phys_zero_page(void);
void map_page(uint64_t, uint64_t, uint64_t);
void setup_identity_paging(uint64_t);

#endif // BEAVER_PAGING_H
