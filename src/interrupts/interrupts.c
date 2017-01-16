#include <interrupts.h>
#include <mapper.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

typedef struct {
    uint16_t offset_1;
    uint16_t seg_sel;
    uint8_t ist:3;
    uint8_t mbz_1:5;
    uint8_t type:4;
    uint8_t mbz_2:1;
    uint8_t dpl:2;
    uint8_t present:1;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t mbz_3;
} __attribute__ ((packed)) interrupt_descriptor_t;

typedef struct {
    uint16_t limit;
    uint64_t offset;
} __attribute__ ((packed)) idtr_t;

/* TODO: move idt_page and co. to linker */
static interrupt_descriptor_t *idt_page = (void *) 0xffffffffffffd000ULL;

extern isr_t _default_isrs[256];

void interrupts_init(void) {
    ASSERT(sizeof(interrupt_descriptor_t) == 16);
    map_page(idt_page, MAP_ANON, PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT);
    memset(idt_page, 0, 256 * sizeof(interrupt_descriptor_t));
    idtr_t *idtr = (void *) (idt_page + 256);
    idtr->limit = 256 * sizeof(interrupt_descriptor_t) - 1;
    idtr->offset = (uint64_t) idt_page;

    for (int i = 0; i < 256; ++i) {
        install_isr(_default_isrs[i], i);
    }

    asm volatile ("lidt (%0)"::"r" (idtr));
}

void install_isr(isr_t isr, uint8_t interrupt_no) {
    interrupt_descriptor_t desc = { 0 };
    desc.offset_1 = (uint64_t) isr & 0xffff;
    desc.offset_2 = ((uint64_t) isr & 0xffff0000) >> 16;
    desc.offset_3 = ((uint64_t) isr & 0xffffffff00000000ULL) >> 32;
    desc.seg_sel = 8;
    desc.dpl = 0;
    desc.present = 1;
    desc.type = 0xE;
    idt_page[interrupt_no] = desc;
}
