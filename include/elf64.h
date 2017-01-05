#ifndef BEAVER_ELF64_H
#define BEAVER_ELF64_H

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t elf64_addr;
typedef uint64_t elf64_off;
typedef uint16_t elf64_half;
typedef uint32_t elf64_word;
typedef  int32_t elf64_sword;
typedef uint64_t elf64_xword;
typedef  int64_t elf64_sxword;

typedef struct {
    unsigned char   e_ident[16];    /* ELF identification */
    elf64_half      e_type;         /* Object file type */
    elf64_half      e_machine;      /* Machine type */
    elf64_word      e_version;      /* Object file version */
    elf64_addr      e_entry;        /* Entry point address */
    elf64_off       e_phoff;        /* Program header offset */
    elf64_off       e_shoff;        /* Section header offset */
    elf64_word      e_flags;        /* Processor-specific flags */
    elf64_half      e_ehsize;       /* ELF header size */
    elf64_half      e_phentsize;    /* Size of program header entry */
    elf64_half      e_phnum;        /* Number of program header entries */
    elf64_half      e_shentsize;    /* Size of section header entry */
    elf64_half      e_shnum;        /* Number of section header entries */
    elf64_half      e_shstrndx;     /* Section name string table index */
} elf64_ehdr_t;

#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4
#define EI_DATA         5
#define EI_VERSION      6
#define EI_OSABI        7
#define EI_ABIVERSION   8
#define EI_PAD          9
#define EI_NINDENT      16

#define ELFCLASS32      1
#define ELFCLASS64      2

#define ELFDATA2LSB     1
#define ELFDATA2MSB     2

#define ELFOSABI_SYSV       0
#define ELFOSABI_HPUX       1
#define ELFOSABI_STANDALONE 255

#define ET_NONE     0
#define ET_REL      1
#define ET_EXEC     2
#define ET_DYN      3
#define ET_CORE     4
#define ET_LOOS     0xFE00
#define ET_HIOS     0xFEFF
#define ET_LOPROC   0xFF00
#define ET_HIPROC   0xFFFF

#define SHN_UNDEF   0
#define SHN_LOPROC  0xFF00
#define SHN_HIPROC  0xFF1F
#define SHN_LOOS    0xFF20
#define SHN_HIOS    0xFF3F
#define SHN_ABS     0xFFF1
#define SHN_COMMON  0xFFF2

typedef struct {
    elf64_word  sh_name;        /* Section name */
    elf64_word  sh_type;        /* Section type */
    elf64_xword sh_flags;       /* Section attributes */
    elf64_addr  sh_addr;        /* Virtual address in memory */
    elf64_off   sh_offset;      /* Offset in file */
    elf64_xword sh_size;        /* Size of section */
    elf64_word  sh_link;        /* Link to other section */
    elf64_word  sh_info;        /* Miscellaneous information */
    elf64_xword sh_addralign;   /* Address alignment boundary */
    elf64_xword sh_entsize;     /* Size of entries, if section has table */
} elf64_shdr_t;

#define SHT_NULL        0
#define SHT_PROGBITS    1
#define SHT_SYMTAB      2
#define SHT_STRTAB      3
#define SHT_RELA        4
#define SHT_HASH        5
#define SHT_DYNAMIC     6
#define SHT_NOTE        7
#define SHT_NOBITS      8
#define SHT_REL         9
#define SHT_SHLIB       10
#define SHT_DYNSYM      11
#define SHT_LOOS        0x60000000
#define SHT_HIOS        0x6FFFFFFF
#define SHT_LOPROC      0x70000000
#define SHT_HIPROC      0x7FFFFFFF

#define SHF_WRITE       0x1
#define SHF_ALLOC       0x2
#define SHF_EXECINSTR   0x4
#define SHF_MASKOS      0x0F000000
#define SHF_MASKPROC    0xF0000000

typedef struct {
    elf64_word      st_name;    /* Symbol name */
    unsigned char   st_info;    /* Type and Binding attributes */
    unsigned char   st_other;   /* Reserved */
    elf64_half      st_shndx;   /* Section table index */
    elf64_addr      st_value;   /* Symbol value */
    elf64_xword     st_size;    /* Size of object (e.g., common) */
} elf64_sym_t;

#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_LOOS    10
#define STB_HIOS    12
#define STB_LOPROC  13
#define STB_HIPROC  15

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_LOOS    10
#define STT_HIOS    12
#define STT_LOPROC  13
#define STT_HIPROC  15

typedef struct {
    elf64_addr  r_offset;   /* Address of reference */
    elf64_xword r_info;     /* Symbol index and type of relocation */
} elf64_rel_t;

typedef struct {
    elf64_addr      r_offset;   /* Address of reference */
    elf64_xword     r_info;     /* Symbol index and type of relocation */
    elf64_sxword    r_addend;   /* Constant part of expression */
} elf64_rela_t;

#define ELF64_R_SYM(I)      ((I) >> 32)
#define ELF64_R_TYPE(I)     ((I) & 0xffffffffL)
#define ELF64_R_INFO(S, T)  (((S) << 32) + ((T) & 0xffffffffL))

typedef struct {
    elf64_word  p_type;     /* Type of segment */
    elf64_word  p_flags;    /* Segment attributes */
    elf64_off   p_offset;   /* Offset in file */
    elf64_addr  p_vaddr;    /* Virtual address in memory */
    elf64_addr  p_paddr;    /* Reserved */
    elf64_xword p_filesz;   /* Size of segment in file */
    elf64_xword p_memsz;    /* Size of segment in memory */
    elf64_xword p_align;    /* Alignment of segment */
} elf64_phdr_t;

#define PT_NULL     0
#define PT_LOAD     1
#define PT_DYNAMIC  2
#define PT_INTERP   3
#define PT_NOTE     4
#define PT_SHLIB    5
#define PT_PHDR     6
#define PT_LOOS     0x60000000
#define PT_HIOS     0x6FFFFFFF
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7FFFFFFF

#define PF_X        0x1
#define PF_W        0x2
#define PF_R        0x4
#define PF_MASKOS   0x00FF0000
#define PF_MASKPROC 0xFF000000

typedef struct {
    elf64_sxword    d_tag;
    union {
        elf64_xword d_val;
        elf64_addr  d_ptr;
    } d_un;
} elf64_dyn_t;

#define DT_NULL         0
#define DT_NEEDED       1
#define DT_PLTRELSZ     2
#define DT_PLTGOT       3
#define DT_HASH         4
#define DT_STRTAB       5
#define DT_SYMTAB       6
#define DT_RELA         7
#define DT_RELASZ       8
#define DT_RELAENT      9
#define DT_STRSZ        10
#define DT_SYMENT       11
#define DT_INIT         12
#define DT_FINI         13
#define DT_SONAME       14
#define DT_RPATH        15
#define DT_SYMBOLIC     16
#define DT_REL          17
#define DT_RELSZ        18
#define DT_RELENT       19
#define DT_PLTREL       20
#define DT_DEBUG        21
#define DT_TEXTREL      22
#define DT_JMPREL       23
#define DT_BIND_NOW     24
#define DT_INIT_ARRAY   25
#define DT_FINI_ARRAY   26
#define DT_INIT_ARRAYSZ 27
#define DT_FINI_ARRAYSZ 28
#define DT_LOOS         0x60000000
#define DT_HIOS         0x6FFFFFFF
#define DT_LOPROC       0x70000000
#define DT_HIPROC       0x7FFFFFFF

void print_elf64(void *);
bool load_elf64(void *, uint64_t *);

#endif // BEAVER_ELF64_H
