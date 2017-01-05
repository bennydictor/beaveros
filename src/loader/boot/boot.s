.section    .entry_text
.globl      start
.type       start,          @function

start:
    cli
    mov     $stack_top,     %esp
    mov     %esp,           %ebp
    push    %ebx
    push    %eax
    call    loader_main
.Lhlt:
    hlt
    jmp     .Lhlt

.section    .loader_stack
    .align 8
stack_bottom:
    .space  4 * 1024
stack_top:
