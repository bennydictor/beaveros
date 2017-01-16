.altmacro

.macro      GEN_ISR             n,          to
.if         \to-\n
.isr_\n:
    mov     $\n,                %rdi
    jmp     .common_stub
GEN_ISR     %(\n + 1),          to
.endif
.endm

.macro      GEN_ISR_PTR         n,          to
.if         \to-\n
.quad       .isr_\n
GEN_ISR_PTR %(\n + 1),          to
.endif
.endm

.section    .text

GEN_ISR     0,                  64
GEN_ISR     64,                 128
GEN_ISR     128,                192
GEN_ISR     192,                256

.common_stub:
    mov     %rdi,               %rsi
    lea     .printf_interrupt,  %rdi
    call    printf
    iretq

.section    .data

.globl      _default_isrs
_default_isrs:
GEN_ISR_PTR 0,                  64
GEN_ISR_PTR 64,                 128
GEN_ISR_PTR 128,                192
GEN_ISR_PTR 192,                256

.printf_interrupt:
    .ascii  "hOI!!!!!!\nI'm dEFAULT ISR #%#.2x!!\n\0"

