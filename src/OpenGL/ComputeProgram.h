#pragma once 
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include <glad/glad.h>

#include "Texture2D.h"
#include "Buffer.h"
#include "Program.h"

class ComputeProgram : public Program
{
private:
	class ComputeShader
	{
		GLuint m_id;
	public:
		[[nodiscard]] GLuint GetId() const { return m_id; }
		ComputeShader(const char* source);
		~ComputeShader();
	};

private:
	GLuint m_id;
public:
	static std::shared_ptr<ComputeProgram> CreateProgramFromFile(const std::string_view& filename);
	static std::shared_ptr<ComputeProgram> CreateProgramFromSource(const std::string_view& filename);
	ComputeProgram(const std::string_view& source);
	~ComputeProgram();
	[[nodiscard]] GLuint GetId() const override { return m_id; }
};
