.extern     early_main
.extern     main
.extern     _kernel_stack_top

.section    .text

.globl      start
.type       start,                  @function

start:
    mov     %rsi,                   %rbx
    and     $0xfffffffffffffff0,    %rsp
    call    early_main

    lea     _kernel_stack_top,      %rbp
    mov     %rbp,                   %rsp

    mov     %rbx,                   %rdi
    call    main

    cli
.hlt:
    hlt
    jmp     .hlt
