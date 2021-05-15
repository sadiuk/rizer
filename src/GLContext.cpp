#include "GLContext.h"


void GLContext::BindUniformBlock(UBO* ubo, uint32_t binding)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo->GetId());
}

void GLContext::BindTexture2D(Texture2D* tex, uint32_t slot)
{
	glBindImageTexture(slot, tex->GetId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
}

void GLContext::BindSSBO(SSBO* ssbo, uint32_t slot)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, ssbo->GetId());
}

void GLContext::BindComputeProgram(ComputeProgram* program)
{
	glUseProgram(program->GetId());
}

void GLContext::Dispatch(uint32_t x_wg_count, uint32_t y_wg_count, uint32_t z_wg_count)
{
	glDispatchCompute(x_wg_count, y_wg_count, z_wg_count);
}

void GLContext::BlitTexture(Texture2D* tex)
{
}
