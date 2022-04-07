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



struct ViewFrustumPlanes
{
	// left, right, top, bottom, near, far
	glm::vec4 planes[6];
};

struct RasterizationParams
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	ViewFrustumPlanes planes;
};

struct alignas(16) RasterizationDynamicParams_old
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
		InputParams(void* vertexData,
			size_t vertexBufferSize,
			void* indexData,
			size_t indexBufferSize,
			RasterizationParams& params) : rasterParams(params)
		{
			vertexBuffer = SSBO::Create(vertexData, vertexBufferSize);
			indexBuffer = SSBO::Create(indexData, indexBufferSize);
			auto triCount = indexBuffer->GetSize() / 3 / sizeof(uint32_t);
			triangleSetupBuffer = SSBO::Create(nullptr, triCount * sizeof(glm::vec4) * 3);
			binRasterizerOutTex = Texture2D::CreateEmptyR8G8B8A8_UNORM(texSize, texSize);
			coarseRasterizerOutTex = Texture2D::CreateEmptyR8G8B8A8_UNORM(texSize, texSize);
			atomics = AtomicCounterBuffer::Create(nullptr, atomicBufferSize);
			uniforms = UBO::Create((void*)&params, sizeof(params));
			perBinTriangleIndices = SSBO::Create(nullptr, 0);
			perTileTriangleIndices = SSBO::Create(nullptr, 0);
			perBinTriangleCountPrefixSum = SSBO::Create(nullptr, binCount * sizeof(uint32_t));
			perTileTriangleCount = SSBO::Create(nullptr, tileCount * sizeof(uint32_t));
		}

		std::shared_ptr<SSBO> vertexBuffer;
		std::shared_ptr<SSBO> indexBuffer;
		std::shared_ptr<SSBO> triangleSetupBuffer;
		std::shared_ptr<SSBO> perBinTriangleIndices;
		std::shared_ptr<SSBO> perTileTriangleIndices;
		std::shared_ptr<SSBO> perBinTriangleCountPrefixSum;
		std::shared_ptr<SSBO> perTileTriangleCount;
		std::shared_ptr<Texture2D> binRasterizerOutTex;
		std::shared_ptr<Texture2D> coarseRasterizerOutTex;
		std::shared_ptr<AtomicCounterBuffer> atomics;
		std::shared_ptr<UBO> uniforms;
		RasterizationParams rasterParams;
		constexpr static uint16_t binCount = 256;
		constexpr static uint16_t tilesPerBin = 64;
		constexpr static uint16_t tileCount = binCount * tilesPerBin;
		constexpr static uint16_t atomicBufferSize = 24 + 4 * binCount;
		constexpr static uint16_t texSize = 1024;
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
	std::shared_ptr<ComputeProgram> m_coarseRasterizerProgram;
};