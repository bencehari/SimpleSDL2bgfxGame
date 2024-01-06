#include "init.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <bgfx/c99/bgfx.h>

#include "consc.h"

static SDL_Window* window;

bool initialize(uint32_t flags, int width, int height) {
	printf("SDL_Init... ");
	if (SDL_Init(flags) != 0) {
		printf("\n" ANSI_COLOR_RED "Error initializing SDL: %s" ANSI_COLOR_RESET "\n", SDL_GetError());
		return false;
	}
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	printf("SDL_CreateWindow... ");
	window = SDL_CreateWindow(
		"SDL2/bgfx Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, 0);
		
	if (!window) {
		printf("\n" ANSI_COLOR_RED "Error creating window: %s" ANSI_COLOR_RESET "\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	
	printf("SDL_GetWindowWMInfo... ");
	if (!SDL_GetWindowWMInfo(window, &wmi)) return false;
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	bgfx_init_t init;
	bgfx_init_ctor(&init);
	
	init.resolution.width = width;
	init.resolution.height = height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	
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

	// for switching to single threaded mode
	// https://bkaradzic.github.io/bgfx/internals.html#api-and-render-thread
	bgfx_render_frame(-1);
	
	printf("bgfx_init... ");
	if (!bgfx_init(&init)) return false;
	printf(ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "\n");
	
	bgfx_set_debug(BGFX_DEBUG_TEXT);
	
	bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x2D2D2DFF, 1.0f, 0);
	bgfx_set_view_rect(0, 0, 0, width, height);
	
	return true;
}

void deinitialize(void) {
	bgfx_shutdown();
	while (bgfx_render_frame(1) != BGFX_RENDER_FRAME_NO_CONTEXT) {};
	
	SDL_DestroyWindow(window);
	SDL_Quit();
}
