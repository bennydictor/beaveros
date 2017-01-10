.section    .text

.globl      get_cr
.type       get_cr,                     @function

get_cr:
    jmp     *getJmpTable(,%rdi,8)
.LgetCr0:
    mov     %cr0,                       %rax
    ret
.LgetCr2:
    mov     %cr2,                       %rax
    ret
.LgetCr3:
    mov     %cr3,                       %rax
    ret
.LgetCr4:
    mov     %cr4,                       %rax
    ret


.globl      set_cr
.type       set_cr,                     @function

set_cr:
    jmp     *setJmpTable(,%rdi,8)
.LsetCr0:
    mov     %rsi,                       %cr0
    ret
.LsetCr2:
    mov     %rsi,                       %cr2
    ret
.LsetCr3:
    mov     %rsi,                       %cr3
    ret
.LsetCr4:
    mov     %rsi,                       %cr4
    ret


.section    .data

getJmpTable:
    .quad   .LgetCr0
    .quad   0
    .quad   .LgetCr2
    .quad   .LgetCr3
    .quad   .LgetCr4

setJmpTable:
    .quad   .LsetCr0
    .quad   0
    .quad   .LsetCr2
    .quad   .LsetCr3
    .quad   .LsetCr4
