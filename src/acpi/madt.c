#include <acpi/acpi.h>
#include <stddef.h>
#include <stdint.h>
#include <terminate.h>
#include <io/printf.h>
#include <memory/malloc.h>

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    uint64_t oem_table_id;
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
    uint32_t local_controller_address;
    uint32_t flags;
} madt_t;

typedef struct {
    uint32_t entry_type;
    uint32_t record_length;
} madt_entry_header_t;

void find_madt(void) {
    madt_t **madt_ptrs = NULL;
    size_t ptr_cnt = 0;
    find_acpi_table("APIC", (void **) &madt_ptrs, &ptr_cnt);
    if (ptr_cnt != 1) {
        PANIC("Found %zd MADT(s), expected 1", ptr_cnt);
    }
    printf("Found MADT\n");
    /* TODO */
    free(madt_ptrs);
}
