#include "Rasterizer.h"
#include <numeric>
#include <thread>
#include "Debug.h"
Rasterizer::Rasterizer()
{
	m_context = GLContext::Get();
	//TODO: relative path
	CompileShaderWithStaticParams();
}

void Rasterizer::CompileShaderWithStaticParams()
{
	m_triangleSetupProgram = ComputeProgram::CreateProgramFromFile("D:/dev/rizer/src/glsl/triangle_setup.comp");
	m_binRasterizerProgram = ComputeProgram::CreateProgramFromFile("D:/dev/rizer/src/glsl/bin_rasterizer.comp");
	m_coarseRasterizerProgram = ComputeProgram::CreateProgramFromFile("D:/dev/rizer/src/glsl/coarse_rasterizer.comp");
	m_fineRasterizerProgram = ComputeProgram::CreateProgramFromFile("D:/dev/rizer/src/glsl/fine_rasterizer.comp");
}

void Rasterizer::Rasterize(const InputParams& params)
{
	//Triangle Setup
	
	TEST_TIMER_START("Buffer Clearing") 
	static constexpr uint32_t ZERO = 0;
	m_context->ClearBuffer(params.atomics.get(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, (void*) & ZERO);
	m_context->ClearBuffer(params.perTileTriangleCount.get(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, (void*) & ZERO);
	m_context->ClearBuffer(params.perBinTriangleIndices.get(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, (void*) & ZERO);
	m_context->ClearBuffer(params.currentTileCounter.get(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, (void*) & ZERO);
	TEST_TIMER_END()

	TEST_TIMER_START("Triangle setup")
	m_context->BindComputeProgram (m_triangleSetupProgram.get());
	m_context->BindUniformBlock(params.uniforms.get(), 0);
	m_context->BindSSBO(params.vertexBuffer.get(), 1);
	m_context->BindSSBO(params.indexBuffer.get(), 2);
	m_context->BindSSBO(params.triangleSetupBuffer.get(), 3);
	m_context->BindAtomicCounterBuffer(params.atomics.get(), 5);

	//TODO Compute optimal sizes
	m_context->Dispatch(1, 1, 1);
	m_context->PipelineBarrier(GL_ALL_BARRIER_BITS);
	m_context->Flush();

	TEST_TIMER_END()

	TEST_TIMER_START("Buffer updates for bin rasterizer")
	////Bin Rasterizer
	static constexpr size_t atomicCount = Rasterizer::InputParams::atomicBufferSize / sizeof(uint32_t);
	uint32_t atomics[atomicCount] = {};
	m_context->GetBufferSubData(params.atomics.get(), 0, Rasterizer::InputParams::atomicBufferSize, (void*) atomics);
	params.perBinTriangleIndices->Update(nullptr, atomics[1] * Rasterizer::InputParams::binCount * sizeof(uint32_t));
	m_context->PipelineBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	m_context->PipelineBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
	m_context->Flush();

	TEST_TIMER_END()

	TEST_TIMER_START("Bin rasterizer")
	m_context->BindSSBO(params.triangleSetupBuffer.get(), 0);
	//TODO: Bind snapped triangles
	m_context->BindSSBO(params.perBinTriangleIndices.get(), 2);
	m_context->BindAtomicCounterBuffer(params.atomics.get(), 3);
	m_context->BindTexture2D(params.binRasterizerOutTex.get(), 4);
	m_context->BindComputeProgram(m_binRasterizerProgram.get());
	m_context->Dispatch(1, 1, 1); // TODO optimal dispatch count
	m_context->PipelineBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	m_context->PipelineBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	m_context->PipelineBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
	m_context->Flush();

	TEST_TIMER_END()

	TEST_TIMER_START("Buffer updates for coarse rasterizer")
	////Coarse Rasterizer
	uint32_t perBinTriangleCount[Rasterizer::InputParams::binCount] = {};
	uint32_t perBinTriangleCountPrefixSum[Rasterizer::InputParams::binCount] = {};
	m_context->GetBufferSubData(params.atomics.get(), 24, Rasterizer::InputParams::binCount * sizeof(uint32_t), (void*) perBinTriangleCount);
	std::partial_sum(perBinTriangleCount, perBinTriangleCount + Rasterizer::InputParams::binCount, perBinTriangleCountPrefixSum);
	params.perBinTriangleCountPrefixSum->Update(perBinTriangleCountPrefixSum, Rasterizer::InputParams::binCount * sizeof(uint32_t));
	params.perTileTriangleIndices->Update(nullptr, perBinTriangleCountPrefixSum[Rasterizer::InputParams::binCount - 1] * Rasterizer::InputParams::tileCount * sizeof(uint32_t));
	m_context->Flush();

	TEST_TIMER_END()

	TEST_TIMER_START("Coarse rasterizer")

	m_context->BindComputeProgram(m_coarseRasterizerProgram.get());
	m_context->BindSSBO(params.triangleSetupBuffer.get(), 0);
	m_context->BindSSBO(params.perBinTriangleIndices.get(), 2);
	m_context->BindAtomicCounterBuffer(params.atomics.get(), 3);
	m_context->BindTexture2D(params.coarseRasterizerOutTex.get(), 4);
	m_context->BindSSBO(params.perTileTriangleCount.get(), 5);
	m_context->BindSSBO(params.perBinTriangleCountPrefixSum.get(), 6);
	m_context->BindSSBO(params.perTileTriangleIndices.get(), 7);
	m_context->Dispatch(20, 1, 1); // TODO optimal dispatch count
	m_context->PipelineBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	m_context->PipelineBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	m_context->PipelineBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
	m_context->Flush();
	TEST_TIMER_END()

	TEST_TIMER_START("Fine rasterizer")
	//Fine Rasterizer
	m_context->BindComputeProgram(m_fineRasterizerProgram.get());
	m_context->BindSSBO(params.triangleSetupBuffer.get(), 0);
	m_context->BindAtomicCounterBuffer(params.currentTileCounter.get(), 2);
	m_context->BindTexture2D(params.fineRasterizerOutTex.get(), 3);
	m_context->BindSSBO(params.perTileTriangleCount.get(), 4);
	m_context->BindSSBO(params.perBinTriangleCountPrefixSum.get(), 5);
	m_context->BindSSBO(params.perTileTriangleIndices.get(), 6);
	m_context->Dispatch(128, 1, 1); // TODO optimal dispatch count
	m_context->PipelineBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	m_context->PipelineBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	m_context->PipelineBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
	m_context->PipelineBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
	m_context->Flush();
	TEST_TIMER_END()

}

