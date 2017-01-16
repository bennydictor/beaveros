#include <multiboot2.h>
#include <stddef.h>

multiboot2_tag_header_t *multiboot2_next_tag(multiboot2_tag_header_t *tag) {
    size_t raw_ptr = (size_t) tag;
    raw_ptr += tag->size;
    raw_ptr += 7;
    raw_ptr &= -8;
    return (multiboot2_tag_header_t *) raw_ptr;
}
