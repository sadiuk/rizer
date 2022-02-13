#include "Buffer.h"

Buffer::Buffer(void* data, uint32_t size) : m_size(size)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}

void Buffer::Update(void* data, uint32_t size)
{
	m_size = size;
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}
