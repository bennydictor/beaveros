#ifndef BEAVER_PAGING_H
#define BEAVER_PAGING_H

#include <stdint.h>
#define BEAVER_MAPPER_H_ONLY_PAGING
#include "../../include/mapper.h"
#undef BEAVER_MAPPER_H_ONLY_PAGING

extern page_table_entry_t *pml4;

void extend_used_memory(void *);
void *get_used_memory(void);
void *new_phys_page(void);
void *new_phys_zero_page(void);
void map_page(uint64_t virt, uint64_t phys, uint64_t flags);
void setup_identity_paging();

#endif /* BEAVER_PAGING_H */
