.section    .text
.globl      start
.type       start,                          @function

start:
            cli
            movq    $0x2f592f412f4b2f4f,    %rax
.Lhlt:      hlt
            jmp     .Lhlt
