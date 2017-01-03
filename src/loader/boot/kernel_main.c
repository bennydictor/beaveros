#include <vga/vga.h>
#include <io/stddev.h>
#include <io/printf.h>
#include <assert.h>
#include <boot_info/tags.h>
#include <elf64/elf64.h>
#include <string.h>
#include <stddef.h>
#include <boot/check_cpuid.h>
#include <boot/check_long_mode.h>

/*
void kernel_internal_printf_testing(void) {
    io_printf("Printf testing (specifier is %%+-#08.3{d,o,x} ): \n");
    io_printf("%+-#08.3d%+-#08.3o%+-#08.3x\n", 100, 100, 100);
    io_printf("%*s%*s%*s\n", 8, "Dec", 8, "Oct", 8, "Hex");
    io_printf("I can even print symbols: %c\n", 'q'); 
}
*/

void kernel_internal_print_module_tag(boot_info_module_tag_t *tag) {
    io_printf("   start: %#.8x\n", tag->mod_start);
    io_printf("     end: %#.8x\n", tag->mod_end);
    io_printf("  string: %s\n", tag->string);
}

void kernel_main(uint32_t eax, uint32_t ebx) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);

    io_set_std_ocdev(vga_get_ocdev());

    io_printf("\n\n>>> Hello world!\n");
//    io_printf("Registers values: EAX = %#.8x, EBX = %#.8x\n", eax, ebx);
    ASSERT(eax == 0x36d76289);

    boot_info_fixed_part_t *boot_info_header = (void *)ebx;
    io_printf("Boot info size: %u bytes\n", boot_info_header->total_size);

    uint32_t kernel_start = 0, kernel_end = 0;
    
    boot_info_memory_map_t *mmap = NULL;
    boot_info_basic_memory_info_t *meminfo = NULL;

    boot_info_tag_header_t *tag = (void *)(boot_info_header) + sizeof(boot_info_fixed_part_t);
    while (tag->type != BOOT_INFO_END_TAG) {
//        io_printf("TAG type: %2u, size: %2u\n", tag->type, tag->size);
        if (tag->type == BOOT_INFO_MODULE_TAG) {
            boot_info_module_tag_t *module_tag = (boot_info_module_tag_t *)tag;
//            kernel_internal_print_module_tag(module_tag);
            if (!strcmp("BEAVEROS", (char *)module_tag->string)) {
                kernel_start = module_tag->mod_start;
                kernel_end = module_tag->mod_end;
            }
        } else if (tag->type == BOOT_INFO_MEMORY_MAP_TAG) {
            mmap = (boot_info_memory_map_t *)tag;
        } else if (tag->type == BOOT_INFO_BASIC_MEMORY_TAG) {
            meminfo = (boot_info_basic_memory_info_t *)tag;
        }
        tag = boot_info_next_tag(tag);
    }

    if (!kernel_start) {
        PANIC("Kernel not found");
    }

    // print_elf64((void *)kernel_start);
    
    if (meminfo != NULL) {
        io_printf("Basic memory info:\nmem_lower: %#.8x; mem_upper: %#.8x\n", meminfo->mem_lower, meminfo->mem_upper);

    }
    
    if (mmap != NULL) {
        io_printf("Memory map:\n");
        uint32_t entries = (mmap->size - sizeof(boot_info_memory_map_t)) / mmap->entry_size;
        for (uint32_t i = 0; i < entries; ++i) {
            boot_info_memory_map_entry_t *entry = mmap->entries + i;
            io_printf("base_addr: %#.8llx; length: %#.8llx; type: %d\n", entry->base_addr, entry->length, entry->type);
        }
    }

    ASSERT(check_cpuid());
    ASSERT(check_long_mode());

    io_printf("kernel_main() done\n");
}
