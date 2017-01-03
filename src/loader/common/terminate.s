.section    .text
.globl      terminate
.type       terminate,  @function
terminate:  cli
.Lhlt:      hlt
            jmp .Lhlt
