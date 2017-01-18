.section    .text

.globl      terminate
.type       terminate,  @function

terminate:
    cli
.hlt:
    hlt
    jmp     .hlt
