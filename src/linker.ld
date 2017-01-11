ENTRY(start)

SECTIONS {
     . = 0xffffffff80000000; /* -2G */

    .text : ALIGN(0x1000) {
        *(.text)
    }

    .rodata : ALIGN(0x1000) {
        *(.rodata)
        *(.rodata.*)
    }

    .data : ALIGN(0x1000) {
        *(.data)
    }

	.bss : ALIGN(0x1000) {
        *(COMMON)
        *(.bss*)
    }

    .eh_frame : ALIGN(0x1000) {
        *(.eh_frame)
    }

	.comment : ALIGN(0x1000) {
        *(.note*) *(.comment*)
    }
}
