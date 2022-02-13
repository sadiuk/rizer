#pragma once 
#include <cstdint>
#include <memory>

#include <glad/glad.h>


class Buffer
{
	GLuint m_id;
	size_t m_size;
	Buffer(void* data, uint32_t size);
public:
	[[nodiscard]] GLuint GetId() const { return m_id; }
	[[nodiscard]] GLuint GetSize() const { return m_size; }
	static std::shared_ptr<Buffer> Create(void* data, uint32_t size)
	{
		return std::shared_ptr<Buffer>(new Buffer(data, size));
	}
	void Update(void* data, uint32_t size);
	~Buffer();
};

using SSBO = Buffer;
using UBO = Buffer;
using AtomicCounterBuffer = Buffer;

