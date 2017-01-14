#include <elf64.h>
#include <io/printf.h>
#include <paging.h>
#include <string.h>
#include <math.h>

bool load_elf64(void *start, uint64_t *entry) {
    elf64_ehdr_t *ehdr = start;
    if (ehdr->e_ident[EI_MAG0] != 0x7f ||
            ehdr->e_ident[EI_MAG1] != 'E' ||
            ehdr->e_ident[EI_MAG2] != 'L' ||
            ehdr->e_ident[EI_MAG3] != 'F') {
        printf("ELF identification incorrect\n");
        return false;
    }
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS64 ||
            ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        printf("Expected 64 bit little endian ELF\n");
        return false;
    }
    if (ehdr->e_ident[EI_VERSION] != 1) {
        printf("Expected ELF version 1\n");
        return false;
    }
    if (ehdr->e_ident[EI_OSABI] != ELFOSABI_SYSV ||
            ehdr->e_ident[EI_ABIVERSION] != 0) {
        printf("Expected System V ABI version 0\n");
        return false;
    }
    if (ehdr->e_type != ET_EXEC) {
        printf("Expected executable\n");
        return false;
    }
    *entry = ehdr->e_entry;

    elf64_phdr_t *phdr = start + ehdr->e_phoff;
    for (uint16_t pi = 0; pi < ehdr->e_phnum; ++pi) {
        if (phdr[pi].p_type == PT_LOAD) {
            for (uint64_t i = 0; i < phdr[pi].p_memsz; i += PAGE_SIZE) {
                void *page = start + phdr[pi].p_offset + i;
                if (i < phdr[pi].p_filesz) {
                    if (phdr[pi].p_filesz < i + PAGE_SIZE) {
                        memset(page + phdr[pi].p_filesz - i,
                                0, i + PAGE_SIZE - phdr[pi].p_filesz);
                    }
                } else {
                    page = new_phys_zero_page();
                }
                map_page(phdr[pi].p_vaddr + i, (uint64_t) (uint32_t) page,
                        PAGE_RW_BIT | PAGE_G_BIT);
            }
        }
    }

    return true;
}
