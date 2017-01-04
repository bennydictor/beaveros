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

    .bss BLOCK(16K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
        *(.rodata)
    }

    end_loader = ALIGN(4K);
}
