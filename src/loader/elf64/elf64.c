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
    elf64_shdr_t *shdr = start + ehdr->e_shoff;
    for (uint16_t si = 0; si < ehdr->e_shnum; ++si) {
        if (shdr[si].sh_type == SHT_PROGBITS ||
            shdr[si].sh_type == SHT_NOBITS) {
            for (uint64_t i = 0; i < shdr[si].sh_size; i += PAGE_SIZE) {
                void *page = new_phys_zero_page();
                if (shdr[si].sh_type == SHT_PROGBITS) {
                    memcpy(page, start + shdr[si].sh_offset, min(shdr[si].sh_size - i, PAGE_SIZE));
                }
                uint64_t flags = PAGE_G_BIT;
                if (shdr[si].sh_flags & SHF_WRITE) {
                    flags |= PAGE_RW_BIT;
                }
                if (!(shdr[si].sh_flags & SHF_EXECINSTR)) {
                    flags |= PAGE_NX_BIT;
                }
                map_page(shdr[si].sh_addr + i, (uint64_t) (uint32_t) page + i, flags);
            }
        }
    }

    return true;
}
