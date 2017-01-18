.altmacro

.macro      GEN,        n
.globl      rdcr\n
.type       rdcr\n,     @function

rdcr\n:
    mov     %cr\n,      %rax
    ret

.globl      wrcr\n
.type       wrcr\n,     @function

wrcr\n:
    mov     %rdi,       %cr\n
    ret
.endm

.section    .text

.irpc       n,0234
GEN         \n
.endr

.globl      cpuid
.type       cpuid,      @function

cpuid:
    push    %rbx
    mov     %rdx,       %r10
    mov     %rcx,       %r11
    mov     %rdi,       %rax
    mov     %rsi,       %rcx
    cpuid
    test    %r10,       %r10
    jz      .ebx
    mov     %eax,       (%r10)
.ebx:
    test    %r11,       %r11
    jz      .ecx
    mov     %ebx,       (%r11)
.ecx:
    test    %r8,        %r8
    jz      .edx
    mov     %ecx,       (%r8)
.edx:
    test    %r9,        %r9
    jz      .ret
    mov     %edx,       (%r9)
.ret:
    pop     %rbx
    ret
