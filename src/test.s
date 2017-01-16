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



.globl interrupt_0
.type  interrupt_0, @function
interrupt_0:
    mov $0, %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_1
.type  interrupt_1    , @function
interrupt_1    :
    mov $1    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_2
.type  interrupt_2    , @function
interrupt_2    :
    mov $2    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_3
.type  interrupt_3    , @function
interrupt_3    :
    mov $3    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_4
.type  interrupt_4    , @function
interrupt_4    :
    mov $4    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_5
.type  interrupt_5    , @function
interrupt_5    :
    mov $5    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_6
.type  interrupt_6    , @function
interrupt_6    :
    mov $6    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_7
.type  interrupt_7    , @function
interrupt_7    :
    mov $7    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_8
.type  interrupt_8    , @function
interrupt_8    :
    mov $8    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_9
.type  interrupt_9    , @function
interrupt_9    :
    mov $9    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_10
.type  interrupt_10    , @function
interrupt_10    :
    mov $10    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_11
.type  interrupt_11    , @function
interrupt_11    :
    mov $11    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_12
.type  interrupt_12    , @function
interrupt_12    :
    mov $12    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_13
.type  interrupt_13    , @function
interrupt_13    :
    mov $13    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_14
.type  interrupt_14    , @function
interrupt_14    :
    mov $14    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_15
.type  interrupt_15    , @function
interrupt_15    :
    mov $15    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_16
.type  interrupt_16    , @function
interrupt_16    :
    mov $16    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_17
.type  interrupt_17    , @function
interrupt_17    :
    mov $17    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_18
.type  interrupt_18    , @function
interrupt_18    :
    mov $18    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic
.globl interrupt_19
.type  interrupt_19    , @function
interrupt_19    :
    mov $19    , %rsi
    lea .interrupt_panic, %rdi
    jmp panic



.section .data

.str:
.ascii "hOI! #1\n\0"
.panic_str:
.ascii "Panic called from %#.16llx\n\0"
.interrupt_panic:
.ascii "Interrupt %d happened"

