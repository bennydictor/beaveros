#include <multiboot2.h>

boot_info_tag_header_t *boot_info_next_tag(boot_info_tag_header_t *tag) {
    uint32_t raw_ptr = (uint32_t)(tag);
    raw_ptr += tag->size;
    raw_ptr += 7;
    raw_ptr &= -8;
    return (boot_info_tag_header_t *)raw_ptr;
}
