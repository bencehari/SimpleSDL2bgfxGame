#ifndef KE_DEBUG_H
#define KE_DEBUG_H

#include "../math/math.h"

namespace dbg {
	/**
	 * @brief Prints 4x4 matric to the screen.
	 *
	 * printf-like wrapper for bgfx::dbgTextPrintf.
	 *
	 * @param _m Matrix4x4 for printing.
	*/
	extern void printToScreen(const char* _format, ...);

	/**
	 * @brief Prints 4x4 matric to the screen.
	 *
	 * @param _m Matrix4x4 for printing.
	 * @param _name Optional string.
	*/
	extern void m4x4ToScreen(const Matrix4x4* _m, const char* _name);

	/**
	 * @brief Prints 4x4 matric to the console.
	 *
	 * @param _m Matrix4x4 for printing.
	*/
	extern void m4x4ToConsole(const Matrix4x4* _m);

	/**
	 * @brief Spacer for screen debug.
	 *
	 * @param _count Count of how many row should be skipped.
	*/
	extern void space(int _count);

	/**
	 * @brief Must call this on every frame's start.
	 *
	 * Resets debug internal states.
	*/
	extern void reset(void);
}

#endif // KE_DEBUG_H
