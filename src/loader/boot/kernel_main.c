#include <vga/vga.h>
#include <io/stddev.h>
#include <io/printf.h>
#include <assert.h>
#include <boot_info/tags.h>

void kernel_internal_printf_testing(void) {
    io_printf("Printf testing (specifier is %%+-#08.3{d,o,x} ): \n");
    io_printf("%+-#08.3d%+-#08.3o%+-#08.3x\n", 100, 100, 100);
    io_printf("%*s%*s%*s\n", 8, "Dec", 8, "Oct", 8, "Hex");
    io_printf("I can even print symbols: %c\n", 'q'); 
}

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
    io_printf("Registers values: EAX = %#.8x, EBX = %#.8x\n", eax, ebx);
    ASSERT(eax == 0x36d76289);

    boot_info_fixed_part_t *boot_info_header = (void *)ebx;
    io_printf("Boot info size: %u bytes\n", boot_info_header->total_size);
    
    boot_info_tag_header_t *tag = (void *)(boot_info_header) + sizeof(boot_info_fixed_part_t);
    while (tag->type != BOOT_INFO_END_TAG) {
        io_printf("TAG type: %2u, size: %2u\n", tag->type, tag->size);
        if (tag->type == BOOT_INFO_MODULE_TAG) {
            kernel_internal_print_module_tag((boot_info_module_tag_t *)tag);
        }
        tag = boot_info_next_tag(tag);
    }

    io_printf("kernel_main() done\n");
}
