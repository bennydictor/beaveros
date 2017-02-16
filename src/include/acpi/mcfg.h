#ifndef BEAVER_ACPI_MCFG_H
#define BEAVER_ACPI_MCFG_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t base;
    uint16_t pci_group;
    uint8_t start;
    uint8_t end;
    uint32_t reserved;
} __attribute__((packed)) mcfg_conf_base_addr_t;

size_t find_mcfg(mcfg_conf_base_addr_t **bases, size_t *size);

#endif /* BEAVER_ACPI_MCFG_H */
