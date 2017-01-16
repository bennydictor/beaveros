#include <mapper.h>
#include <string.h>
#include <assert.h>

void with_interrupts_disabled(void *, ...);


#define SEQ \
    X(0) \
    X(1) \
    X(2) \
    X(3) \
    X(4) \
    X(5) \
    X(6) \
    X(7) \
    X(8) \
    X(9) \
    X(10) \
    X(11) \
    X(12) \
    X(13) \
    X(14) \
    X(15) \
    X(16) \
    X(17) \
    X(18) \
    X(19) \
    X(20) \

#define X(i) void interrupt_ ## i (void);
SEQ
#undef X
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

static interrupt_descriptor_t *idt_page = 0xffffffffffffd000;

void init_interrupts(void) {
    ASSERT(sizeof(interrupt_descriptor_t) == 16);
    map_page(idt_page, MAP_ANON, PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT);
    memset(idt_page, 0, 256 * sizeof(interrupt_descriptor_t));
    idtr_t *idtr = idt_page + 256;
    idtr->limit = 256 * sizeof(interrupt_descriptor_t) - 1;
    idtr->offset = idt_page;
    asm volatile ("lidt (%0)"::"r" (idtr));

#define X(i) \
    install_interrupt(interrupt_ ## i, i);
    SEQ
#undef X
}

void install_interrupt(uint64_t isr, uint8_t interrupt_no) {
    interrupt_descriptor_t desc = { 0 };
    desc.offset_1 = isr & 0xffff;
    desc.offset_2 = (isr & 0xffff0000) >> 16;
    desc.offset_3 = (isr & 0xffffffff00000000ULL) >> 32;
    desc.seg_sel = 8;
    desc.dpl = 0;
    desc.present = 1;
    desc.type = 0xE;
    idt_page[interrupt_no] = desc;
}

