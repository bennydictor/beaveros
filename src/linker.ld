OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
ENTRY(start)

SECTIONS {
    . = 0xffffff8000000000; /* -512G */

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
    . = ALIGN(0x1000);
    _prog_break_begin = .;

    _phys_windows = 4;

    _special_pages = 0xfffffffffffff000 - 0x1000 * (_phys_windows - 1);
    _phys_window_pages = _special_pages;

    _special_pages -= 0x1000;
    _phys_window_pt = _special_pages;

    _special_pages -= 0x1000;
    _idt_page = _special_pages;

    _special_pages -= 0x1000;
    _gdt_page = _special_pages;

    _special_pages -= 0x1000;
    _apic_page = _special_pages;

    _kernel_stack_top = _special_pages;
    _special_pages -= 0x1000;
    _kernel_stack_bottom = _special_pages;

    _isr_stack_top = _special_pages;
    _special_pages -= 0x1000;
    _isr_stack_bottom = _special_pages;

    _special_pages -= 0x100000;
    _first_mb = _special_pages;

    _prog_break_end = _special_pages;
}
