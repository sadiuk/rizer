#pragma once 
#include <memory>

#include "OpenGL/Texture2D.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/ComputeProgram.h"
#include "OpenGL/Framebuffer.h"
class GLContext
{
	GLContext() = default;
public:
	static GLContext* Get()
	{
		GLContext ctx;
		return &ctx;
	}

	void BindTexture2D(Texture2D* tex, uint32_t binding);
	void BindSampler2D(Texture2D* tex, uint32_t binding);
	void BindUniformBlock(UBO* ubo, uint32_t binding);
	void BindSSBO(SSBO* ssbo, uint32_t binding);
	void BindAtomicCounterBuffer(AtomicCounterBuffer* buffer, uint32_t binding);
	void BindComputeProgram(ComputeProgram* program);

	void Dispatch(uint32_t x_wg_count, uint32_t y_wg_count, uint32_t z_wg_count);

	void BlitFramebuffer(Framebuffer* fbo);
	
	void PipelineBarrier(GLenum barrier);

	void ClearBuffer(SSBO* buff, int internalFormat, int format, int type, void* data);

	void GetBufferSubData(Buffer* buff, size_t offset, size_t size, void* data);

	void Flush();
};