OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
ENTRY(kernel_main)

SECTIONS {
    . = 0xffffffff80000000; /* -2G */

    .text : ALIGN(0x1000) {
        *(.text*)
    }

    .data : ALIGN(0x1000) {
        *(.data*)
    }

    .rodata : ALIGN(0x1000) {
        *(.rodata*)
    }

	.bss : ALIGN(0x1000) {
        *(COMMON)
        *(.bss*)
    }

    .eh_frame : ALIGN(0x1000) {
        *(.eh_frame*)
    }

	.comment : ALIGN(0x1000) {
        *(.note*) *(.comment*)
    }

    _idt_page = 0xffffffffffffd000;
    _phys_window_pt = 0xffffffffffffe000;
    _phys_window_page = 0xfffffffffffff000;
}
