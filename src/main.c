#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <bgfx/c99/bgfx.h>

#include "sys/init.h"
#include "math/HMM_math.h"
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
	programs_init(1);
	models_init(1);
	
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
	
	// FPS
	int milliPeriod = (int)(1.0 / (double)FPS * 1000);
	Uint32 lastTick;
	Uint32 currentTick;
	
	// view related
	const float moveSpeed = 0.1f;
	const float rotationSpeed = 20.0f;
	HMM_Vec3 playerPos = HMM_V3(0.0f, 0.0f, -5.0f);
	HMM_Vec2 lookInput = HMM_V2(0.0f, 0.0f);
	
	// input
	const float widthRec = 1 / WIDTH_F;
	const float heightRec = 1 / HEIGHT_F;
	bool wDown;
	bool aDown;
	bool sDown;
	bool dDown;

	// temp
	size_t counter = 0;
	
	bool running = true;
	SDL_Event event;
	
	puts(AC_MAGENTA "[GAME LOOP START]" AC_RESET);
	while (running) {
		lastTick = SDL_GetTicks();
		bgfx_dbg_text_clear(0, false);
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_KEYDOWN: {
					switch (event.key.keysym.sym) {
						case SDLK_UP:
						case SDLK_w: wDown = true; break;
						case SDLK_DOWN:
						case SDLK_s: sDown = true; break;
						case SDLK_LEFT:
						case SDLK_a: aDown = true; break;
						case SDLK_RIGHT:
						case SDLK_d: dDown = true; break;
					}
				} break;
				case SDL_KEYUP: {
					switch (event.key.keysym.sym) {
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
				} break;
				case SDL_MOUSEBUTTONDOWN: {
					SDL_MouseButtonEvent* e = (SDL_MouseButtonEvent*)&event;
					if (e->button == SDL_BUTTON_LEFT) SDL_SetRelativeMouseMode(SDL_TRUE);
				} break;
				// case SDL_MOUSEBUTTONUP: break;
				case SDL_MOUSEMOTION: {
					if (SDL_GetRelativeMouseMode()) {
						SDL_MouseMotionEvent* e = (SDL_MouseMotionEvent*)&event;
						
						lookInput.X += e->yrel * widthRec * rotationSpeed;
						lookInput.Y += e->xrel * heightRec * rotationSpeed;
					}
				} break;
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
		
		// process input when the mouse is locked in the window
		if (SDL_GetRelativeMouseMode()) {
			HMM_Vec3 move = ZERO_V3;
			
			if (wDown) move.Z += 1.0f;
			if (sDown) move.Z -= 1.0f;
			if (aDown) move.X -= 1.0f;
			if (dDown) move.X += 1.0f;
			
			if (!HMM_Eq(move, ZERO_V3)) playerPos = HMM_Add(playerPos, HMM_Mul(HMM_NormV3(move), moveSpeed));
		}
	
		bgfx_dbg_text_printf(0, 1, 0x0f, "%f %f", lookInput.X, lookInput.Y);

		// calculate view and projection matrix
		{
			float view[16];
			const HMM_Vec3 v/*iew*/ = HMM_NormV3(HMM_Sub(ZERO_V3, playerPos));
	
			HMM_Vec3 right = ZERO_V3;
			HMM_Vec3 up = ZERO_V3;
			
			const HMM_Vec3 uxv = HMM_Cross(UP_V3, v/*iew*/);
			
			right = HMM_DotV3(uxv, uxv) == 0.0f ? LEFT_V3 : HMM_NormV3(uxv);
			up = HMM_Cross(v/*iew*/, right);
			
			view[0] = right.X;
			view[1] = up.X;
			view[2] = v/*iew*/.X;
			view[3] = 0.0f;
			
			view[4] = right.Y;
			view[5] = up.Y;
			view[6] = v/*iew*/.Y;
			view[7] = 0.0f;
			
			view[8] = right.Z;
			view[9] = up.Z;
			view[10] = v/*iew*/.Z;
			view[11] = 0.0f;
			
			view[12] = -HMM_DotV3(right, playerPos);
			view[13] = -HMM_DotV3(up, playerPos);
			view[14] = -HMM_DotV3(v/*iew*/, playerPos);
			view[15] = 1.0f;

			// TODO: rotate view
			
			HMM_Mat4 proj = HMM_Perspective_LH_ZO(90.0f, WIDTH_F / HEIGHT_F, 0.1f, 100.0f);
			bgfx_set_view_transform(0, view, &proj);
			bgfx_set_view_rect(0, 0, 0, WIDTH, HEIGHT);
		}

		bgfx_encoder_t* encoder = bgfx_encoder_begin(true);
		bgfx_encoder_touch(encoder, 0);

		// render objects START

		const float angle = counter * 0.01f;
		HMM_Mat4 cubeTr = HMM_M4D(1.0f);
		cubeTr = HMM_Mul(HMM_Rotate_LH(angle, AXIS_X), HMM_Rotate_LH(angle, AXIS_Y));
		cube.transform = cubeTr;

		counter++;
		obj_encoder_render(encoder, &cube);
		
		// render objects END
		
		bgfx_encoder_end(encoder);
		
		bgfx_frame(false);
		
		currentTick = SDL_GetTicks();
		int sleep = milliPeriod - (currentTick - lastTick);
		if (sleep > 0) SDL_Delay(sleep);
	}
	
	puts(AC_MAGENTA "[SHUTTING DOWN]" AC_RESET);

	puts(AC_MAGENTA "Cleaning up models..." AC_RESET);
	models_cleanup();
	puts(AC_MAGENTA "Cleaning up programs..." AC_RESET);
	programs_cleanup();
	
	sys_cleanup();
	puts(AC_MAGENTA "DONE" AC_RESET);
	
	return 0;
}
