#ifndef BEAVER_ACPI_H
#define BEAVER_ACPI_H

char find_sdt(void);
void find_acpi_table(char sign[4], void **ptrs, size_t *size);
void find_madt(void);

#endif /* BEAVER_ACPI_H */
