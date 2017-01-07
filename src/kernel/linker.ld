ENTRY(start)

SECTIONS {
    /* . = 0xffffffff80000000; /* -2G */
    . = 0xfff00000;

    .text : 
    {
        *(.text)
    }
}
