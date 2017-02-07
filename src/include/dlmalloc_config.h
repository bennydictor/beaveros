#ifndef BEAVER_DLMALLOC_CONFIG_H
#define BEAVER_DLMALLOC_CONFIG_H

#define MALLOC_ALIGNMENT (16)
#define ABORT PANIC("malloc error")
#define MALLOC_FAILURE_ACTION PANIC("enomem") //fixme
#define HAVE_MORECORE 1
#define MORECORE sbrk
#define HAVE_MMAP 0  //fixme
#define HAVE_MREMAP 0
#define malloc_getpagesize (4096)
#define LACKS_UNISTD_H 1
#define LACKS_FCNTL_H 1
#define LACKS_SYS_PARAM_H 1
#define LACKS_SYS_MMAN_H 1
#define LACKS_STRINGS_H 1
#define LACKS_STRING_H 1
#define LACKS_SYS_TYPES_H 1
#define LACKS_ERRNO_H 1
#define LACKS_STDLIB_H 1
#define LACKS_SCHED_H 1
#define LACKS_TIME_H 1
#define NO_MALLOC_STATS 1
#include <stddef.h>
#include <string.h>
#include <terminate.h>
#define EINVAL 22
#define ENOMEM 12

#endif /* BEAVER_DLMALLOC_CONFIG_H */
