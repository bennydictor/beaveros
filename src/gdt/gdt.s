.section    .text

.globl      _lgdt
.type       _lgdt,  @function

_lgdt:
    lgdt    (%rdi)
    mov     $0x28,  %ax
    ltr     %ax

    mov     $0x10,  %ax
    mov     %ax,    %ds
    mov     %ax,    %ss
    mov     %ax,    %es
    mov     %ax,    %fs
    mov     %ax,    %gs

    pop     %rax
    push    $0x08
    push    %rax
    lretq
