#include "Rasterizer.h"

Rasterizer::Rasterizer(const RasterizationParams& params) : m_params(params)
{
	m_context = GLContext::Get();
	m_raster_params_ubo = UBO::Create(&m_params, sizeof(m_params));
	//TODO: relative path
	m_program = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/rasterizer.comp");
}

void Rasterizer::Rasterize(SSBO* vertex_buffer, SSBO* index_buffer, Texture2D* out_tex)
{
	m_context->BindComputeProgram (m_program.get());
	m_context->BindUniformBlock(m_raster_params_ubo.get(), 0);
	m_context->BindTexture2D(out_tex, 0);
	m_context->BindSSBO(vertex_buffer, 1);
	m_context->BindSSBO(index_buffer, 2);
	m_context->Dispatch(m_params.texture_width / 16 + 1, m_params.texture_height / 16 + 1, 1);
	m_context->PipelineBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Rasterizer::SetRasterizationParams(const RasterizationParams& params)
{
	m_params = params;
	m_raster_params_ubo = UBO::Create(&m_params, sizeof(m_params));
}

