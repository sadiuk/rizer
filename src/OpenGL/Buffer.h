#pragma once 
#include <cstdint>
#include <memory>

#include <glad/glad.h>


class SSBO
{
	GLuint m_id;
	SSBO(void* data, uint32_t size);

public:
	GLuint GetId() const { return m_id; }
	static std::shared_ptr<SSBO> Create(void* data, uint32_t size)
	{
		return std::shared_ptr<SSBO>(new SSBO(data, size));
	}
};