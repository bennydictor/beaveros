.section    .text
.globl      start
.type       start,      @function

start:
    cli
    mov     %rax,       0xb8000
.Lhlt:
    hlt
    jmp     .Lhlt
