#include "Rasterizer.h"

Rasterizer::Rasterizer()
{
	m_context = GLContext::Get();
	//TODO: relative path
	CompileShaderWithStaticParams();
}

void Rasterizer::CompileShaderWithStaticParams()
{
	m_triangleSetupProgram = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/triangle_setup.comp");
	m_binRasterizerProgram = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/bin_rasterizer.comp");
	m_coarseRasterizerProgram = ComputeProgram::CreateProgramFromFile("C:/dev/rizer/src/glsl/coarse_rasterizer.comp");
}

void Rasterizer::Rasterize(const InputParams& params)
{
	static constexpr uint32_t ZERO = 0;
	m_context->ClearBuffer(params.atomics.get(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, (void*) & ZERO);
	m_context->ClearBuffer(params.perBinTriangleIndices.get(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, (void*) & ZERO);
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

	static constexpr size_t atomicCount = Rasterizer::InputParams::atomicBufferSize / sizeof(uint32_t);
	uint32_t atomics[atomicCount] = {};
	m_context->GetBufferSubData(params.atomics.get(), 0, Rasterizer::InputParams::atomicBufferSize, (void*) & atomics);
	params.perBinTriangleIndices->Update(nullptr, atomics[1] * Rasterizer::InputParams::binCount * sizeof(uint32_t));
	m_context->BindSSBO(params.triangleSetupBuffer.get(), 0);
	//TODO: Bind snapped triangles
	m_context->BindSSBO(params.perBinTriangleIndices.get(), 2);
	m_context->BindAtomicCounterBuffer(params.atomics.get(), 3);
	m_context->BindTexture2D(params.binRasterizerOutTex.get(), 4);
	m_context->BindComputeProgram(m_binRasterizerProgram.get());
	m_context->Dispatch(1, 1, 1); // TODO optimal dispatch count
	m_context->PipelineBarrier(GL_ALL_BARRIER_BITS);

	//m_context->BindComputeProgram(m_coarseRasterizerProgram.get());
	//m_context->BindSSBO(params.triangleSetupBuffer.get(), 3);
	//m_context->BindSSBO(params.perBinTriangleIndices.get(), 2);
	//m_context->BindAtomicCounterBuffer(params.atomics.get(), 3);
	//m_context->BindTexture2D(params.coarseRasterizerOutTex.get(), 4);
	//m_context->Dispatch(1, 1, 1); // TODO optimal dispatch count
	//m_context->PipelineBarrier(GL_ALL_BARRIER_BITS);

}

