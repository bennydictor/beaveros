.section    .text

.globl      io_wait
.type       io_wait,            @function

io_wait:
    nop
    nop
    nop
    ret

.globl      io_wait_longer
.type       io_wait_longer,     @function

io_wait_longer:
    mov     %rbx,               %rdi
    cpuid
    cpuid
    cpuid
    mov     %rdi,               %rbx
    ret
