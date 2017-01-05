ENTRY(start)

KERNEL_VMA = 0xFFFFFFFFFFFFFFFF;

SECTIONS {
    . = KERNEL_VMA;

    .text : 
    {
        *(.text)
    }
}
