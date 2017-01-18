#ifndef BEAVER_IO_OCDEV_H
#define BEAVER_IO_OCDEV_H

#include <stddef.h>

typedef struct {
    void (*putc) (char);
    int (*puts) (const char *);
    int (*putns) (const char *, size_t);
} ocdev_t;

extern ocdev_t std_ocdev;

#endif /* BEAVER_IO_OCDEV_H */
