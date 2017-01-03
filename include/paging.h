#ifndef BEAVER_PAGING_H
#define BEAVER_PAGING_H

#include <stdint.h>

typedef struct page_table_entry {
    uint64_t present : 1;
    uint64_t writable : 1;
    uint64_t user_accessible : 1;
    uint64_t write_through_caching : 1;
    uint64_t disable_cache : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t huge_page : 1;
    uint64_t global : 1;
    uint64_t low_free : 3;
    uint64_t address : 40;
    uint64_t high_free : 11;
    uint64_t no_execute : 1;
} page_table_entry_t;

#endif // BEAVER_PAGING_H
