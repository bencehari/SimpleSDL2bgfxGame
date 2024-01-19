#include "game.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <bgfx/c99/bgfx.h>

#include "math/HMM_include.h"
#include "core/programs.h"
#include "core/vertex.h"
#include "core/models.h"
#include "core/object.h"

#include "utils/consc.h"

struct Object createCube();

void game(float width, float height, float fps) {
	vertex_init();
	programs_init(1);
	models_init(1);
	
	struct Object cube = createCube();
	
	// FPS
	int milliPeriod = (int)(1.0 / (double)fps * 1000);
	Uint32 lastTick;
	Uint32 currentTick;
	
	// view related
	const float moveSpeed = 0.1f;
	const float rotationSpeed = 1.0f;
	Vec3 playerPos = VEC3_CTOR(0.0f, 0.0f, -5.0f);
	Vec2 lookRotation = VEC2_CTOR(0.0f, 0.0f);
	
	// input
	const float widthRec = 1 / width;
	const float heightRec = 1 / height;
	bool
		forwardDown = false,
		backDown = false,
		leftDown = false,
		rightDown = false;

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
						case SDLK_w: forwardDown = true; break;
						case SDLK_DOWN:
						case SDLK_s: backDown = true; break;
						case SDLK_LEFT:
						case SDLK_a: leftDown = true; break;
						case SDLK_RIGHT:
						case SDLK_d: rightDown = true; break;
					}
				} break;
				case SDL_KEYUP: {
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE: SDL_SetRelativeMouseMode(SDL_FALSE); break;
						case SDLK_UP:
						case SDLK_w: forwardDown = false; break;
						case SDLK_DOWN:
						case SDLK_s: backDown = false; break;
						case SDLK_LEFT:
						case SDLK_a: leftDown = false; break;
						case SDLK_RIGHT:
						case SDLK_d: rightDown = false; break;
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
						
						lookRotation.X += e->yrel * widthRec * 360.0f * rotationSpeed * DEG_TO_RAD;
						lookRotation.Y += e->xrel * heightRec * 360.0f * rotationSpeed * DEG_TO_RAD;
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

		// TODO: gimbal lock problem, if playerPos changed by this, let's learn quaternions
		// X = pitch, Y = yaw
		const Vec3 forward = VEC3_CTOR(
			cos(lookRotation.X) * sin(lookRotation.Y),
			-sin(lookRotation.X),
			cos(lookRotation.X) * cos(lookRotation.Y)
		);
		
		// process input when the mouse is locked in the window
		if (SDL_GetRelativeMouseMode()) {
			Vec3 move = ZERO_V3;
			
			if (forwardDown) move.Z += 1.0f;
			if (backDown) move.Z -= 1.0f;
			if (leftDown) move.X -= 1.0f;
			if (rightDown) move.X += 1.0f;
			
			if (!EQ(move, ZERO_V3)) playerPos = ADD(playerPos, MUL(NORM_V3(move), moveSpeed));
		}
		
		int dbgTextIdx = 1;
		bgfx_dbg_text_printf(0, dbgTextIdx++, 0x0f, "POSITION: %.3f %.3f %.3f", playerPos.X, playerPos.Y, playerPos.Z);
		bgfx_dbg_text_printf(0, dbgTextIdx++, 0x0f, "   MOUSE: %.3f %.3f", lookRotation.X, lookRotation.Y);
		bgfx_dbg_text_printf(0, dbgTextIdx++, 0x0f, " FORWARD: %.3f %.3f %.3f", forward.X, forward.Y, forward.Z);

		// calculate view and projection matrix
		{
			Mat4 view = LOOK_AT(playerPos, ADD(playerPos, forward));
			MAT4_print_to_screen(&view, ++dbgTextIdx, NULL); dbgTextIdx += 5;
			
			Mat4 proj = PERSPECTIVE(90.0f, width / height, 0.1f, 100.0f);
			bgfx_set_view_transform(0, &view, &proj);
			bgfx_set_view_rect(0, 0, 0, (int)width, (int)height);

			bgfx_encoder_t* encoder = bgfx_encoder_begin(true);
			bgfx_encoder_touch(encoder, 0);
		}

		// RENDER objects START

		const float angle = counter++ * 0.01f;

		cube.transform = MUL(ROT(angle, AXIS_X), ROT(angle, AXIS_Y));
		MAT4_print_to_screen(&cube.transform, ++dbgTextIdx, "cube"); dbgTextIdx += 5;
		obj_encoder_render(encoder, &cube);
		
		// RENDER objects END
		
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
}

// ~~~~~

struct Object createCube() {
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
	
	return OBJECT_CTOR(pCubeModel, cubeProgHnd);
}
