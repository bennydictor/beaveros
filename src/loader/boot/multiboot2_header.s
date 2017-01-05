# See Multiboot 2 specification at http://nongnu.askapache.com/grub/phcoder/multiboot.pdf

.set        magic,      0xe85250d6
.set        arch,       0
.set        length,     header_end - header_start;
.set        checksum,   0x100000000 - (magic + arch + length)

.section    .multiboot2_header
header_start:
    .align  8
    .long   magic
    .long   arch
    .long   length
    .long   checksum

    .align  8
    .short  0
    .short  0
    .long   8
header_end:
