.section    .text
.globl      check_cpuid
.type       check_cpuid,    @function

check_cpuid:
    pushf
    pop     %eax
    mov     %eax,           %ecx
    xorl    $(1 << 21),     %eax
    push    %eax
    popf
    pushf
    popl    %eax
    push    %ecx
    popf
    cmp     %ecx,           %eax
    mov     $0,             %eax
    sete    %al
    xor     $1,             %eax
    ret
