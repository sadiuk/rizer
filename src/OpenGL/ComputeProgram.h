#pragma once 
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include <glad/glad.h>

#include "Texture2D.h"
#include "Buffer.h"

class ComputeProgram
{
private:
	class ComputeShader
	{
		GLuint m_id;
	public:
		GLuint GetId() const { return m_id; }
		ComputeShader(const std::string_view& source);
		~ComputeShader();
	};

private:
	GLuint m_id;
public:
	static std::shared_ptr<ComputeProgram> CreateProgramFromFile(const std::string_view& filename);
	ComputeProgram(const std::string_view& source);
	~ComputeProgram();
};
