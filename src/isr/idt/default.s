.extern         _default_c_isr

.altmacro

.macro          GEN_ISR                 n,          to,     err
.if             \to-\n
.isr_\n:
.if 1-\err
    push        $0
.endif
    movl        $\n,                    4(%rsp)
    .irp        r,      rax,rbx,rcx,rdx,rdi,rsi,rbp,r8,r9,r10,r11,r12,r13,r14,r15
    push        %\r
    .endr
    mov         $\n,                    %rax
    mov         %rsp,                   %rdi
    mov         _c_isr_table(,%rax,8),  %rax
    call        *%rax
    .irp        r,      r15,r14,r13,r12,r11,r10,r9,r8,rbp,rsi,rdi,rdx,rcx,rbx,rax
    pop         %\r
    .endr
    add         $8,                     %rsp
    iretq

GEN_ISR         %(\n + 1),              \to,        \err
.endif
.endm

.macro          GEN_ISR_PTR             n,          to
.if             \to-\n
.quad           .isr_\n
GEN_ISR_PTR     %(\n + 1),              \to
.endif
.endm

.macro          GEN_PTR                 n,          to,     val=0
.if             \to-\n
.quad           \val
GEN_PTR         %(\n + 1),              \to,        \val
.endif
.endm

.section        .text

GEN_ISR         0,                      8,          0
GEN_ISR         8,                      9,          1
GEN_ISR         10,                     15,         1
GEN_ISR         16,                     17,         0
GEN_ISR         17,                     18,         1
GEN_ISR         18,                     20,         0
GEN_ISR         30,                     31,         1
GEN_ISR         32,                     64,         0
GEN_ISR         64,                     128,        0
GEN_ISR         128,                    192,        0
GEN_ISR         192,                    256,        0

.section        .data

.globl          _isr_table
_isr_table:
GEN_ISR_PTR     0,                      9
GEN_PTR         9,                      10
GEN_ISR_PTR     10,                     15
GEN_PTR         15,                     16
GEN_ISR_PTR     16,                     20
GEN_PTR         20,                     30
GEN_ISR_PTR     30,                     31
GEN_PTR         31,                     32
GEN_ISR_PTR     32,                     64
GEN_ISR_PTR     64,                     128
GEN_ISR_PTR     128,                    192
GEN_ISR_PTR     192,                    256

.globl          _c_isr_table
_c_isr_table:
GEN_PTR         0,                      64,         _default_c_isr
GEN_PTR         64,                     128,        _default_c_isr
GEN_PTR         128,                    192,        _default_c_isr
GEN_PTR         192,                    256         _default_c_isr
