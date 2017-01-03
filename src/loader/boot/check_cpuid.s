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
