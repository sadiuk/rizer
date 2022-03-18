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

void GLContext::BindAtomicCounterBuffer(AtomicCounterBuffer* buffer, uint32_t binding)
{
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, binding, buffer->GetId());
}

void GLContext::BindComputeProgram(ComputeProgram* program)
{
	glUseProgram(program->GetId());
}

void GLContext::Dispatch(uint32_t x_wg_count, uint32_t y_wg_count, uint32_t z_wg_count)
{
	glDispatchCompute(x_wg_count, y_wg_count, z_wg_count);
}

void GLContext::BlitFramebuffer(Framebuffer* fbo)
{
	glBlitNamedFramebuffer(fbo->GetId(), 0, 
		0, 0, fbo->GetSize().x, fbo->GetSize().y,
		0, 0, fbo->GetSize().x, fbo->GetSize().y, 
		GL_COLOR_BUFFER_BIT, 
		GL_NEAREST);
}

void GLContext::PipelineBarrier(GLenum barrier)
{
	glMemoryBarrier(barrier);
}

void GLContext::GetBufferSubData(Buffer* buff, size_t offset, size_t size, void* data)
{
	glGetNamedBufferSubData(buff->GetId(), offset, size, data);
}

void GLContext::ClearBuffer(Buffer* buff, int internalFormat, int format, int type, void* data)
{
	glClearNamedBufferData(buff->GetId(), internalFormat, format, type, data);
}

