#include <elf64.h>
#include <io/printf.h>
#include <assert.h>

void print_elf64(void *data) {
    Elf64_Ehdr *elf_header = data;
    ASSERT(elf_header->e_ident[0] == 0x7f);
    ASSERT(elf_header->e_ident[1] == 'E');
    ASSERT(elf_header->e_ident[2] == 'L');
    ASSERT(elf_header->e_ident[3] == 'F');

    io_printf("Elf file detected\n==========================\n");
    uint32_t idx = 4;
    io_printf("File class: %hhd\n", elf_header->e_ident[idx++]);
    io_printf("Data encoding: %hhd\n", elf_header->e_ident[idx++]);
    io_printf("File version: %hhd\n", elf_header->e_ident[idx++]);
    io_printf("OS/ABI identification: %hhd\n", elf_header->e_ident[idx++]);
    io_printf("ABI version: %hhd\n", elf_header->e_ident[idx++]);
    io_printf("Start of padding bytes: %hhd\n", elf_header->e_ident[idx++]);

    io_printf("\nObject file type: %hd\n", elf_header->e_type);
    io_printf("Machine type: %hd\n", elf_header->e_machine);
    io_printf("Object file version: %ld\n", elf_header->e_version);
    io_printf("Entry point address: %#llx\n", elf_header->e_entry);
    io_printf("Program header offset: %#llx\n", elf_header->e_phoff);
    io_printf("Section header offset: %#llx\n", elf_header->e_shoff);
    io_printf("Processor-specific flags: %lx\n", elf_header->e_flags);
    io_printf("ELF header size: %hd\n", elf_header->e_ehsize);
    io_printf("Size of program header entry: %hd\n", elf_header->e_phentsize);
    io_printf("Number of program header entries: %hd\n", elf_header->e_phnum);
    io_printf("Size of section header entry: %hd\n", elf_header->e_shentsize);
    io_printf("Number of section header entries: %hd\n", elf_header->e_shnum);
    io_printf("Section name string table index: %hd\n", elf_header->e_shstrndx);
}
