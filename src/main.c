#include <stdio.h>

#include <SDL2/SDL.h>

#include "sys/init.h"
#include "game.h"

#include "utils/consc.h"

#define WIDTH 1024
#define HEIGHT 768
#define WIDTH_F 1024.0
#define HEIGHT_F 768.0
#define FPS 60

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char* argv[]) {
#pragma GCC diagnostic pop
	Uint32 initFlags =
		SDL_INIT_TIMER |
		SDL_INIT_AUDIO |
		SDL_INIT_VIDEO |
		// SDL_INIT_JOYSTICK |
		// SDL_INIT_HAPTIC |
		// SDL_INIT_GAMECONTROLLER |
		SDL_INIT_EVENTS //|
		// SDL_INIT_SENSOR |
		// SDL_INIT_NOPARACHUTE
		;
	
	if (!sys_initialize(initFlags, WIDTH, HEIGHT)) {
		printf("Initialization failed. Terminating...");
		return 0;
	}
	
	game(WIDTH_F, HEIGHT_F, FPS);
	
	sys_cleanup();
	puts(AC_MAGENTA "DONE" AC_RESET);
	
	return 0;
}
