.section    .text
.globl      check_cpuid
.type       check_cpuid,    @function

check_cpuid:
    pushfl
    pop     %eax
    mov     %eax,           %ecx
    xorl    $(1 << 21),     %eax
    push    %eax
    popfl
    pushfl
    popl    %eax
    push    %ecx
    popfl
    cmp     %ecx,           %eax
    mov     $0,             %eax
    sete    %al
    xor     $1,             %eax
    ret

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
