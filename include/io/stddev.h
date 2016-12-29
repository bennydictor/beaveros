#ifndef BEAVER_IO_STDDEV_H
#define BEAVER_IO_STDDEV_H

#include <io/output_character_device.h>

void io_set_std_ocdev(const ocdev_t);
const ocdev_t io_get_std_ocdev(void);

#endif // BEAVER_IO_STDDEV_H
