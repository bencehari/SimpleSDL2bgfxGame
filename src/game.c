#include "game.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <bgfx/c99/bgfx.h>

#include "math/math.h"
#include "sys/loaders.h"
#include "core/programs.h"
#include "core/vertex.h"
#include "core/models.h"
#include "core/transform.h"
#include "core/object.h"

#include "utils/consc.h"
#include "utils/debug.h"

static struct Object createCube(bgfx_program_handle_t _programHandle);

void game(float width, float height, float fps) {
	vertex_init();
	programs_init(1);
	models_init(2);
	
	struct Transform camera = TRANSFORM_NEW(V3_NEW(0.0f, 0.0f, -5.0f), Q_IDENTITY, V3_ONE);

	// init assets
	bgfx_program_handle_t programHandle = program_create("vs_cubes.bin", "fs_cubes.bin", true);
	
	struct Object cube = createCube(programHandle);
	
	// test
	struct Model* suzanneModel = NULL;
	load_external_obj_model("assets/models/suzanne.obj", &vertexLayout, &suzanneModel, INDICES_ORDER_AUTO);
	
	struct Object suzanne = OBJECT_NEW(suzanneModel, programHandle);
	suzanne.transform.position = V3_NEW(5.0f, 0.0f, 0.0f);
	
	// FPS
	int milliPeriod = (int)(1.0 / (double)fps * 1000);
	Uint32 lastTick;
	Uint32 currentTick;
	
	// view related
	const float moveSpeed = 0.1f;
	const float rotationSpeed = 3.0f;
	
	// input
	const float widthNorm = 1.0f / width;
	const float heightNorm = 1.0f / height;
	bool
		forwardDown = false,
		backDown = false,
		leftDown = false,
		rightDown = false,
		elevateDown = false,
		descendDown = false;
	bool
		freeMove = true,
		upDownMove = true;

	// system
	bool running = true;
	SDL_Event event;
	
	puts(AC_MAGENTA "[GAME LOOP START]" AC_RESET);
	while (running) {
		dbg_reset();
		
		lastTick = SDL_GetTicks();
		bgfx_dbg_text_clear(0, false);
		
		Vector2 cameraRotation = {0};
		
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
						case SDLK_e: elevateDown = true; break;
						case SDLK_q: descendDown = true; break;
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
						case SDLK_e: elevateDown = false; break;
						case SDLK_q: descendDown = false; break;
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
						
						cameraRotation = V2_NEW(
							-DEG_TO_RAD(e->yrel * widthNorm * 90.0f * rotationSpeed),
							-DEG_TO_RAD(e->xrel * heightNorm * 90.0f * rotationSpeed)
						);
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
		
		// TODO: limit X rotation to avoid flipping
		tr_fps_rotate(&camera, cameraRotation);
		
		// process input when the mouse is locked in the window
		if (SDL_GetRelativeMouseMode()) {
			Vector3 move = V3_ZERO;
			
			if (forwardDown || backDown) {
				if (forwardDown != backDown) {
					Vector3 fwd = tr_get_forward(&camera);
					
					if (freeMove) move = forwardDown ? fwd : V3_MUL_F(fwd, -1.0f);
					else {
						float Y = move.Y;
						move = forwardDown ? fwd : V3_MUL_F(fwd, -1.0f);
						move.Y = Y;
					}
				}
			}
			if (leftDown || rightDown) {
				if (leftDown != rightDown) {
					Vector3 right = tr_get_right(&camera);
					
					if (freeMove) move = V3_ADD(move, rightDown ? right : V3_MUL_F(right, -1.0f));
					else {
						float Y = move.Y;
						move = V3_ADD(move, rightDown ? right : V3_MUL_F(right, -1.0f));
						move.Y = Y;
					}
				}
			}
			
			if (upDownMove) {
				if (elevateDown || descendDown) {
					if (elevateDown != descendDown) {
						if (freeMove) {
							Vector3 up = tr_get_up(&camera);
							move = V3_ADD(move, elevateDown ? up : V3_MUL_F(up, -1.0f));
						}
						else {
							move = V3_ADD(move, elevateDown ? V3_UP : V3_MUL_F(V3_UP, -1.0f));
						}
					}
				}
			}

			if (!V3_EQ(move, V3_ZERO)) {
				if (!freeMove) move = V3_NORM(move);
				camera.position = V3_ADD(camera.position, V3_MUL_F(move, moveSpeed));
			}
		}
		
		dbg_print_to_screen("CAM POSITION: %.3f %.3f %.3f", camera.position.X, camera.position.Y, camera.position.Z);

		// calculate view and projection matrix
		{
			Matrix4x4 view = LOOK_AT(camera.position, tr_get_look_at(&camera));
			
			Matrix4x4 proj = PERSPECTIVE(DEG_TO_RAD(90.0f), width / height, 0.1f, 100.0f);
			bgfx_set_view_transform(0, &view, &proj);
			bgfx_set_view_rect(0, 0, 0, (int)width, (int)height);
		}

		bgfx_encoder_t* encoder = bgfx_encoder_begin(true);
		bgfx_encoder_touch(encoder, 0);

		// RENDER objects START
		
		obj_encoder_render(encoder, &cube);
		obj_encoder_render(encoder, &suzanne);
		
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

static struct Object createCube(bgfx_program_handle_t _programHandle) {
	struct Model* pCubeModel = model_create(
		(struct Vertex[]) {
			VERTEX_NEW(-1.0f,  1.0f,  1.0f, 0xff000000),
			VERTEX_NEW( 1.0f,  1.0f,  1.0f, 0xff0000ff),
			VERTEX_NEW(-1.0f, -1.0f,  1.0f, 0xff00ff00),
			VERTEX_NEW( 1.0f, -1.0f,  1.0f, 0xff00ffff),
			VERTEX_NEW(-1.0f,  1.0f, -1.0f, 0xffff0000),
			VERTEX_NEW( 1.0f,  1.0f, -1.0f, 0xffff00ff),
			VERTEX_NEW(-1.0f, -1.0f, -1.0f, 0xffffff00),
			VERTEX_NEW( 1.0f, -1.0f, -1.0f, 0xffffffff),
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
	
	return OBJECT_NEW(pCubeModel, _programHandle);
}
