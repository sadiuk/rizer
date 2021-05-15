#pragma once 

#include <glad/glad.h>
#include <cstdint>
#include <memory>

class Texture2D
{
public:
	//It's gonna be that easy for now, probably will change the API if necessary
	static std::shared_ptr<Texture2D> CreateEmptyR8G8B8A8_UNORM(int width, int height)
	{
		return std::shared_ptr<Texture2D>(new Texture2D(width, height, GL_RGBA8));
	}

	[[nodiscard]] GLuint GetId() const { return m_id; }
	~Texture2D();
	Texture2D(uint32_t width, uint32_t height, GLenum tex_type);
private:
	GLuint m_id;
};