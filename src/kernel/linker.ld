ENTRY(start)

KERNEL_VMA = 0xffffffff80000000; /* -2G */

SECTIONS {
    . = KERNEL_VMA;

    .text : 
    {
        *(.text)
    }
}
