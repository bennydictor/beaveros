.section    .text
.globl      start
.type       start,                  @function

start:
    cli
    mov     $0x2f592f412f4b2f4f,    %rax
    mov     %rax,                   0xb8000
.Lhlt:
    hlt
    jmp     .Lhlt

.globl      get_cr3
.type       get_cr3,                @function

get_crx:
    jmp     *$.Ljmptable(,%rdi,8)
.LjumpTable
    .quad   $.Lcr0
    .quad   0
    .quad   $.Lcr2
    .quad   $.Lcr3
    .quad   $.Lcr4
.Lcr0:
    mov     %cr0,                   %rax
    ret
.Lcr2:
    mov     %cr2,                   %rax
    ret
.Lcr3:
    mov     %cr3,                   %rax
    ret
.Lcr4:
    mov     %cr4,                   %rax
    ret



set_crx:
    jmp     *$.Ljmptable(,%rdi,8)
.LjumpTable
    .quad   $.Lcr0
    .quad   0
    .quad   $.Lcr2
    .quad   $.Lcr3
    .quad   $.Lcr4
.Lcr0:
    mov     %rsi,                   %cr0
    ret
.Lcr2:
    mov     %rsi,                   %cr2
    ret
.Lcr3:
    mov     %rsi,                   %cr3
    ret
.Lcr4:
    mov     %rsi,                   %cr4
    ret

