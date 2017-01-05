.section    .text
.globl      setup_gdt
.type       setup_gdt,      @function

setup_gdt:
    lgdt    gdt64_pointer
    mov     $0x10,          %ax
    mov     %ax,            %ss
    mov     %ax,            %ds
    mov     %ax,            %es

    # TODO long jump to kernel

.section    .rodata
    .align  8
gdt64:
    .quad   0
    # TODO code selector
    # TODO data selector
gdt64_pointer:
    .short  . - gdt64 - 1
    .long   gdt64
