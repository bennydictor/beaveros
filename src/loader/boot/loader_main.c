#include <vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <string.h>
#include <panic.h>
#include <multiboot2.h>
#include <elf64.h>
#include <cpuid.h>
#include <paging.h>
#include <long_mode.h>

void loader_main(uint32_t eax, uint32_t ebx) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);
    set_std_ocdev(vga_get_ocdev());

    if (eax != 0x36d76289) {
        PANIC("Not loaded by multiboot2 loader");
    }
    if (!check_cpuid()) {
        PANIC("CPU doesn't support cpuid instruction");
    }
    if (!check_long_mode()) {
        PANIC("CPU doesn't support long mode");
    }

    multiboot2_fixed_part_t *multiboot2_header = (void *) ebx;
    extend_used_memory((void *) ebx + multiboot2_header->total_size);

    void *kernel_start = 0, *kernel_end = 0;
    multiboot2_tag_header_t *tag = (void *) multiboot2_header + sizeof(multiboot2_fixed_part_t);
    while (tag->type != MULTIBOOT2_END_TAG) {
        if (tag->type == MULTIBOOT2_MODULE_TAG) {
            multiboot2_module_tag_t *module_tag = (multiboot2_module_tag_t *) tag;
            if (!strcmp("BEAVEROS", (char *) module_tag->string)) {
                kernel_start = (void *) module_tag->mod_start;
                kernel_end = (void *) module_tag->mod_end;
            }
        }
        tag = multiboot2_next_tag(tag);
    }
    if (!kernel_start) {
        PANIC("Kernel not found");
    }

    if ((uint32_t) kernel_start & 0xfff) {
        PANIC("Kernel not aligned");
    }

    extend_used_memory(kernel_end);

    setup_identity_paging((void *) 0x800000); // 8M

    uint64_t kernel_entry;
    if (!load_elf64(kernel_start, &kernel_entry)) {
        PANIC("Can't load kernel");
    }

    if ((uint32_t) get_used_memory() > 0x800000) {
        PANIC("Used memory is greater than 8M");
    } 

    enable_long_mode(kernel_entry);
}
