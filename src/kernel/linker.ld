ENTRY(start)

SECTIONS {
    . = 0xfff00000; /* -1M */

    .text : 
    {
        *(.text)
    }
}
