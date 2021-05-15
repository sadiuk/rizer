#pragma once
#include <glad/glad.h>

#include "Texture2D.h"
class Framebuffer
{
private:
	GLuint m_id;
	glm::uvec2 m_size;
	Framebuffer();
public:
	[[nodiscard]] GLuint GetId() const { return m_id; }
	[[nodiscard]] glm::uvec2 GetSize() const { return m_size; }
	static std::shared_ptr<Framebuffer> Create()
	{
		return std::shared_ptr<Framebuffer>(new Framebuffer());
	}
	void AttachTexture(Texture2D* attachment);
};