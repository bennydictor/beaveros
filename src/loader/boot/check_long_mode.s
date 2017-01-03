.section .text
.globl check_long_mode
.type check_long_mode, @function

check_long_mode:
    movl $0x80000000, %eax
    cpuid
    cmpl $0x80000001, %eax
    jb .no_long_mode

    movl $0x80000001, %eax
    cpuid
    testl $(1 << 29), %edx
    jz .no_long_mode

    movl $1, %eax
    ret
.no_long_mode:
    movl $0, %eax
    ret
