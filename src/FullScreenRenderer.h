#pragma once


class Texture2D;
class FullScreenRenderer
{
private:
	constexpr static const char* VS_SOURCE = R"(
#version 460 core

layout(location = 0) in vec2 pos;

out vec2 uv;

void main()
{
	uv = pos * 0.5 + 0.5;
	gl_Position = vec4(pos, 0, 1); 	
}
)";
	constexpr static const char* FS_SOURCE = R"(
#version 460 core

in vec2 uv;

out vec4 color;

//Don't wanna setup the sampler just for debugging 
layout(binding = 0, rgba8) uniform image2D tex;

void main()
{
	ivec2 size = imageSize(tex);
	color = imageLoad(tex, ivec2(size * uv));
}
)";

	static constexpr float vertPositions[] = { -1, -1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1 };

	uint32_t m_vboID;
	uint32_t m_vaoID;
	uint32_t m_vertShaderID, m_fragShaderID;
	uint32_t m_programID;
public:
	FullScreenRenderer();
	void RenderTextureFullScreen(Texture2D* tex);
	~FullScreenRenderer();
};