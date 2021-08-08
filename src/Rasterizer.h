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



struct RasterizationParams
{
	
};

struct alignas(16) RasterizationDynamicParams
{
	uint32_t texture_width, texture_height;
	VertexBufferLayout vertex_buffer_layout;
	glm::vec4 alignas(16) color = glm::vec4(0, 1, 1, 1); // The default color in case vertex_buffer_layout is POSR8G8B8A8
	glm::vec3 alignas(16) clear_color = glm::vec3(1, 1, 1);

	uint32_t enable_depth_test = 1;
	uint32_t update_depth_buffer = 1;
};

class Rasterizer
{
public:
	Rasterizer(const RasterizationParams& params);
	~Rasterizer() = default;

	void Rasterize(SSBO* vertex_buffer, SSBO* index_buffer, Texture2D* out_tex, const RasterizationDynamicParams& params, SSBO* depth_buffer = nullptr);
	void SetRasterizationParams(const RasterizationParams& params);
private:
	RasterizationParams m_static_params;
	RasterizationDynamicParams m_dynamic_params;
	GLContext* m_context;

	std::shared_ptr<UBO> m_raster_params_ubo;
	std::shared_ptr<ComputeProgram> m_program;
};