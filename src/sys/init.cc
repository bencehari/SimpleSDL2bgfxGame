#include "init.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "../bx/platform.h"

#include "../utils/consc.h"

static void printDetectedPlatform(void);

static SDL_Window* window;

bool initSystem(uint32_t _flags, int _width, int _height) {
	printf("SDL_Init... ");
	if (SDL_Init(_flags) != 0) {
		printf("\n" AC_RED "Error initializing SDL: %s" AC_RESET "\n", SDL_GetError());
		return false;
	}
	puts(AC_GREEN "DONE" AC_RESET);
	
	printf("SDL_CreateWindow... ");
	window = SDL_CreateWindow(
		"SDL2/bgfx Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_width, _height, 0);
		
	if (!window) {
		printf("\n" AC_RED "Error creating window: %s" AC_RESET "\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	puts(AC_GREEN "DONE" AC_RESET);
	
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	
	printf("SDL_GetWindowWMInfo... ");
	if (!SDL_GetWindowWMInfo(window, &wmi)) return false;
	puts(AC_GREEN "DONE" AC_RESET);

	printDetectedPlatform();
	
#if BX_PLATFORM_NONE
	puts(AC_RED "NO BX_PLATFORM DEFINED" AC_RESET);
	return false;
#endif
	
	bgfx::Init init;
	init.type = bgfx::RendererType::Count;
	init.platformData.nwh = nullptr;
	init.platformData.ndt = nullptr;
	init.platformData.type = bgfx::NativeWindowHandleType::Count;
	init.resolution.width = _width;
	init.resolution.height = _height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	puts(AC_MAGENTA "BX_PLATFORM_LINUX || BX_PLATFORM_BSD" AC_RESET);
	init.platformData.ndt = wmi.info.x11.display;
	init.platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
	puts(AC_MAGENTA "BX_PLATFORM_OSX" AC_RESET);
	init.platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
	puts(AC_MAGENTA "BX_PLATFORM_WINDOWS" AC_RESET);
	init.platformData.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
	puts(AC_MAGENTA "BX_PLATFORM_STEAMLINK" AC_RESET);
	init.platformData.ndt = wmi.info.vivante.display;
	init.platformData.nwh = wmi.info.vivante.window;
#else
	puts(AC_RED "UNKNOWN BX_PLATFORM" AC_RESET);
	return false;
#endif // BX_PLATFORM_

	// for switching to single threaded mode
	// https://bkaradzic.github.io/bgfx/internals.html#api-and-render-thread
	// bgfx::renderFrame(-1)
	
	/*
	RendererTypes
	( 0) Noop,			// No rendering.
	( 1) Agc,			// AGC
	( 2) Direct3D11,	// Direct3D 11.0
	( 3) Direct3D12,	// Direct3D 12.0
	( 4) Gnm,			// GNM
	( 5) Metal,			// Metal
	( 6) Nvn,			// NVN
	( 7) OpenGLES,		// OpenGL ES 2.0+
	( 8) OpenGL,		// OpenGL 2.1+
	( 9) Vulkan,		// Vulkan
	(10) Count
	
	NativeWindowHandleType
	( 0) Default = 0,	// Platform default handle type (X11 on Linux).
	( 1) Wayland,		// Wayland.
	( 2) Count
	*/
	printf(
		AC_YELLOW "bgfx Init data:\n" AC_RESET
		"Type:         %s (%d, pd: %d)\n"
		"ndt:          %s\n"
		"nwh:          %s\n"
		"context:      %s\n"
		"backBuffer:   %s\n"
		"backBufferDS: %s\n",
		bgfx::RendererType::Noop != init.type ? (AC_GREEN "!Noop" AC_RESET) : (AC_RED "Noop" AC_RESET), init.type, init.platformData.type,
		init.platformData.ndt == nullptr ? (AC_RED "NULL" AC_RESET) : (AC_GREEN "NOT NULL" AC_RESET),
		init.platformData.nwh == nullptr ? (AC_RED "NULL" AC_RESET) : (AC_GREEN "NOT NULL" AC_RESET),
		init.platformData.context == nullptr ? (AC_RED "NULL" AC_RESET) : (AC_GREEN "NOT NULL" AC_RESET),
		init.platformData.backBuffer == nullptr ? (AC_RED "NULL" AC_RESET) : (AC_GREEN "NOT NULL" AC_RESET),
		init.platformData.backBufferDS == nullptr ? (AC_RED "NULL" AC_RESET) : (AC_GREEN "NOT NULL" AC_RESET));
	// */

	printf("bgfx_init... ");
	if (!bgfx::init(init)) return false;
	puts(AC_GREEN "DONE" AC_RESET);
	
	bgfx::setDebug(BGFX_DEBUG_TEXT);
	
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x2D2D2DFF, 1.0f, 0);
	
	return true;
}

void cleanupSystem(void) {
	puts(AC_MAGENTA "Cleaning up bgfx..." AC_RESET);
	bgfx::shutdown();
	while (bgfx::renderFrame(1) != bgfx::RenderFrame::NoContext) {};
	
	puts(AC_MAGENTA "Cleaning up SDL..." AC_RESET);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void printDetectedPlatform(void) {
	printf(AC_MAGENTA "PLATFORM DEFINED AS " AC_RESET);
#if BX_PLATFORM_ANDROID
	puts(AC_MAGENTA "BX_PLATFORM_ANDROID" AC_RESET);
#elif BX_PLATFORM_BSD
	puts(AC_MAGENTA "BX_PLATFORM_BSD" AC_RESET);
#elif BX_PLATFORM_EMSCRIPTEN
	puts(AC_MAGENTA "BX_PLATFORM_EMSCRIPTEN" AC_RESET);
#elif BX_PLATFORM_HAIKU
	puts(AC_MAGENTA "BX_PLATFORM_HAIKU" AC_RESET);
#elif BX_PLATFORM_HURD
	puts(AC_MAGENTA "BX_PLATFORM_HURD" AC_RESET);
#elif BX_PLATFORM_IOS
	puts(AC_MAGENTA "BX_PLATFORM_IOS" AC_RESET);
#elif BX_PLATFORM_LINUX
	puts(AC_MAGENTA "BX_PLATFORM_LINUX" AC_RESET);
#elif BX_PLATFORM_NX
	puts(AC_MAGENTA "BX_PLATFORM_NX" AC_RESET);
#elif BX_PLATFORM_OSX
	puts(AC_MAGENTA "BX_PLATFORM_OSX" AC_RESET);
#elif BX_PLATFORM_PS4
	puts(AC_MAGENTA "BX_PLATFORM_PS4" AC_RESET);
#elif BX_PLATFORM_PS5
	puts(AC_MAGENTA "BX_PLATFORM_PS5" AC_RESET);
#elif BX_PLATFORM_RPI
	puts(AC_MAGENTA "BX_PLATFORM_RPI" AC_RESET);
#elif BX_PLATFORM_WINDOWS
	puts(AC_MAGENTA "BX_PLATFORM_WINDOWS" AC_RESET);
#elif BX_PLATFORM_WINRT
	puts(AC_MAGENTA "BX_PLATFORM_WINRT" AC_RESET);
#elif BX_PLATFORM_XBOXONE
	puts(AC_MAGENTA "BX_PLATFORM_XBOXONE" AC_RESET);
#else
	puts(AC_MAGENTA "???" AC_RESET);
#endif
}