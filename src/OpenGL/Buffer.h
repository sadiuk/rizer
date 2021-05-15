#pragma once 
#include <cstdint>
#include <memory>

#include <glad/glad.h>


class SSBO
{
	GLuint m_id;
	SSBO(void* data, uint32_t size);

public:
	[[nodiscard]] GLuint GetId() const { return m_id; }
	static std::shared_ptr<SSBO> Create(void* data, uint32_t size)
	{
		return std::shared_ptr<SSBO>(new SSBO(data, size));
	}
};

class UBO
{
	GLuint m_id;
	UBO(void* data, uint32_t size);
public:
	[[nodiscard]] GLuint GetId() const { return m_id; }
	static std::shared_ptr<UBO> Create(void* data, uint32_t size)
	{
		return std::shared_ptr<UBO>(new UBO(data, size));
	}
};