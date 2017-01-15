#ifndef BEAVER_MEMORY_PAGE_ALLOCATOR_H
#define BEAVER_MEMORY_PAGE_ALLOCATOR_H

#include <stdint.h>

typedef struct {
    uint64_t size; /* in pages */
    uint64_t next_block; 

} page_block_header_t;

void init_page_allocator(); 
void add_page_block(uint64_t block_begin, uint64_t block_size);
uint64_t alloc_page(void);
uint64_t calloc_page(void);
void free_page(uint64_t phys_addr);

#endif /* BEAVER_MEMORY_PAGE_ALLOCATOR_H */
