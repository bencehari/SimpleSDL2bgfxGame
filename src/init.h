#ifndef INIT_H
#define INIT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool initialize(uint32_t flags, int width, int height);
void deinitialize(void);

#endif // INIT_H
