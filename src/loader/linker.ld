ENTRY(start)

SECTIONS {
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        /* ensure that the multiboot header is at the beginning */
        /* and text is just after header */
        *(.multiboot_header)
        *(.text)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(16K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
