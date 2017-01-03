#ifndef BEAVER_MULTIBOOT2_H
#define BEAVER_MULTIBOOT2_H

#include <stdint.h>

typedef struct {
    uint32_t total_size;
    uint32_t reserved;
} __attribute__ ((packed)) multiboot2_fixed_part_t;

typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__ ((packed)) multiboot2_tag_header_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mem_lower;
    uint32_t mem_upper;
} __attribute__ ((packed)) multiboot2_basic_memory_info_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t biosdev;
    uint32_t partition;
    uint32_t sub_partition;
} __attribute__ ((packed)) multiboot2_bios_boot_device_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint8_t string[0];
} __attribute__ ((packed)) multiboot2_boot_cmd_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mod_start;
    uint32_t mod_end;
    uint8_t string[0];
} __attribute__ ((packed)) multiboot2_module_tag_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint16_t num;
    uint16_t entsize;
    uint16_t shndx;
    uint16_t reserved;
    uint8_t section_headers[0];
} __attribute__ ((packed)) multiboot2_elf_symbols_t;

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__ ((packed)) multiboot2_memory_map_entry_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    multiboot2_memory_map_entry_t entries[0];
} __attribute__ ((packed)) multiboot2_memory_map_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint8_t string[0];
} __attribute__ ((packed)) multiboot2_boot_loader_name_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg_16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t cseg_len;
    uint16_t cseg_16_len;
    uint16_t dseg_len;
} __attribute__ ((packed)) multiboot2_apm_table_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    uint8_t vbe_control_info[512];
    uint8_t vbe_mode_info[256];
} __attribute__ ((packed)) multiboot2_vbe_info_t;

typedef struct {
    uint8_t red_value;
    uint8_t green_value;
    uint8_t blue_value;
} __attribute__ ((packed)) multiboot2_color_descriptor_t;

typedef struct {
    uint32_t framebuffer_palette_num_colors;
    multiboot2_color_descriptor_t color_descriptors[0];
} __attribute__ ((packed)) multiboot2_framebuffer_palette_t;

typedef struct {
    uint8_t framebuffer_red_field_position;
    uint8_t framebuffer_red_mask_size;
    uint8_t framebuffer_green_field_position;
    uint8_t framebuffer_green_mask_size;
    uint8_t framebuffer_blue_field_position;
    uint8_t framebuffer_blue_mask_size;
} __attribute__ ((packed)) multiboot2_color_info_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
    uint8_t color_info[0];
} __attribute__ ((packed)) multiboot2_framebuffer_info_t;

multiboot2_tag_header_t *multiboot2_next_tag(multiboot2_tag_header_t *);

#define MULTIBOOT2_END_TAG (0)
#define MULTIBOOT2_CMD_TAG (1)
#define MULTIBOOT2_BOOT_LOADER_NAME_TAG (2)
#define MULTIBOOT2_MODULE_TAG (3)
#define MULTIBOOT2_BASIC_MEMORY_TAG (4)
#define MULTIBOOT2_BIOS_BOOT_DEVICE_TAG (5)
#define MULTIBOOT2_MEMORY_MAP_TAG (6)
#define MULTIBOOT2_VBE_INFO_TAG (7)
#define MULTIBOOT2_FRAMEBUFFER_INFO_TAG (8)
#define MULTIBOOT2_ELF_SYMBOLS_TAG (9)
#define MULTIBOOT2_APM_TABLE_TAG (10)

#endif // BEAVER_MULTIBOOT2_H
