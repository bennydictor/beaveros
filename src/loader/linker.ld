OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
ENTRY(start)

SECTIONS {
    . = 1M;

	.text : {
        *(.multiboot2_header)
        *(.entry_text)
        *(.text*)
    }

	.data : {
        *(.data*)
    }

	.rodata : {
        *(.rodata*)
    }

	.bss : {
        *(COMMON)
        *(.bss*)
        *(.loader_stack)
    }

    .eh_frame : {
        *(.eh_frame)
    }

	.comment : {
        *(.note*) *(.comment*)
    }

    . = ALIGN(0x1000);
	loader_end = .;
}
