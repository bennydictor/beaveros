#ifndef BEAVER_ACPI_H
#define BEAVER_ACPI_H

#include <stddef.h>

char find_sdt(void);
size_t find_acpi_table(char sign[4], void **ptrs, size_t *size);

void find_madt(void);

#endif /* BEAVER_ACPI_H */
