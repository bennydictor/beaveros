.section    .text
.globl      start
.type       start,  @function

start:
    cli
    mov     stack_top,  %esp
    mov     %esp,       %ebp
    push    %ebx
    push    %eax
    call    loader_main
.Lhlt:
    hlt
    jmp     .Lhlt

.section    .bss
stack_bottom:
    .space  16 * 1024
stack_top:
