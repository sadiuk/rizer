#pragma once

#include <glm/glm.hpp>

#include "GLContext.h"
#include "OpenGL/Buffer.h"
struct RasterizationParams
{
	struct alignas(16)
	{
		uint32_t texture_width, texture_height;
	};
};

class Rasterizer
{
public:
	Rasterizer(RasterizationParams params);
	~Rasterizer() = default;

	void Rasterize(Texture2D* out_tex);
private:
	RasterizationParams m_params;
	GLContext* m_context;

	std::shared_ptr<UBO> m_raster_params_ubo;
	std::shared_ptr<ComputeProgram> m_program;
};