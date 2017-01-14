.section    .text

#define FLAGS_ID_BIT    (1 << 21)
#define CPUID_LM_BIT    (1 << 29)

.globl      check_cpuid
.type       check_cpuid,        @function

check_cpuid:
    pushfl
    pop     %eax
    mov     %eax,               %ecx

    xor     $FLAGS_ID_BIT,      %eax

    push    %eax
    popfl
    pushfl
    pop     %eax

    push    %ecx
    popfl

    cmp     %ecx,               %eax
    mov     $0,                 %eax
    sete    %al
    xor     $1,                 %eax
    ret

.globl      check_long_mode
.type       check_long_mode,    @function

check_long_mode:
    mov     $0x80000000,        %eax
    cpuid
    cmp     $0x80000001,        %eax
    jb      .no_long_mode

    mov     $0x80000001,        %eax
    cpuid
    test    $CPUID_LM_BIT,      %edx
    jz      .no_long_mode

    mov     $1,                 %eax
    ret
.no_long_mode:
    movl    $0,                 %eax
    ret
