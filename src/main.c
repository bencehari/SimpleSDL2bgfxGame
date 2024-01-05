#include <stdio.h>
#include <stdbool.h>
// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
// bgfx
#include <bgfx/c99/bgfx.h>

#include "consc.h"

#define WIDTH 640
#define HEIGHT 480

bool init_bgfx(SDL_Window* window);

int main(int argc, char* argv[]) {
	unsigned int initFlags =
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
	
	printf("SDL_Init... ");
	if (SDL_Init(initFlags) != 0) {
		printf("\n" ANSI_COLOR_RED "Error initializing SDL: %s" ANSI_COLOR_RESET "\n", SDL_GetError());
		return 0;
	}
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	printf("SDL_CreateWindow... ");
	SDL_Window* window = SDL_CreateWindow(
		"SDL2/bgfx Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, 0);
		
	if (!window) {
		printf("\n" ANSI_COLOR_RED "Error creating window: %s" ANSI_COLOR_RESET "\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	printf("init_bgfx...\n");
	if (!init_bgfx(window)) {
		printf("\n" ANSI_COLOR_RED "Initializing bgfx failed." ANSI_COLOR_RESET);
		return 0;
	}
	printf("init_bgfx " ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
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
		
		bgfx_set_view_rect(0, 0, 0, WIDTH, HEIGHT);
		bgfx_touch(0);
		bgfx_frame(false);
	}
	
	bgfx_shutdown();
	while (bgfx_render_frame(1) != BGFX_RENDER_FRAME_NO_CONTEXT) {};
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

bool init_bgfx(SDL_Window* window) {
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	
	printf("- SDL_GetWindowWMInfo... ");
	if (!SDL_GetWindowWMInfo(window, &wmi)) return false;
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	bgfx_init_t init;
	bgfx_init_ctor(&init);
	
	init.resolution.width = WIDTH;
	init.resolution.height = HEIGHT;
	
	init.platformData.context = NULL;
	init.platformData.backBuffer = NULL;
	init.platformData.backBufferDS = NULL;
	init.platformData.ndt = NULL;
	
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = wmi.info.x11.display;
	init.platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
	init.platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
	init.platformData.ndt = wmi.info.vivante.display;
	init.platformData.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
	
	printf("- bgfx_init...\n");
	if (!bgfx_init(&init)) return false;
	printf("- " ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x2D2D2DFF, 1.0f, 0);
	
	return true;
}
