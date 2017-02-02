#include <stddef.h>
#include <mapper.h>
extern void *_prog_break_begin;
static void *prog_break_begin = &_prog_break_begin;
extern void *_prog_break_end;
static void *prog_break_end = &_prog_break_end;

static void *prog_break = &_prog_break_begin;

void *sbrk(ptrdiff_t difference) {
    void *new_break = prog_break + difference;
    new_break = (void*)((((uint64_t)new_break + 4095) / 4096) * 4096);
    void *prev_break = prog_break;
    if (new_break > prog_break_end || new_break < prog_break_begin) {
        PANIC("sbrk out of bounds");
    }
    if (difference > 0) {
        for (; prog_break < new_break; prog_break += 4096) {
            map_page(prog_break, MAP_ANON, PAGE_P_BIT | PAGE_RW_BIT | PAGE_G_BIT);
        }
    } else {
        for (; prog_break > new_break; prog_break -= 4096) {
            map_page(prog_break - 4096, NULL, 0);
        }
    }
    return prev_break;
}
