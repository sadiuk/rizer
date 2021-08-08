#include "Rasterizer.h"

Rasterizer::Rasterizer(const RasterizationParams& params) : m_static_params(params)
{
	m_context = GLContext::Get();
	//TODO: relative path
	m_program = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/rasterizer.comp");
}

void Rasterizer::Rasterize(SSBO* vertex_buffer, SSBO* index_buffer, Texture2D* out_tex, const RasterizationDynamicParams& params, SSBO* depth_buffer)
{
	m_dynamic_params = params;
	if (m_raster_params_ubo == nullptr)
	{
		m_raster_params_ubo = UBO::Create(&m_dynamic_params, sizeof(m_dynamic_params));
	}
	else
	{
		m_raster_params_ubo->Update(&m_dynamic_params, sizeof(m_dynamic_params));
	}
	m_context->BindComputeProgram (m_program.get());
	m_context->BindUniformBlock(m_raster_params_ubo.get(), 0);
	m_context->BindTexture2D(out_tex, 0);
	m_context->BindSSBO(vertex_buffer, 1);
	m_context->BindSSBO(index_buffer, 2);
	m_context->BindSSBO(depth_buffer, 3);
	m_context->Dispatch(m_dynamic_params.texture_width / 16 + 1, m_dynamic_params.texture_height / 16 + 1, 1);
	m_context->PipelineBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Rasterizer::SetRasterizationParams(const RasterizationParams& params)
{
	m_static_params = params;
}

