#include "Buffer.h"

SSBO::SSBO(void* data, uint32_t size)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}

UBO::UBO(void* data, uint32_t size)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}