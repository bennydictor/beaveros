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
    mov     %ebx,               %edi
    cpuid
    cpuid
    cpuid
    mov     %edi,               %ebx
    ret
