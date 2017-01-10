ENTRY(start)

SECTIONS {
     . = 0xffffffff80000000; /* -2G */

    .text : ALIGN(0x1000) {
        *(.text)
    }

    .data : ALIGN(0x1000) {
        *(.data)
    }
}
