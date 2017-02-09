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

.globl      rflags_restore
.type       rflags_restore, @function
rflags_restore:
    push    %rdi
    popfq
    ret

.globl      cli_save
.type       cli_save,   @function
cli_save:
    pushfq
    pop     %rax
    cli
    ret



.globl      rdtsc
.type       rdtsc,      @function
rdtsc:
    rdtsc
    shl     $32,        %rdx
    or      %rdx,       %rax
    ret

.globl      rdmsr
.type       rdmsr,      @function
rdmsr:
    mov     %rdi,       %rcx
    rdmsr
    shl     $32,        %rdx
    or      %rdx,       %rax
    ret

.globl      wrmsr
.type       wrmsr,      @function
wrmsr:
    mov     %rdi,       %rcx
    mov     %rsi,       %rax
    mov     %rsi,       %rdx
    shr     $32,        %rdx
    wrmsr
    ret

.globl      xsetbv
.type       xsetbv,     @function
xsetbv:
    mov     %rdi,       %rcx
    mov     %rsi,       %rax
    mov     %rsi,       %rdx
    shr     $32,        %rdx
    xsetbv
    ret

.globl      xsave
.type       xsave,      @function
xsave:
    mov     %rsi,       %rax
    mov     %rsi,       %rdx
    shr     $32,        %rdx
    xsave   (%rdi)
    ret

.globl      xrstor
.type       xrstor,     @function
xrstor:
    mov     %rsi,       %rax
    mov     %rsi,       %rdx
    shr     $32,        %rdx
    xrstor  (%rdi)
    ret

.globl      fxsave
.type       fxsave,     @function
fxsave:
    fxsave  (%rdi)
    ret

.globl      fxrstor
.type       fxrstor,    @function
fxrstor:
    fxrstor (%rdi)
    ret



.globl      gsbase
.type       gsbase,      @function
gsbase:
    mov     %gs:0x0,     %rax
    ret

.globl      fsbase
.type       fsbase,      @function
fsbase:
    mov     %fs:0x0,     %rax
    ret


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

.globl      get_rsp0p
.type       get_rsp0p,  @function
get_rsp0p:
    push    $0
    push    $0
    sgdt    6(%rsp)
    pop     %rcx
    pop     %rax
    str     %rcx
    add     %rcx,       %rax
    mov     2(%rax),    %ecx
    and     $0xFFFFFF,  %ecx
    xor     %rdx,       %rdx
    mov     7(%rax),    %dl
    shl     $24,        %rdx
    or      %rdx,       %rcx
    mov     8(%rax),    %edx
    shl     $32,        %rdx
    or      %rdx,       %rcx
    lea     4(%rcx),    %rax


    ret
