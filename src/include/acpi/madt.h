#ifndef BEAVEROS_ACPI_MADT_H
#define BEAVEROS_ACPI_MADT_H

#include <stdint.h>

#define PROCESSOR_LAPIC_TYPE            0
#define IOAPIC_TYPE                     1
#define INTERRUPT_SOURCE_OVERRIDE_TYPE  2
#define NMI_SOURCE_TYPE                 3
#define LAPIC_NMI_TYPE                  4
#define LAPIC_ADDRESS_OVERRIDE_TYPE     5
#define IOSAPIC_TYPE                    6
#define LSAPIC_TYPE                     7
#define PLATFORM_INTERRUPT_SOURCE_TYPE  8
#define PROCESSOR_LX2APIC_TYPE          9
#define LX2APIC_NMI_TYPE                10
#define GICC_TYPE                       11
#define GICD_TYPE                       12
#define GIC_MSI_FRAME_TYPE              13
#define GICR_TYPE                       14
#define GIC_ITS_TYPE                    15

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

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd;
    uint32_t lapic_addr;
} __attribute__((packed)) lapic_address_override_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t iosapic_id;
    uint8_t rsvd;
    uint32_t global_system_interrupt_base;
    uint64_t iosapic_addr;
} __attribute__((packed)) iosapic_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t proc_id;
    uint8_t lsapic_id;
    uint8_t lsapic_eid;
    uint8_t rsvd[3];
    uint32_t flags;
    uint32_t proc_uid_value;
    char proc_uid_str[];
} __attribute__((packed)) lsapic_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t flags;
    uint8_t int_type;
    uint8_t proc_id;
    uint8_t proc_uid;
    uint8_t iosapic_vector;
    uint32_t global_system_interrupt;
    uint32_t platform_interrupt_source_flags;
} __attribute__((packed)) platform_interrupt_source_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd;
    uint32_t x2apic_id;
    uint32_t flags;
    uint32_t proc_uid;
} __attribute__((packed)) processor_lx2apic_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t flags;
    uint32_t proc_uid;
    uint8_t lx2apic_lint;
    uint8_t rsvd[3];
} __attribute__((packed)) lx2apic_nmi_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd_lo;
    uint32_t cpu_interface;
    uint32_t proc_uid;
    uint32_t flags;
    uint32_t parking_protocol_version;
    uint32_t performance_interrupt_gsiv;
    uint64_t parked_addr;
    uint64_t phys_base_addr;
    uint64_t gicv;
    uint64_t gich;
    uint32_t vgic_maint_int;
    uint64_t gicr_base_addr;
    uint64_t mpidr;
    uint8_t proc_power_eff_class;
    uint8_t rsvd_hi[3];
} __attribute__((packed)) gicc_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd_lo;
    uint32_t gic_id;
    uint64_t phys_base_addr;
    uint32_t system_vector_base;
    uint8_t gic_version;
    uint8_t rsvd_hi[3];
} __attribute__((packed)) gicd_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd;
    uint32_t gic_msi_frame_id;
    uint64_t phys_base_addr;
    uint32_t flags;
    uint16_t spi_count;
    uint16_t spi_base;
} __attribute__((packed)) gic_msi_frame_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd;
    uint64_t discovery_range_base_addr;
    uint32_t discovery_range_length;
} __attribute__((packed)) gicr_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint16_t rsvd_lo;
    uint32_t gic_its_id;
    uint64_t phys_base_addr;
    uint32_t rsvd_hi;
} __attribute__((packed)) gic_its_t;

#endif /* BEAVEROS_ACPI_MADT_H */
