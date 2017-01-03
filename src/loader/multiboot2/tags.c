#include <multiboot2.h>

multiboot2_tag_header_t *multiboot2_next_tag(multiboot2_tag_header_t *tag) {
    uint32_t raw_ptr = (uint32_t)(tag);
    raw_ptr += tag->size;
    raw_ptr += 7;
    raw_ptr &= -8;
    return (multiboot2_tag_header_t *)raw_ptr;
}
