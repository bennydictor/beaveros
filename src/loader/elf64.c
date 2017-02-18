#include <elf64.h>
#include <io/printf.h>
#include <paging.h>
#include <string.h>
#include <math.h>

bool load_kernel(void *start, uint64_t *entry) {
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

    /* parsing section header table */
    elf64_shdr_t *shdr = start + ehdr->e_shoff;
    char *shstr = start + shdr[ehdr->e_shstrndx].sh_offset;
    char *strtab = NULL;
    elf64_sym_t *symtab = NULL;
    uint32_t symtab_size = 0;
    for (uint16_t si = 0; si < ehdr->e_shnum; ++si) {
        if (!strcmp(".strtab", shstr + shdr[si].sh_name)) {
            strtab = start + shdr[si].sh_offset;
        }
        if (!strcmp(".symtab", shstr + shdr[si].sh_name)) {
            symtab = start + shdr[si].sh_offset;
            symtab_size = shdr[si].sh_size / shdr[si].sh_entsize;
        }
    }

    if (!strtab) {
        printf(".strtab not found\n");
        return false;
    }

    if (!symtab) {
        printf(".symtab not found\n");
        return false;
    }

    uint64_t phys_window_pages = 0;
    uint32_t phys_window_size = 0;
    uint64_t phys_window_pt = 0;

    for (uint32_t si = 1; si < symtab_size; ++si) {
        if (!strcmp("_phys_window_size", strtab + symtab[si].st_name)) {
            phys_window_size = symtab[si].st_value;
        }
        if (!strcmp("_phys_window_pages", strtab + symtab[si].st_name)) {
            phys_window_pages = symtab[si].st_value;
        }
        if (!strcmp("_phys_window_pt", strtab + symtab[si].st_name)) {
            phys_window_pt = symtab[si].st_value;
        }
    }

    if (!phys_window_pages) {
        printf("_phys_window_pages not found\n");
        return false;
    }

    if (!phys_window_size) {
        printf("_phys_window_size not found\n");
        return false;
    }

    if (!phys_window_pt) {
        printf("_phys_window_pt not found\n");
        return false;
    }

    for (uint32_t wi = 0; wi < phys_window_size; ++wi) {
        map_page(phys_window_pages + (wi << 12), 0, PAGE_RW_BIT | PAGE_G_BIT);
    }
    map_page(phys_window_pt,
            (uint32_t) get_used_memory() - 0x1000, PAGE_RW_BIT | PAGE_G_BIT);

    return true;
}
