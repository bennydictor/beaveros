.section    .text
.globl      start
.type       start,                      @function

start:
            cli
            call kernel_main
.Lhlt:      hlt
            jmp     .Lhlt

.section    .bss
stack_bottom:
            .space 16 * 1024
stack_top:
