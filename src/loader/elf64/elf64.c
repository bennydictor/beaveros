#include <elf64.h>
#include <io/printf.h>
#include <assert.h>

void print_elf64(void *data) {
    elf64_ehdr_t *elf_header = data;
    ASSERT(elf_header->e_ident[0] == 0x7f);
    ASSERT(elf_header->e_ident[1] == 'E');
    ASSERT(elf_header->e_ident[2] == 'L');
    ASSERT(elf_header->e_ident[3] == 'F');

    printf("Elf file detected\n==========================\n");
    uint32_t idx = 4;
    printf("File class: %hhd\n", elf_header->e_ident[idx++]);
    printf("Data encoding: %hhd\n", elf_header->e_ident[idx++]);
    printf("File version: %hhd\n", elf_header->e_ident[idx++]);
    printf("OS/ABI identification: %hhd\n", elf_header->e_ident[idx++]);
    printf("ABI version: %hhd\n", elf_header->e_ident[idx++]);
    printf("Start of padding bytes: %hhd\n", elf_header->e_ident[idx++]);

    printf("\nObject file type: %hd\n", elf_header->e_type);
    printf("Machine type: %hd\n", elf_header->e_machine);
    printf("Object file version: %ld\n", elf_header->e_version);
    printf("Entry point address: %#llx\n", elf_header->e_entry);
    printf("Program header offset: %#llx\n", elf_header->e_phoff);
    printf("Section header offset: %#llx\n", elf_header->e_shoff);
    printf("Processor-specific flags: %lx\n", elf_header->e_flags);
    printf("ELF header size: %hd\n", elf_header->e_ehsize);
    printf("Size of program header entry: %hd\n", elf_header->e_phentsize);
    printf("Number of program header entries: %hd\n", elf_header->e_phnum);
    printf("Size of section header entry: %hd\n", elf_header->e_shentsize);
    printf("Number of section header entries: %hd\n", elf_header->e_shnum);
    printf("Section name string table index: %hd\n", elf_header->e_shstrndx);
}
