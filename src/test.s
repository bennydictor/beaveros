.section .text

.globl test_isr
.type test_isr, @function

test_isr:
    lea .str, %rdi
    xor %al, %al
    call printf
    iretq

.section .data

.str:
.ascii "hOI! #1\n\0"
