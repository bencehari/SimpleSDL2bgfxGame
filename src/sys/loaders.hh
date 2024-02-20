#ifndef LOADERS_HH
#define LOADERS_HH

#include <bgfx/bgfx.h>

/**
 * @brief Loads shader.
 *
 * Loads binary data (shipped with bgfx examples) and
 * feeds it to bgfx.
 *
 * @param _filename Path to the shader file.
 * @param _shaderHandle bgfx::ShaderHandle pointer to the created shader.
 *
 * @return False on error, otherwise true.
*/
extern bool loadShader(const char* _filename, bgfx::ShaderHandle* _shaderHandle);

#endif // LOADERS_HH
