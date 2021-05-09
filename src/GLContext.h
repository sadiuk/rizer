#pragma once 
#include <memory>

#include "OpenGL/Texture2D.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/ComputeProgram.h"
class GLContext
{
public:
	void BindTexture2D(Texture2D* tex, uint32_t slot);
	void BindSSBO(SSBO* ssbo, uint32_t slot);

	void Dispatch(uint32_t x_wg_count, uint32_t y_wg_count, uint32_t z_wg_count);

	void BlitTexture(Texture2D* tex);


};