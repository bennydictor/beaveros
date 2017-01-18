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
