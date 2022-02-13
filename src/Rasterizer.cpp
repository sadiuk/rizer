#include "Rasterizer.h"

Rasterizer::Rasterizer()
{
	m_context = GLContext::Get();
	//TODO: relative path
	CompileShaderWithStaticParams();
}

void Rasterizer::CompileShaderWithStaticParams()
{
	m_program = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/rasterizer_new.comp");
}

void Rasterizer::Rasterize(const InputParams& params)
{
	//TODO TAKE STRUCT SIZES NOT MEMBER (I'LL REGRET NOT DOING IT NOW)

	m_context->BindComputeProgram (m_program.get());
	m_context->BindTexture2D(params.out_tex, 0);
	m_context->BindUniformBlock(params.uniforms, 0);
	m_context->BindSSBO(params.vertex_buffer, 1);
	m_context->BindSSBO(params.index_buffer, 2);
	m_context->BindSSBO(params.triangle_setup_buffer, 3);
	//
	m_context->BindAtomicCounterBuffer(params.atomics, 5);

	//TODO Compute optimal sizes
	m_context->Dispatch(64, 64, 1);



	m_context->PipelineBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

