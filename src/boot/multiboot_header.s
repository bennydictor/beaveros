.section .multiboot_header
# Read Multiboot 2 specification at http://www.gnu.org/software/grub/manual/multiboot/multiboot.html
header_start:
    .set magic, 0xe85250d6
    .set arch, 0
    .set length, header_end - header_start
    .set checksum, 0x100000000 - (magic + arch + length)

    .align 8
    .long magic
    .long arch
    .long length
    .long checksum

    .short 0
    .short 0
    .long 8
header_end:
