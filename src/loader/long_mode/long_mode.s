.section    .text
.globl      enable_long_mode
.type       enable_long_mode,      @function

.set        CR4_PAE_BIT,    1 << 5
.set        CR0_PG_BIT,     1 << 31
.set        EFER_REGNUM,    0xC0000080
.set        EFER_LME_BIT,   1 << 8

enable_long_mode:
    push    %ebp
    mov     %esp,           %ebp

    # load pml4 address
    mov     pml4,           %eax
    mov     %eax,           %cr3
    
    # enable PAE
    mov     %cr4,           %eax
    or      $CR4_PAE_BIT,   %eax
    mov     %eax,           %cr4

    # enable long mode
    mov     $EFER_REGNUM,   %ecx
    rdmsr
    or      $EFER_LME_BIT,  %eax
    wrmsr
    
    # enable paging
    mov     %cr0,           %eax
    or      $CR0_PG_BIT,    %eax
    mov     %eax,           %cr0

    lgdt    gdt64r
    mov     $0x10,          %ax
    mov     %ax,            %ds
    mov     %ax,            %es
    mov     %ax,            %ss
    mov     %ax,            %fs
    mov     %ax,            %gs

    push    $0x08
    push    8(%ebp)
    lret

.section    .rodata
gdt64:
    .quad   0
    .quad   (1 << 41) | (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
    .quad   (1 << 41) | (1 << 44) | (1 << 47)
gdt64r:
    .short  . - gdt64 - 1
    .long   gdt64
    .long   0
