.section     .text
.globl       with_interrupts_disabled
.type        with_interrupts_disabled    @function

with_interrupts_disabled:
    cli
    call %rsi
    sti
    retq

