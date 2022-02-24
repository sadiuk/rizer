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
	VertexBufferLayout vertex_buffer_layout;
};

struct ViewFrustumPlanes
{
	// left, right, top, bottom, near, far
	glm::vec4 planes[6];
};

struct RasterizationParams_new
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	ViewFrustumPlanes planes;
};

struct alignas(16) RasterizationDynamicParams
{
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;
	uint32_t texture_width, texture_height;
	glm::vec4 alignas(16) color = glm::vec4(0, 1, 1, 1); // The default color in case vertex_buffer_layout is POSR8G8B8A8
	glm::vec3 alignas(16) clear_color = glm::vec3(1, 1, 1);

	uint32_t enable_depth_test = 1;
	uint32_t update_depth_buffer = 1;
};


class Rasterizer
{
public:
	struct InputParams
	{
		SSBO* vertex_buffer;
		SSBO* index_buffer;
		SSBO* triangle_setup_buffer;;
		Texture2D* out_tex;
		AtomicCounterBuffer* atomics;
		UBO* uniforms;
		RasterizationParams_new raster_params;

	};
	Rasterizer();
	~Rasterizer() = default;

	void Rasterize(const InputParams& params);
private:
	void CompileShaderWithStaticParams();
private:
	GLContext* m_context;

	std::shared_ptr<UBO> m_raster_params_ubo;
	std::shared_ptr<ComputeProgram> m_triangleSetupProgram;
	std::shared_ptr<ComputeProgram> m_binRasterizerProgram;
};