#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <bgfx/c99/bgfx.h>

#include "init.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* argv[]) {
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
	
	if (!initialize(initFlags, WIDTH, HEIGHT)) {
		printf("Initialization failed. Terminating...");
		return 0;
	}
	
	// INITIALIZATION FINISHED
	
	bool running = true;
	SDL_Event event;	
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_WINDOWEVENT: {
					const SDL_WindowEvent wev = event.window;
					switch (wev.event) {
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_SIZE_CHANGED: break;
						case SDL_WINDOWEVENT_CLOSE: running = false; break;
					}
				} break;
			}
		}
		
		bgfx_touch(0);
		bgfx_frame(false);
	}

	deinitialize();
	
	return 0;
}
