#include <vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <assert.h>
#include <multiboot2.h>
#include <elf64.h>
#include <string.h>
#include <stddef.h>
#include <cpuid.h>

static void print_module_tag(multiboot2_module_tag_t *tag) {
    printf("   start: %#.8x\n", tag->mod_start);
    printf("     end: %#.8x\n", tag->mod_end);
    printf("  string: %s\n", tag->string);
}

void loader_main(uint32_t eax, uint32_t ebx) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);

    set_std_ocdev(vga_get_ocdev());

    ASSERT(eax == 0x36d76289);

    multiboot2_fixed_part_t *multiboot2_header = (void *) ebx;
    printf("Boot info size: %u bytes\n", multiboot2_header->total_size);

    uint32_t kernel_start = 0, kernel_end = 0;

    multiboot2_memory_map_t *mmap = NULL;
    multiboot2_basic_memory_info_t *meminfo = NULL;

    multiboot2_tag_header_t *tag = (void *) multiboot2_header + sizeof(multiboot2_fixed_part_t);
    while (tag->type != MULTIBOOT2_END_TAG) {
        printf("TAG type: %2u, size: %2u\n", tag->type, tag->size);
        if (tag->type == MULTIBOOT2_MODULE_TAG) {
            multiboot2_module_tag_t *module_tag = (multiboot2_module_tag_t *) tag;
            print_module_tag(module_tag);
            if (!strcmp("BEAVEROS", (char *) module_tag->string)) {
                kernel_start = module_tag->mod_start;
                kernel_end = module_tag->mod_end;
            }
        } else if (tag->type == MULTIBOOT2_MEMORY_MAP_TAG) {
            mmap = (multiboot2_memory_map_t *) tag;
        } else if (tag->type == MULTIBOOT2_BASIC_MEMORY_TAG) {
            meminfo = (multiboot2_basic_memory_info_t *) tag;
        }
        tag = multiboot2_next_tag(tag);
    }

    if (!kernel_start) {
        PANIC("Kernel not found");
    }

    (void) kernel_end; // TODO something with it

    if (meminfo != NULL) {
        printf("Basic memory info:\nmem_lower: %#.8x; mem_upper: %#.8x\n",
                    meminfo->mem_lower,
                    meminfo->mem_upper);
    }

    if (mmap != NULL) {
        printf("Memory map:\n");
        uint32_t entries = (mmap->size - sizeof(multiboot2_memory_map_t)) / mmap->entry_size;
        for (uint32_t i = 0; i < entries; ++i) {
            multiboot2_memory_map_entry_t *entry = mmap->entries + i;
            printf("base_addr: %#.8llx; length: %#.8llx; type: %d\n",
                        entry->base_addr,
                        entry->length,
                        entry->type);
        }
    }

    ASSERT(check_cpuid());
    ASSERT(check_long_mode());

    printf("kernel_main() done\n");
}
