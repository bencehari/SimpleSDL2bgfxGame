#ifndef KE_INIT_HH
#define KE_INIT_HH

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes SDL2 and bgfx.
 *
 * @param _flags SDL2 subsystem initialization flags.
 * @param _width Window width in pixels.
 * @param _height Window height in pixels.
 *
 * @return False on error, otherwise true.
*/
extern bool initSystem(uint32_t _flags, int _width, int _height);

/**
 * @brief Shutdown bgfx and SDL2 systems.
*/
extern void cleanupSystem(void);

#endif // KE_INIT_HH
