#include "Buffer.h"

SSBO::SSBO(void* data, uint32_t size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	glBufferData(m_id, size, data, GL_STATIC_DRAW);
}
