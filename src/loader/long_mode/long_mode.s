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

    ljmp    $0x08,          $.Ljmp64

.code64
.Ljmp64:
    jmp     *8(%rbp)

.section    .rodata
gdt64:
    .word   0x0,    0x0,    0x0,    0x0     # null segment descriptor
    .word   0xffff, 0x0000, 0x9a00, 0x00af  # code segment descriptor
    .word   0xffff, 0x0000, 0x9200, 0x00cf  # data segment descriptor
gdt64r:
    .short  . - gdt64 - 1
    .long   gdt64
    .long   0
