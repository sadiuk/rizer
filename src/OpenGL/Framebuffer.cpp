#include "Framebuffer.h"
#include <cassert>

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_id);
}

void Framebuffer::AttachTexture(Texture2D* attachment)
{
	m_size = attachment->GetSize();
	glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, attachment->GetId(), 0);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
