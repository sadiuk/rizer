#pragma once

#include <glm/glm.hpp>

#include "GLContext.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/Framebuffer.h"

enum class VertexBufferLayout : uint32_t {
	POSR8G8B8A8 = 1,
	POSR8G8B8A8_COLR8G8B8A8 = 1 << 1,
	POSR8G8B8A8_COLR8G8B8A8_NORMR8G8B8A8 = 1 << 2// Reserved for future
};

struct alignas(16) RasterizationParams
{
	uint32_t texture_width, texture_height;
	VertexBufferLayout vertex_buffer_layout;
	glm::vec4 alignas(16) color = glm::vec4(1, 1, 1, 1); // The default color in case vertex_buffer_layout is POSR8G8B8A8
};

class Rasterizer
{
public:
	Rasterizer(const RasterizationParams& params);
	~Rasterizer() = default;

	void Rasterize(SSBO* vertex_buffer, SSBO* index_buffer, Texture2D* out_tex);
	void SetRasterizationParams(const RasterizationParams& params);
private:
	RasterizationParams m_params;
	GLContext* m_context;

	std::shared_ptr<UBO> m_raster_params_ubo;
	std::shared_ptr<ComputeProgram> m_program;
};