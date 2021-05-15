#include "Texture2D.h"

Texture2D::Texture2D(uint32_t width, uint32_t height, GLenum tex_type)
{
	m_size = glm::uvec2(width, height);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	glTextureStorage2D(m_id, 1, tex_type, width, height);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_id);
}
