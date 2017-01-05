.section    .text
.globl      enable_paging
.type       enable_paging,  @function

enable_paging:
    mov     pml4,           %eax
    mov     %eax,           %cr3
    
    mov     %cr4,           %eax
    or      $(1 << 5),      %eax
    mov     %eax,           %cr4

    mov     $0xC0000080,    %eax
    rdmsr
    or      $(1 << 8),      %eax
    wrmsr

    mov     %cr0,           %eax
    or      $(1 << 31),     %eax
    mov     %eax,           %cr0

    ret
