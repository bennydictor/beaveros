#include <io/vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <multiboot2.h>
#include <cpu.h>
#include <terminate.h>
#include <math.h>
#include <mapper.h>
#include <gdt.h>
#include <isr/idt.h>
#include <isr/apic.h>
#include <isr/timer.h>
#include <sched.h>

extern void *_first_mb;
static void *first_mb = &_first_mb;

extern void *_kernel_stack_bottom;
static void *kernel_stack_bottom = &_kernel_stack_bottom;

void early_main(void *multiboot, uint64_t used_mem) {
    vga_init((void *) 0xb8000);
    vga_set_foreground(COLOR_LIGHT_GREEN);
    std_ocdev = vga_ocdev;

    printf("If you see tEMMIE, something has gone terribly wrong\n");
    printf("Used memory = %#.16lx\n", used_mem);

    multiboot2_fixed_part_t *multiboot2_header = (void *) multiboot;

    multiboot2_tag_header_t *tag =
            (void *) multiboot2_header + sizeof(multiboot2_fixed_part_t);

    mapper_init();

    /* TODO: recover unused memory */
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
                printf("%.16lx-%.16lx %.16lx type=%d\n",
                        memory_map->entries[i].base_addr,
                        memory_map->entries[i].base_addr +
                        memory_map->entries[i].length,
                        memory_map->entries[i].length,
                        memory_map->entries[i].type);
                if (memory_map->entries[i].type == 1) {
                    if (memory_map->entries[i].base_addr +
                            memory_map->entries[i].length > used_mem) {
                        uint64_t start = ulmax(memory_map->entries[i].base_addr,
                            used_mem);
                        uint64_t length = memory_map->entries[i].base_addr +
                                memory_map->entries[i].length - start;
                        add_phys_mem((void *) start, length);
                    }
                }
            }
        }
        tag = multiboot2_next_tag(tag);
    }

    map_page(kernel_stack_bottom, MAP_ANON,
            PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT);
}


__attribute__((target("sse")))
void test_task(int c) {
    float a = 0.0;
    for (int i = 0; i < 100; i++) {
        a += 10 * c;
        yield();
    }
    printf("%d:%x\n", c, (int)a);
    while(1) yield();
}

__attribute__ ((noreturn))
int main(uint64_t used_mem) {
    for (uintptr_t pg = 0x0; pg < 0x100000; pg += 0x1000) {
        map_page(first_mb + pg, (void *) pg, PAGE_P_BIT |
                        PAGE_RW_BIT | PAGE_G_BIT);
    }
    for (uintptr_t pg = 0x0; pg < used_mem; pg += 0x1000) {
        map_page((void *) pg, NULL, 0);
    }

    vga_buffer = first_mb + 0xb8000;

    printf("Unmapped identity\n");

    gdt_init();
    isr_init();
    apic_init();
    timer_init();

    start_task(test_task, 1, 0);
    start_task(test_task, 2, 0);
    main_loop();
}
