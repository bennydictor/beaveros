#include <acpi/mcfg.h>
#include <acpi/acpi.h>
#include <terminate.h>
#include <io/printf.h>
#include <memory/mapper.h>
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
    uint64_t reserved;
} __attribute__((packed)) mcfg_t;

size_t find_mcfg(mcfg_conf_base_addr_t **bases, size_t *size) {
    mcfg_t **mcfg_ptrs = NULL;
    size_t ptr_cnt = 0;
    find_acpi_table("MCFG", (void **) &mcfg_ptrs, &ptr_cnt);
    if (ptr_cnt != 1) {
        PANIC("Found %zd MCFG(s), expected 1", ptr_cnt);
    }
    phys_look(*mcfg_ptrs);
    size_t cnt = (PHYS_WINDOW(mcfg_t)->length - sizeof(mcfg_t)) /
            sizeof(mcfg_conf_base_addr_t);
    if (*size == 0) {
        *bases = malloc(cnt);
        *size = cnt;
    } else if (*size < cnt) {
        *bases = realloc(bases, cnt * sizeof(mcfg_conf_base_addr_t));
        *size = cnt;
    }
    mcfg_conf_base_addr_t *addr = (void *) (*mcfg_ptrs + 1);
    for (size_t i = 0; i < cnt; ++i, ++addr) {
        phys_look(addr);
        (*bases)[i] = *PHYS_WINDOW(mcfg_conf_base_addr_t);
        ++addr;
    }
    free(mcfg_ptrs);
    return cnt;
}
