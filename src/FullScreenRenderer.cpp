#include <iostream>
#include "FullScreenRenderer.h"
#include "OpenGL/Texture2D.h"

FullScreenRenderer::FullScreenRenderer()
{
	auto err = glGetError();
	m_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertShaderID, 1, &VS_SOURCE, nullptr);
	glCompileShader(m_vertShaderID);

	{
		int success;
		char info_log[512];
		glGetShaderiv(m_vertShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cout << "Compile error:" << info_log << "\n";
		}
	}
	err = glGetError();
	GL_NO_ERROR;
	m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragShaderID, 1, &FS_SOURCE, nullptr);
	glCompileShader(m_fragShaderID);

	{
		int success;
		char info_log[512];
		glGetShaderiv(m_fragShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cout << "Compile error:" << info_log << "\n";
		}
	}

	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertShaderID);
	glAttachShader(m_programID, m_fragShaderID);
	glLinkProgram(m_programID);
	{
		int  success;
		char info_log[512];
		glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_programID, 512, NULL, info_log);
			std::cout << "Link error:" << info_log << "\n";
		}
	}

	glCreateBuffers(1, &m_vboID);
	glNamedBufferData(m_vboID, sizeof vertPositions, vertPositions, GL_STATIC_DRAW);
	glCreateVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void FullScreenRenderer::RenderTextureFullScreen(Texture2D* tex)
{
	glUseProgram(m_programID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindImageTexture(0, tex->GetId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	glBindVertexArray(m_vaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

FullScreenRenderer::~FullScreenRenderer()
{
	glDeleteShader(m_vertShaderID);
	glDeleteShader(m_fragShaderID);
	glDeleteProgram(m_programID);

	glDeleteBuffers(1, &m_vboID);
}
