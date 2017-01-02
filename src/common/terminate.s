.section .text
.globl terminate
terminate:
    cli
    hlt
    jmp terminate
