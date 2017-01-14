.section    .text

.globl      get_cr
.type       get_cr,                     @function

get_cr:
    jmp     *.get_jmp_table(,%rdi,8)
.get_cr0:
    mov     %cr0,                       %rax
    ret
.get_cr2:
    mov     %cr2,                       %rax
    ret
.get_cr3:
    mov     %cr3,                       %rax
    ret
.get_cr4:
    mov     %cr4,                       %rax
    ret


.globl      set_cr
.type       set_cr,                     @function

set_cr:
    jmp     *.set_jmp_table(,%rdi,8)
.set_cr0:
    mov     %rsi,                       %cr0
    ret
.set_cr2:
    mov     %rsi,                       %cr2
    ret
.set_cr3:
    mov     %rsi,                       %cr3
    ret
.set_cr4:
    mov     %rsi,                       %cr4
    ret


.section    .data

.get_jmp_table:
    .quad   .get_cr0
    .quad   0
    .quad   .get_cr2
    .quad   .get_cr3
    .quad   .get_cr4

.set_jmp_table:
    .quad   .set_cr0
    .quad   0
    .quad   .set_cr2
    .quad   .set_cr3
    .quad   .set_cr4
