ENTRY(start)

SECTIONS {
    . = 0xffffffff80000000; /* -2G */

    .text : 
    {
        *(.text)
    }
}
