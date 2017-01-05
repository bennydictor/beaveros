#ifndef BEAVER_PAGING_H
#define BEAVER_PAGING_H

#include <stdint.h>

typedef struct {
    uint64_t p : 1;
    uint64_t rw : 1;
    uint64_t us : 1;
    uint64_t pwt : 1;
    uint64_t pcd : 1;
    uint64_t a : 1;
    uint64_t d : 1;
    uint64_t pat : 1;
    uint64_t g : 1;
    uint64_t avl_low : 3;
    uint64_t addr : 40;
    uint64_t avl_high : 11;
    uint64_t nx : 1;
} page_table_entry_t;

extern page_table_entry_t *pml4;

void extend_used_memory(void *);
void *get_used_memory(void);
void *new_phys_page(void);
void *new_phys_zero_page(void);
void map_page(uint64_t, uint64_t);
void setup_paging(uint64_t);

#endif // BEAVER_PAGING_H
