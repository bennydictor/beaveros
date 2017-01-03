.section .text
.globl check_cpuid
.type check_cpuid, @function

check_cpuid:
    pushfl
    popl %eax
    movl %eax, %ecx

    xorl $(1 << 21), %eax

    pushl %eax
    popfl

    pushfl
    popl %eax

    pushl %ecx
    popfl

    xorl %ecx, %eax
    ret
