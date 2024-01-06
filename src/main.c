#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <bgfx/c99/bgfx.h>

#include "init.h"
#include "loaders.h"
#include "bx_math.h"

#define WIDTH 640
#define HEIGHT 480
#define WIDTH_F 640.0
#define HEIGHT_F 480.0
#define FPS 60

struct PosColorVertex {
	float x;
	float y;
	float z;
	uint32_t abgr;
};

static struct PosColorVertex vertices[] = {
	{ -1.0f,  1.0f,  1.0f, 0xff000000 },
    {  1.0f,  1.0f,  1.0f, 0xff0000ff },
    { -1.0f, -1.0f,  1.0f, 0xff00ff00 },
    {  1.0f, -1.0f,  1.0f, 0xff00ffff },
    { -1.0f,  1.0f, -1.0f, 0xffff0000 },
    {  1.0f,  1.0f, -1.0f, 0xffff00ff },
    { -1.0f, -1.0f, -1.0f, 0xffffff00 },
    {  1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t indices[] = {
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

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
	
	bgfx_vertex_layout_t vertexLayout;
	bgfx_vertex_layout_begin(&vertexLayout, BGFX_RENDERER_TYPE_COUNT);
	bgfx_vertex_layout_add(&vertexLayout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false, false);
	bgfx_vertex_layout_add(&vertexLayout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8, true, false);
	bgfx_vertex_layout_end(&vertexLayout);
	
	bgfx_vertex_buffer_handle_t vbh = bgfx_create_vertex_buffer(bgfx_make_ref(vertices, sizeof(vertices)), &vertexLayout, BGFX_BUFFER_NONE);
	bgfx_index_buffer_handle_t ibh = bgfx_create_index_buffer(bgfx_make_ref(indices, sizeof(indices)), BGFX_BUFFER_NONE);
	
	bgfx_shader_handle_t vertexShaderHnd = load_shader("vs_cubes.bin");
	bgfx_shader_handle_t fragmentShaderHnd = load_shader("fs_cubes.bin");
	bgfx_program_handle_t program = bgfx_create_program(vertexShaderHnd, fragmentShaderHnd, true);
	
	unsigned int counter = 0;
	
	// FPS
	int milliPeriod = (int)(1.0 / (double)FPS * 1000);
	Uint32 lastTick;
	Uint32 currentTick;
	
	bool running = true;
	SDL_Event event;	
	while (running) {
		lastTick = SDL_GetTicks();
		
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
		
		const struct Vec3 at = vec3Zero;
		const struct Vec3 eye = { 0.0f, 0.0f, -5.0f };
		
		float view[16];
		mtx_look_at(view, &eye, &at);
		
		float proj[16];
		mtx_proj(proj, 60.0f, WIDTH_F / HEIGHT_F, 0.1f, 100.0f, bgfx_get_caps()->homogeneousDepth);
		
		bgfx_set_view_transform(0, view, proj);
		
		float mtx[16];
		mtx_rotate_xy(mtx, counter * 0.01f, counter * 0.01f);
		counter++;
		bgfx_set_transform(mtx, 1);
		
		bgfx_set_vertex_buffer(0, vbh, 0, sizeof(vertices) / sizeof(struct PosColorVertex));
		bgfx_set_index_buffer(ibh, 0, sizeof(indices) / sizeof(uint16_t));
		
		bgfx_submit(0, program, 0, BGFX_DISCARD_ALL);
		bgfx_touch(0);
		bgfx_frame(false);
		
		currentTick = SDL_GetTicks();
		int sleep = milliPeriod - (currentTick - lastTick);
		if (sleep > 0) SDL_Delay(sleep);
	}

	deinitialize();
	
	return 0;
}
