#include <gdt.h>
#include <terminate.h>
#include <string.h>
#include <mapper.h>
#include <malloc.h>

#define IOMAP_SIZE 32

typedef struct {
    uint16_t limit_1;
    uint16_t base_1;
    uint8_t base_2;
    uint8_t accessed : 1;
    uint8_t rw : 1;
    uint8_t dc : 1;
    uint8_t ex : 1;
    uint8_t mbo : 1;
    uint8_t priv : 2;
    uint8_t p : 1;
    uint8_t limit_2 : 4;
    uint8_t avl : 1;
    uint8_t l : 1;
    uint8_t sz : 1;
    uint8_t gr : 1;
    uint8_t base_3;
} __attribute__ ((packed)) segment_descriptor_t;

typedef struct {
    uint16_t limit_1;
    uint16_t base_1;
    uint8_t base_2;
    uint8_t type : 4;
    uint8_t mbz_1 : 1;
    uint8_t dpl : 2;
    uint8_t p : 1;
    uint8_t limit_2 : 4;
    uint8_t avl : 1;
    uint8_t mbz_2 : 2;
    uint8_t gr : 1;
    uint8_t base_3;
    uint32_t base_4;
    uint32_t mbz_3;
} __attribute__ ((packed)) tss_descriptor_t;

typedef struct {
    uint32_t mbz_1;
    struct {
        uint32_t lower;
        uint32_t upper;
    } rsp[3];
    uint32_t mbz_2[2];
    struct {
        uint32_t lower;
        uint32_t upper;
    } ist[7];
    uint32_t mbz_3[2];
    uint16_t mbz_4;
    uint16_t iomap_offset;
} __attribute__ ((packed)) tss_t;

typedef struct {
    uint16_t limit;
    uint64_t offset;
} __attribute__ ((packed)) gdtr_t;

void install_segment_descriptor(uint8_t code, uint8_t priv, segment_descriptor_t **descs) {
    (*descs)->base_1 = 0;
    (*descs)->base_2 = 0;
    (*descs)->base_3 = 0;
    (*descs)->limit_1 = 0xffff;
    (*descs)->limit_2 = 0xf;
    (*descs)->accessed = 0;
    (*descs)->rw = 1;
    (*descs)->dc = 0;
    (*descs)->ex = code;
    (*descs)->mbo = 1;
    (*descs)->priv = priv;
    (*descs)->p = 1;
    (*descs)->avl = 0;
    (*descs)->l = code;
    (*descs)->sz = code ^ 1;
    (*descs)->gr = 1;
    ++*descs;
}

void _lgdt(void *);

void gdt_init(void) {
    segment_descriptor_t *descs = malloc(sizeof(segment_descriptor_t) * 5 + sizeof(tss_descriptor_t));
    void *gdt_base = descs;
    memset(descs, 0, sizeof(segment_descriptor_t));
    ++descs;
    install_segment_descriptor(1, 0, &descs);
    install_segment_descriptor(0, 0, &descs);
    install_segment_descriptor(1, 3, &descs);
    install_segment_descriptor(0, 3, &descs);

    tss_descriptor_t *tss_desc = (void *) descs;
    tss_t *tss = malloc(sizeof(tss_t) + IOMAP_SIZE);

    tss_desc->limit_1 = sizeof(tss_t) & 0xffff;
    tss_desc->limit_2 = (sizeof(tss_t) & 0xf0000) >> 16;
    tss_desc->base_1 = (uint64_t) tss & 0xffff;
    tss_desc->base_2 = ((uint64_t) tss & 0xff0000) >> 16;
    tss_desc->base_3 = ((uint64_t) tss & 0xff000000) >> 24;
    tss_desc->base_4 = ((uint64_t) tss & 0xffffffff00000000) >> 32;
    tss_desc->type = 9;
    tss_desc->mbz_1 = 0;
    tss_desc->mbz_2 = 0;
    tss_desc->mbz_3 = 0;
    tss_desc->dpl = 0;
    tss_desc->p = 1;
    tss_desc->avl = 0;
    tss_desc->gr = 1;

    tss->mbz_1 = 0;
    memset(&tss->mbz_2, 0, sizeof(tss->mbz_2));
    memset(&tss->mbz_3, 0, sizeof(tss->mbz_3));
    tss->mbz_4 = 0;
    for (int i = 0; i < 3; ++i) {
        void *stack = malloc(PAGE_SIZE);
        tss->rsp[i].lower = (uint64_t) stack & 0xffffffff;
        tss->rsp[i].upper = ((uint64_t) stack &
                0xffffffff00000000) >> 32;
    }
    for (int i = 0; i < 7; ++i) {
        void *stack = malloc(PAGE_SIZE);
        tss->ist[i].lower = (uint64_t) stack & 0xffffffff;
        tss->ist[i].upper = ((uint64_t) stack &
                0xffffffff00000000) >> 32;
    }
    tss->iomap_offset = sizeof(tss_t);

    uint64_t *iomap = (void *) (tss + 1);
    memset(iomap, 0, IOMAP_SIZE);

    gdtr_t *gdtr = malloc(sizeof(gdtr_t));
    gdtr->offset = (uint64_t) gdt_base;
    gdtr->limit = (sizeof(segment_descriptor_t) * 5 + sizeof(tss_descriptor_t) - 1);

    _lgdt(gdtr);
}
