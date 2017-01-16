.section .text

.globl test_isr
.type test_isr, @function

test_isr:
    lea .str, %rdi
    xor %al, %al
    call printf
    iretq

.globl panic
.type painc, @function
painc:
    mov %rdi, %rbx
    mov %rsi, %r12
    mov %rdx, %r13
    mov %rcx, %r14
    mov %r8,  %r15
    mov %r9,  %rbp
    lea .panic_str, %rdi
    pop %rsi
    push %rsi
    call printf
    mov %rbx, %rdi
    mov %r12, %rsi
    mov %r13, %rdx
    mov %r14, %rcx
    mov %r15, %r8
    mov %rbp, %r9
    call printf
.Lhlt:
    hlt
    jmp .Lhlt

.section .data

.str:
.ascii "hOI! #1\n\0"
.panic_str:
.ascii "Panic called from %#.16llx\n\0"

