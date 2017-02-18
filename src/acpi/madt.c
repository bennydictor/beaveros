#include <acpi/acpi.h>
#include <stddef.h>
#include <stdint.h>
#include <terminate.h>
#include <io/printf.h>
#include <memory/malloc.h>
#include <memory/mapper.h>

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
} __attribute__((packed)) madt_t;

typedef struct {
    uint8_t type;
    uint8_t lentgh;
} __attribute__((packed)) madt_entry_header_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t proc_uid;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__((packed)) processor_lapic_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t ioapic_id;
    uint8_t rsvd;
    uint32_t ioapic_addr;
    uint32_t global_system_interrupt_base;
} __attribute__((packed)) ioapic_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t bus;
    uint8_t source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} __attribute__((packed)) interrupt_source_override_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t flags;
    uint32_t global_system_interrupt;
} __attribute__((packed)) nmi_source_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t proc_uid;
    uint16_t flags;
    uint8_t lapic_lint;
} __attribute__((packed)) lapic_nmi_t;

void find_madt(void) {
    madt_t **madt_ptrs = NULL;
    size_t ptr_cnt = 0;
    find_acpi_table("APIC", (void **) &madt_ptrs, &ptr_cnt);
    if (ptr_cnt != 1) {
        PANIC("Found %zd MADT(s), expected 1", ptr_cnt);
    }
    printf("Found MADT\n");

    madt_t *madt = phys_look(madt_ptrs[0]);

    if (strncmp("APIC", madt->signature, 4)) {
        printf("I am surprised that I am executed\n");
        PANIC("I've found some shit instead of MADT signature");
    }

    void *madt_end = ((void *) madt) + madt->length;
    void *madt_iter = (void *) (madt) + sizeof(madt_t);

    while (madt_iter < madt_end) {
        madt_entry_header_t *ptr = madt_iter;
        printf("Entry type %d, length %d\n", ptr->type, ptr->lentgh);

        switch (ptr->type) {
        case 0: {
            processor_lapic_t *p = madt_iter;
            printf(">>> Processor Local APIC\n"
                   ">>> UID = %d\n"
                   ">>> APIC ID = %d\n"
                   ">>> Flags = %x\n",
                   p->proc_uid, p->apic_id, p->flags);
            break;
            }
        }

        madt_iter += ptr->lentgh;
    }

    free(madt_ptrs);
}
