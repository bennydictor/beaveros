# See Multiboot 2 specification at
# http://nongnu.askapache.com/grub/phcoder/multiboot.pdf

#define MAGIC       0xe85250d6
#define ARCH        0
#define LENGTH      (header_end - header_start)
#define CHECKSUM    (0x100000000 - MAGIC - ARCH - LENGTH)

.section    .multiboot2_header

header_start:
    .align  8
    .long   MAGIC
    .long   ARCH
    .long   LENGTH
    .long   CHECKSUM

    # Module alignment tag
    .align  8
    .short  6
    .short  0
    .long   8

    # End tag
    .align  8
    .short  0
    .short  0
    .long   8

header_end:
