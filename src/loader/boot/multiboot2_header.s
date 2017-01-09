# See Multiboot 2 specification at http://nongnu.askapache.com/grub/phcoder/multiboot.pdf

.set        MAGIC,      0xe85250d6
.set        ARCH,       0
.set        LENGTH,     header_end - header_start;
.set        CHECKSUM,   0x100000000 - (MAGIC + ARCH + LENGTH)

.section    .multiboot2_header
header_start:
    .align  8
    .long   MAGIC
    .long   ARCH
    .long   LENGTH
    .long   CHECKSUM

    .align  8
    .short  0
    .short  0
    .long   8
header_end:
