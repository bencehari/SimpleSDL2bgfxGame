#ifndef KE_INIT_H
#define KE_INIT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

extern bool sys_initialize(uint32_t _flags, int _width, int _height);
extern void sys_deinitialize(void);

#endif // KE_INIT_H
