#include <io/ocdev.h>

static ocdev_t std_ocdev;

void set_std_ocdev(const ocdev_t ocdev) {
    std_ocdev = ocdev;
}

ocdev_t get_std_ocdev(void) {
    return std_ocdev;
}
