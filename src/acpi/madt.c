#include <acpi/acpi.h>
#include <stddef.h>
#include <stdint.h>
#include <terminate.h>
#include <io/printf.h>
#include <memory/malloc.h>
#include <memory/mapper.h>
#include <acpi/madt.h>

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
        case PROCESSOR_LAPIC_TYPE: {
                processor_lapic_t *p = madt_iter;
                printf(">>> Processor Local APIC\n"
                       ">>> UID = %d\n"
                       ">>> APIC ID = %d\n"
                       ">>> Flags = %#.8x\n",
                       p->proc_uid, p->apic_id, p->flags);
                break;
            }
        case INTERRUPT_SOURCE_OVERRIDE_TYPE: {
                interrupt_source_override_t *p = madt_iter;
                printf(">>> Interrupt Source Override\n"
                       ">>> Bus = %d\n"
                       ">>> Source = %d\n"
                       ">>> Global System Interrupt = %d\n"
                       ">>> Flags = %#.8x\n",
                       p->bus, p->source,
                       p->global_system_interrupt, p->flags);
                break;
            }
        case LAPIC_NMI_TYPE: {
                lapic_nmi_t *p = madt_iter;
                printf(">>> Local APIC NMI\n"
                       ">>> ACPI Processor UID = %d\n"
                       ">>> Flags = %#.8x\n"
                       ">>> Local APIC LINT# = %d\n",
                       p->proc_uid, p->flags, p->lapic_lint);
                break;
            }
        }

        madt_iter += ptr->lentgh;
    }

    free(madt_ptrs);
}
