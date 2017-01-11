.section    .text
.globl      start
.type       start,      @function

start:
    call    kernel_main
.Lhlt:
    hlt
    jmp     .Lhlt
