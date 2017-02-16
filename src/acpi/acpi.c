#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <mapper.h>
#include <malloc.h>

extern void *_first_mb;
static char *first_mb = (void *) &_first_mb;

typedef struct {
    char signature[8];
    uint8_t checksum_1;
    char oemid[6];
    uint8_t version;
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t checksum_2;
    char mbz[3];
} __attribute__((packed)) rsdp_descriptor_t;

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    char creator_id[4];
    uint32_t creator_revision;
} __attribute__((packed)) acpi_table_header_t;

static char xsdt;
static uint64_t sdt;

char find_sdt(void) {
    for (int i = 0; i < 0x100000 - 16; i += 16) {
        if (strncmp(first_mb + i, "RSD PTR ", 8) == 0) {
            uint8_t sum = 0;
            for (uint8_t j = 0; j < 20; ++j) {
                sum += first_mb[i + j];
            }
            if (sum == 0) {
                rsdp_descriptor_t *rsdpd = (void *) first_mb + i;
                if (rsdpd->version == 2) {
                    uint8_t sum2 = 0;
                    for (uint8_t j = 0; j < 33; ++j) {
                        sum2 += first_mb[i + j];
                    }
                    if (sum2 == 0) {
                        sdt = rsdpd->xsdt_address;
                        xsdt = 1;
                        return 1;
                    }
                }
                if (rsdpd->version == 0) {
                    sdt = rsdpd->rsdt_address;
                    xsdt = 0;
                    return 1;
                }
            }
        }
    }
    return 0;
}

size_t find_acpi_table(char sign[4], void ***ptrs, size_t *size) {
    PHYS_LOOK(sdt);
    size_t entry_size = 4 << xsdt;
    size_t entries = (PHYS_WINDOW(acpi_table_header_t)->length -
            sizeof(acpi_table_header_t)) / entry_size;
    size_t cur_size = 0;
    for (size_t i = 0; i < entries; ++i) {
        void *addr_ptr = PHYS_WINDOW(void) + sizeof(acpi_table_header_t) +
                entry_size * i;
        uint64_t addr;
        if (entry_size == 4) {
            addr = *(uint32_t *) addr_ptr;
        } else {
            addr = *(uint64_t *) addr_ptr;
        }
        PHYS_LOOK(addr);
        if (strncmp(sign, PHYS_WINDOW(char), 4) == 0) {
            if (*size == 0) {
                *ptrs = malloc(8);
                *size = 1;
            } else if (cur_size == *size) {
                *size *= 2;
                *ptrs = realloc(*ptrs, 8 * *size);
            }
            (*ptrs)[cur_size++] = (void *) addr;
        }
        PHYS_LOOK(sdt);
    }
    return cur_size;
}
