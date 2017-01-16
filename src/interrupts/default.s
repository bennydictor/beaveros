.extern         _default_common_stub

.altmacro

.macro          GEN_ISR             n,          to,     err
.if             \to-\n
.isr_\n:
.if 1-\err
    push        $0
.endif
    movl        $\n,                4(%rsp)
    call        _default_common_stub
.if 1-\err
    add         $8,                 %rsp
.endif
    iretq

GEN_ISR         %(\n + 1),          to,         err
.endif
.endm

.macro          GEN_ISR_PTR         n,          to
.if             \to-\n
.quad           .isr_\n
GEN_ISR_PTR     %(\n + 1),          to
.endif
.endm

.macro          GEN_NULL_PTR        n,          to
.if             \to-\n
.quad           0
GEN_NULL_PTR    %(\n + 1),          to
.endif
.endm

.section        .text

GEN_ISR         0,                  8,          0
GEN_ISR         8,                  9,          1
GEN_ISR         10,                 15,         1
GEN_ISR         16,                 17,         0
GEN_ISR         17,                 18,         1
GEN_ISR         18,                 20,         0
GEN_ISR         30,                 31,         1
GEN_ISR         32,                 64,         0
GEN_ISR         64,                 128,        0
GEN_ISR         128,                192,        0
GEN_ISR         192,                256,        0

.section        .data

.globl          _default_isrs
_default_isrs:
GEN_ISR_PTR     0,                  9
GEN_NULL_PTR    9,                  10
GEN_ISR_PTR     10,                 15
GEN_NULL_PTR    15,                 16
GEN_ISR_PTR     16,                 20
GEN_NULL_PTR    20,                 30
GEN_ISR_PTR     30,                 31
GEN_NULL_PTR    31,                 32
GEN_ISR_PTR     32,                 64
GEN_ISR_PTR     64,                 128
GEN_ISR_PTR     128,                192
GEN_ISR_PTR     192,                256
