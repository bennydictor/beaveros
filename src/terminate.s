.section    .text

.globl      terminate
.type       terminate,  @function

terminate:
    cli
.terminate_hlt:
    hlt
    jmp     .terminate_hlt

.globl      intloop
.type       intloop,    @function

intloop:
    sti
.intloop_hlt:
    hlt
    jmp     .intloop_hlt
