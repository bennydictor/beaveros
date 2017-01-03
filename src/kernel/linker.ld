ENTRY(start)

KERNEL_VMA = 1M; /* TODO Kernel should be loaded at it's virtual address. */

SECTIONS {
    . = KERNEL_VMA;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    }
}
