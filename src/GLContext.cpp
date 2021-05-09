#include "GLContext.h"

void GLContext::BindTexture2D(Texture2D* tex, uint32_t slot)
{
	glBindTexture(slot, tex->GetId());
}

void GLContext::BindSSBO(SSBO* ssbo, uint32_t slot)
{
}

void GLContext::Dispatch(uint32_t x_wg_count, uint32_t y_wg_count, uint32_t z_wg_count)
{
}

void GLContext::BlitTexture(Texture2D* tex)
{
}
