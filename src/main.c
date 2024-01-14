#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <bgfx/c99/bgfx.h>

#include "sys/init.h"
#include "math/vector3.h"
#include "math/vector2.h"
#include "math/matrix4.h"
#include "core/programs.h"
#include "core/vertex.h"
#include "core/models.h"
#include "core/object.h"

#include "utils/consc.h"

#define WIDTH 640
#define HEIGHT 480
#define WIDTH_F 640.0
#define HEIGHT_F 480.0
#define FPS 60

void handle_input(void);

void on_key_down(SDL_Keycode _keyCode);
void on_key_up(SDL_Keycode _keyCode);

void on_mouse_button_down(SDL_MouseButtonEvent* _buttonEvent);
void on_mouse_button_up(SDL_MouseButtonEvent* _buttonEvent);
void on_mouse_motion(SDL_MouseMotionEvent* _motionEvent);

static const float moveSpeed = 0.1f;
static struct Vec3 playerPos = { 0.0f, 0.0f, -5.0f };

static const float rotationSpeed = 0.001f;
static struct Vec2 lookInput = { 0.0f, 0.0f };

static bool wDown;
static bool aDown;
static bool sDown;
static bool dDown;

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
	
	vertex_init();
	model_hnd_initialize(1);
	
	struct Model* pCubeModel = model_create(
		(struct Vertex[]) {
			VERTEX_CTOR(-1.0f,  1.0f,  1.0f, 0xff000000),
			VERTEX_CTOR( 1.0f,  1.0f,  1.0f, 0xff0000ff),
			VERTEX_CTOR(-1.0f, -1.0f,  1.0f, 0xff00ff00),
			VERTEX_CTOR( 1.0f, -1.0f,  1.0f, 0xff00ffff),
			VERTEX_CTOR(-1.0f,  1.0f, -1.0f, 0xffff0000),
			VERTEX_CTOR( 1.0f,  1.0f, -1.0f, 0xffff00ff),
			VERTEX_CTOR(-1.0f, -1.0f, -1.0f, 0xffffff00),
			VERTEX_CTOR( 1.0f, -1.0f, -1.0f, 0xffffffff),
		},
		8,
		(uint16_t[]) {
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
		},
		36,
		&vertexLayout);
	
	// model_print(pCubeModel, true, true);
	
	bgfx_program_handle_t cubeProgHnd = program_create("vs_cubes.bin", "fs_cubes.bin", true);
	
	struct Object cube = OBJECT_CTOR(pCubeModel, cubeProgHnd);
	
	size_t counter = 0;
	
	// FPS
	int milliPeriod = (int)(1.0 / (double)FPS * 1000);
	Uint32 lastTick;
	Uint32 currentTick;

	puts(AC_MAGENTA "[GAME LOOP START]" AC_RESET);
	bool running = true;
	SDL_Event event;	
	while (running) {
		lastTick = SDL_GetTicks();
		bgfx_dbg_text_clear(0, false);
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_KEYDOWN: on_key_down(event.key.keysym.sym); break;
				case SDL_KEYUP: on_key_up(event.key.keysym.sym); break;
				case SDL_MOUSEBUTTONDOWN: on_mouse_button_down((SDL_MouseButtonEvent*)&event); break;
				case SDL_MOUSEBUTTONUP: on_mouse_button_up((SDL_MouseButtonEvent*)&event); break;
				case SDL_MOUSEMOTION: on_mouse_motion((SDL_MouseMotionEvent*)&event); break;
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
		
		handle_input();
		
		const struct Vec3 at = ADD_V3_V3(playerPos, FORWARD_V3);
	
		bgfx_dbg_text_printf(0, 1, 0x0f, "%f %f", lookInput.x, lookInput.y);
	
		{
			float view[16];
			mtx_look_at(view, &playerPos, &at);
			
			// TODO: rotate view
			float rotXMtx[16] = IDENTITY_MTX;
			float rotYMtx[16] = IDENTITY_MTX;
			
			mtx_rotate_x(rotXMtx, lookInput.x);
			mtx_rotate_y(rotYMtx, lookInput.y);
			
			mul_mtx_mtx(view, view, rotYMtx);
			mul_mtx_mtx(view, view, rotXMtx);
			
			float proj[16];
			mtx_proj(proj, 60.0f, WIDTH_F / HEIGHT_F, 0.1f, 100.0f, bgfx_get_caps()->homogeneousDepth);
			
			bgfx_set_view_transform(0, view, proj);
			bgfx_set_view_rect(0, 0, 0, WIDTH, HEIGHT);
		}

		bgfx_touch(0);

		// render objects START

		mtx_rotate_xy(cube.transform, counter * 0.01f, counter * 0.01f);
		counter++;
		obj_render(&cube);
		
		// render objects END
		
		bgfx_frame(false);
		
		currentTick = SDL_GetTicks();
		int sleep = milliPeriod - (currentTick - lastTick);
		if (sleep > 0) SDL_Delay(sleep);
	}
	
	puts(AC_MAGENTA "[SHUTTING DOWN]" AC_RESET);

	model_hnd_deinitialize();
	sys_deinitialize();
	
	return 0;
}

void handle_input(void) {
	if (!SDL_GetRelativeMouseMode()) return;
	
	struct Vec3 move = ZERO_V3;
	if (wDown) move.z += 1.0f;
	if (sDown) move.z -= 1.0f;
	if (aDown) move.x -= 1.0f;
	if (dDown) move.x += 1.0f;
	
	if (EQ_V3(move, ZERO_V3)) return;
	
	playerPos = ADD_V3_V3(playerPos, MUL_V3_F(NORM_V3(move), moveSpeed));
}

void on_key_down(SDL_Keycode _keyCode) {
	switch (_keyCode) {
		case SDLK_UP:
		case SDLK_w: wDown = true; break;
		case SDLK_DOWN:
		case SDLK_s: sDown = true; break;
		case SDLK_LEFT:
		case SDLK_a: aDown = true; break;
		case SDLK_RIGHT:
		case SDLK_d: dDown = true; break;
	}
}

void on_key_up(SDL_Keycode _keyCode) {
	switch (_keyCode) {
		case SDLK_ESCAPE: SDL_SetRelativeMouseMode(SDL_FALSE); break;
		case SDLK_UP:
		case SDLK_w: wDown = false; break;
		case SDLK_DOWN:
		case SDLK_s: sDown = false; break;
		case SDLK_LEFT:
		case SDLK_a: aDown = false; break;
		case SDLK_RIGHT:
		case SDLK_d: dDown = false; break;
	}
}

void on_mouse_button_down(SDL_MouseButtonEvent* _buttonEvent) {
	if (_buttonEvent->button == SDL_BUTTON_LEFT) SDL_SetRelativeMouseMode(SDL_TRUE);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void on_mouse_button_up(SDL_MouseButtonEvent* _buttonEvent) {
#pragma GCC diagnostic pop
	
}

void on_mouse_motion(SDL_MouseMotionEvent* _motionEvent) {
	if (!SDL_GetRelativeMouseMode()) return;
	
	// TODO: probably wrong.
	lookInput.x += _motionEvent->yrel * rotationSpeed;
	lookInput.y += _motionEvent->xrel * rotationSpeed;
}
