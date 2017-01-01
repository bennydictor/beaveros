.section    .text
.globl      start
.type       start,                      @function

start:
            cli
            pushl %ebp
            movl %esp, %ebp 
            pushl %ebx
            pushl %eax
            call kernel_main
            addl $8, %esp
            popl %ebp
.Lhlt:      hlt
            jmp     .Lhlt

.section    .bss
stack_bottom:
            .space 16 * 1024
stack_top:
