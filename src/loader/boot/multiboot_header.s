# See Multiboot 2 specification at http://nongnu.askapache.com/grub/phcoder/multiboot.pdf
.section .multiboot_header
header_start:
    .set    magic,      0xe85250d6
    .set    arch,       0

    .set    length,     header_end - header_start
    .set    checksum,   0x100000000 - (magic + arch + length)

    .align  8
    .long   magic
    .long   arch
    .long   length
    .long   checksum

    # Module alignmet tag
    .short  6           # type
    .short  1           # flags
    .long   8           # size

    # End tag
    .short  0           # type
    .short  0           # flags
    .long   8           # size
header_end:
