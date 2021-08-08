#include "Buffer.h"

SSBO::SSBO(void* data, uint32_t size)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}

SSBO::~SSBO()
{
	glDeleteBuffers(1, &m_id);
}

UBO::~UBO()
{
	glDeleteBuffers(1, &m_id);
}

UBO::UBO(void* data, uint32_t size)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}

void UBO::Update(void* data, uint32_t size)
{
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}
