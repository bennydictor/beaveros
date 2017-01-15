#include <io/vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <multiboot2.h>
#include <registers.h>
#include <terminate.h>
#include <panic.h>
#include <mapper.h>
#include <memory/page_allocator.h>
#include <math.h>

static uint64_t find_first_free_page(void) {
    uint64_t addr = 0, i;

    i = 0;
    while (pml4[i] & PAGE_P_BIT) {
        ++i;
    }
    --i;

    addr = i; /* last pml4 entry that has subtables */
    page_table_entry_t *pdp = (void *) (pml4[i] & PAGE_ADDR_BITS);

    i = PAGE_TABLE_SIZE - 1;
    while (!(pdp[i] & PAGE_P_BIT)) {
        --i;
    }
    addr = (addr << 9) | i;
    page_table_entry_t *pd = (void *) (pdp[i] & PAGE_ADDR_BITS);

    i = PAGE_TABLE_SIZE - 1;
    while (!(pd[i] & PAGE_P_BIT)) {
        --i;
    }
    addr = (addr << 9) | i;
    page_table_entry_t *pt = (void *) (pd[i] & PAGE_ADDR_BITS);

    i = 0;
    while (pt[i] & PAGE_P_BIT) {
        ++i;
    }
    addr = (addr << 9) | i;
    return addr << 12;
}

__attribute__ ((force_align_arg_pointer))
__attribute__ ((noreturn))
void kernel_main(void *multiboot) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);
    std_ocdev = vga_ocdev;

    printf("CR0 = %#.16llx\n", get_cr(0));
    printf("CR3 = %#.16llx\n", get_cr(3));
    printf("CR4 = %#.16llx\n", get_cr(4));

    mapper_init(-1); /* It is needed for adding page blocks in page allocator
                        and other stuff; will be re-initialized later */

    uint64_t phys_memory_size = 0;
    uint64_t first_free_page = find_first_free_page();

    init_page_allocator();

    printf("First free page: %#.16llx\n", first_free_page);

    multiboot2_fixed_part_t *multiboot2_header = (void *) multiboot;

    multiboot2_tag_header_t *tag =
            (void *) multiboot2_header + sizeof(multiboot2_fixed_part_t);
    while (tag->type != MULTIBOOT2_END_TAG) {
        if (tag->type == MULTIBOOT2_MEMORY_MAP_TAG) {
            multiboot2_memory_map_t *memory_map =
                    (multiboot2_memory_map_t *) tag;
            if (memory_map->entry_size !=
                    sizeof(multiboot2_memory_map_entry_t)) {
                PANIC("entry_size is incorrect");
            }
            if (memory_map->entry_version != 0) {
                PANIC("entry_version is not 0");
            }
            for (uint32_t i = 0; sizeof(multiboot2_memory_map_t) +
                    i * sizeof(multiboot2_memory_map_entry_t) <
                    memory_map->size; ++i) {
                uint64_t base_addr = memory_map->entries[i].base_addr;
                uint64_t length = memory_map->entries[i].length;
                if (memory_map->entries[i].type == 1) {
                    printf("base_addr=%#.16llx length=%#.16llx\n", base_addr, length);
                    uint64_t block_begin = max_ull(base_addr, first_free_page);
                    block_begin += PAGE_SIZE - 1;
                    block_begin &= -PAGE_SIZE;
                    uint64_t block_end = base_addr + length;
                    block_end &= -PAGE_SIZE;
                    if (block_end > block_begin) {
                        printf("Adding pages from %#.16llx to %#.16llx\n", block_begin, block_end);
                        add_page_block(block_begin, (block_end - block_begin) >> 12);
                        printf("Added pages from %#.16llx to %#.16llx\n", block_begin, block_end);
                    }
                }
                phys_memory_size = max_ull(phys_memory_size, base_addr + length);
            }
        }
        tag = multiboot2_next_tag(tag);
    }

    /* Re-initialize mapper with correct physical memory size */
    mapper_init(phys_memory_size);

    terminate();
}
