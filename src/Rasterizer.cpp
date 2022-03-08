#include "Rasterizer.h"

Rasterizer::Rasterizer()
{
	m_context = GLContext::Get();
	//TODO: relative path
	CompileShaderWithStaticParams();
}

void Rasterizer::CompileShaderWithStaticParams()
{
	m_triangleSetupProgram = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/rasterizer_new.comp");
	m_binRasterizerProgram = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/bin_rasterizer.comp");
}

void Rasterizer::Rasterize(const InputParams& params)
{
	//TODO TAKE STRUCT SIZES NOT MEMBER (I'LL REGRET NOT DOING IT NOW)
	m_context->BindComputeProgram (m_triangleSetupProgram.get());
	m_context->BindUniformBlock(params.uniforms.get(), 0);
	m_context->BindSSBO(params.vertexBuffer.get(), 1);
	m_context->BindSSBO(params.indexBuffer.get(), 2);
	m_context->BindSSBO(params.triangleSetupBuffer.get(), 3);
	//
	m_context->BindAtomicCounterBuffer(params.atomics.get(), 5);

	//TODO Compute optimal sizes
	m_context->Dispatch(64, 64, 1);
	m_context->PipelineBarrier(GL_ALL_BARRIER_BITS);

	m_context->BindSSBO(params.triangleSetupBuffer.get(), 0);

}

