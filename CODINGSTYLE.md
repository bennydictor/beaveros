# Which languages to use
* C, 1999 standart edition with gnu extensions (`-std=gnu99`). Do not use trigraphs.
* `x86` Assembler, GAS syntax
* `x86_64` Assembler, GAS syntax

## File extensions
For C files use extension `.c`, for headers use `.h`,
for assembler files use `.s`, or `.S` if you use C preprocessor.
There should not be two files in the same directory named like
`file.s` and `file.S`.
This is required for Makefile to work correctly.

## File locations
Place all files in `src/` directory. Group C and assembler files
in the same directory, if they provide definitions for the same
header. Place header in `include/` directories, and group
them in sub-directories by meaning.

## Errors
Use `PANIC` macro for fatal errors. Use `WARNING` macro for warnings.
When using these two macros, start the message from lower case letter.


# C code formatting
Use spaces for indentation and alignment. Tabs are four spaces.
Line lingth is 80 characters, including comments. Always use braces on one-line
statements inside for, if, etc.

## Header structure
* Include guard
* Includes
* Defines
* Typedefs
* Structures
* Variables
* Functions
* Include guard

This file structure is recommended, but you can ignore it,
if it improves readability.

## C file structure
* Includes
* Defines
* Typedefs
* Structures
* Variables
* Functions

This file structure is recommended, but you can ignore it,
if it improves readability.

## Blank lines
If you want to separate blocks of code, use exaclty one blank line.

You should always use a blank line:
* If two entities in the file are from different categories, as described in *Header structure*
  and *C file structure*
* Between two structs or two function definitions.

## Spaces
Add one space after:
* for
* if
* while
* casts

Do not add space after function name.

## Include guards
Name one like `BEAVER_PATH_TO_FILE_H`, where `PATH_TO_FILE`
is path to file, starting from but not including `src/` directory
and all non-word (all but letters, numbers and `_`) characters are
replaced by `_`. Do not use `#pragma once`.

## Defines
Name defines and their arguments in `UPPERCASE_WITH_UNDERSCORES`.

Do not add parentheses around a simple expression define, like one number.
```
#define VAR 1   /* Good */
#define VAR (1) /* Bad */
```

If the define is a block of code to be used like a function call,
include all contents in a statement expression.
```
#define TREE_GET(TREE, L, R) ({ \
    _tree_get((TREE), 0, 0, (TREE).size, (L), (R)); \
});
/* Now you can write statements like
   int a = TREE_GET(root, 0, k); */
```

Align consecutive defines to the same tab.

## Structures
This includes also enums and unions.
Do not name structures, instead typedef them.
Use `lowercase_with_underscores`.

## Typedefs
End each typedef with `_t`, including structure names.

## Variables
Use `lowercase_with_underscores`. If you need to export a variable, but don't want it to be
accessible from outside, append `_` character to the beginning.

## Function declatations
Function return type, name and argument types are on the same line.
Include argument names, unless argument meaning is obvious from context.
If you need to export a function, but don't want it to be
accessible from outside, append `_` character to the beginning.
```
int foo(int, void *, size_t);
```

## Function definitions
Function return type, name and arguments are on the same line. Do not use old-style
argument declaration.
```
int foo(int val, void *ptr, size_t size) { /* Good */
    /* Actual definition */
}

int
foo(val, ptr, size) /* Bad */
    int val;
    void *ptr;
    size_t size; {
    /* Actual definition */
}
```

## Braces
Open brace is on the same line, after one blank. Always brace
body of `if`, `for`, etc., even if it is one expression.
Closing brace is on a separate line, with the same indentation
level, as the current block, except for `} else {` and `} while (...);` in do-while.
```
    if (cond) {
        do_smth();
    } else {
        do_smth_else();
    }
```

## Long lines
If breaking around a binary operator, leave it in the first line.
Add two tabs before the second line, and one extra tab for each parentheses
level. When breaking inside a function call, do not line up with parentheses.
```
    if (ehdr->e_ident[EI_MAG0] != 0x7f ||
            (ehdr->e_ident[EI_MAG1] != 'E' || /* Two tabs on line break */
                ehdr->e_ident[EI_MAG2] != 'L' || /* Extra tab for parentheses */
                ehdr->e_ident[EI_MAG3] != 'F')) {
        printf("ELF identification incorrect\n");
        return false;
    }
```

## Labels
Labels are indented one tab below the code, including case labels.

## Comments
use `C` style (`/* */`), and do not add a star at each line.
A comment starts at column 33 or later, all aligned.
Indent comment to start at column 33 or later,
except preprocessor comments, they are after one space.
```
    if (foo) {                  /* Foo! */
        bar();
    }
...
#endif /* baz */
```

# Assembler code formatting
Think of the source file as a table. Each word occupies one element.
Now just add tabs, so each column does not intersect any other.
Instructions are padded by one tab in it's element. The exception are
lines with one label, the label is not indented and you should
not add it to your imaginary table.

## Blank lines
As in C, do not add more than one blank line.
Use blank lines:
* After `.section`
* After label type related stuff (`.globl`, `.type`, etc.)
* Before a global label with it's alignment, etc.

## Labels
Name labels in `lowercase_with_underscores`.
For local labels add `.L` prefix.

## Instructions
Do not include instruction suffix unless it creates ambiguity.

## Defines
Don not use `.set`, use `#define` instead.

# Code examples
## Header example
```
#ifndef BEAVER_TASK_H
#define BEAVER_TASK_H

#include <memory.h>
#include <stdint.h>

#define TASK_SLEEPING   0
#define TASK_RUNNING    1

typedef uint8_t task_flags_t;

typedef struct {
    void *stack;
    pid_t pid;
    task_flags_t flags;
} task_t;

extern task_t *tasks;

task_t add_task(pid_t, void *);

#endif /* BEAVER_TASK_H */
```

## C file example
```
#include <task.h>
#include <registers.h>

#define STACK_SIZE (2 * 1024 * 1024)
#define LOG(FORMAT, VAR) ({ \
    printf((FORMAT), (VAR)); \
    (VAR); \
})

task_t *tasks;

task_t add_task(pid_t pid, void *entry) {
    if (!is_user_space(LOG("entry = %#.16llx", entry))) {
        PANIC("entry is not in user space");
    }

    void *stack = alloc_pages(STACK_SIZE);
    if (!stack) {
        PANIC("No availible memory for stack");
    }
    registers_t *registers =
            (registers_t *) (stack + STACK_TOP - sizeof(registers_t));
    registers.rsp = stack + STACK_TOP;
    registers.rip = entry;

    task_t ret;
    ret.stack = stack + STACK_SIZE;
    ret.pid = pid;
    ret.state = TASK_SLEEPING;
    return ret;
}
```

## Assembly file example
```
.section    .text

.globl      task_switch
.type       task_switch,    @function

enter_task:
    movb    $TASK_RUNNING,  10(%rdi)
    call    load_page_map
    pop     %rax
    pop     %rbx
    pop     %rcx
    pop     %rdx
    pop     %rdi
    pop     %rsi
    pop     %r8
    pop     %r9
    pop     %r10
    pop     %r11
    pop     %r12
    pop     %r13
    pop     %r14
    pop     %r15
    pop     %rbp
    pop     %rsp
    ret
```
