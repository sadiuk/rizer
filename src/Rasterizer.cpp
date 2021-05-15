#include "Rasterizer.h"

Rasterizer::Rasterizer(RasterizationParams params) : m_params(params)
{
	m_context = GLContext::Get();
	m_raster_params_ubo = UBO::Create(&params, sizeof(params));
	//TODO: relative path
	m_program = ComputeProgram::CreateProgramFromFile("D:/dev/Rasterizer/src/glsl/rasterizer.comp");
}

void Rasterizer::Rasterize(Texture2D* out_tex)
{
	m_context->BindComputeProgram(m_program.get());
	m_context->BindUniformBlock(m_raster_params_ubo.get(), 0);
	m_context->BindTexture2D(out_tex, 0);
	m_context->Dispatch(m_params.texture_width / 16 + 1, m_params.texture_height / 16 + 1, 1);

}

