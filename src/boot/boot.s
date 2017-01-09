.section    .text
.globl      start
.type       start,                          @function

start:      cli
            mov     $0x2f592f412f4b2f4f,    %rax
            mov     %rax,                   0xb8000
.Lhlt:      hlt
            jmp     .Lhlt
