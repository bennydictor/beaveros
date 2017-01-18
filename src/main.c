#include <io/vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <isr/idt.h>
#include <isr/apic.h>
#include <multiboot2.h>
#include <cpu.h>
#include <terminate.h>
#include <math.h>
#include <mapper.h>

__attribute__ ((force_align_arg_pointer))
__attribute__ ((noreturn))
void kernel_main(void *multiboot, uint64_t used_mem) {
    vga_init((void *) 0xb8000);
    vga_set_foreground(COLOR_LIGHT_GREEN);
    std_ocdev = vga_ocdev;

    printf("If you see tEMMIE, something has gone terribly wrong.\n");
    printf("CR0 = %#.16lx\n", rdcr0());
    printf("CR3 = %#.16lx\n", rdcr3());
    printf("CR4 = %#.16lx\n", rdcr4());
    printf("Used memory = %#.16lx\n", used_mem);

    multiboot2_fixed_part_t *multiboot2_header = (void *) multiboot;

    multiboot2_tag_header_t *tag =
            (void *) multiboot2_header + sizeof(multiboot2_fixed_part_t);
    uint64_t phys_mem = 0;
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
                if (memory_map->entries[i].type == 1) {
                    printf("base_addr=%#.16lx length=%#.16lx\n",
                            memory_map->entries[i].base_addr,
                            memory_map->entries[i].length);
                    phys_mem = ullmax(phys_mem,
                            memory_map->entries[i].base_addr +
                            memory_map->entries[i].length);
                }
            }
        }
        tag = multiboot2_next_tag(tag);
    }

    mapper_init();
    add_phys_mem((void *) used_mem, phys_mem - used_mem);

    isr_init();
    apic_init();

    terminate();
}
