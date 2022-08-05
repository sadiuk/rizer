#include "ComputeProgram.h"
#include <iostream>
#include "../fs/FileManager.h"

ComputeProgram::ComputeShader::ComputeShader(const GLchar* source)
{
	auto err = glGetError();
	m_id = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(m_id, 1, &source, nullptr);
	glCompileShader(m_id);
	
	int success;
	char info_log[512];
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cout << "Compile error:" << info_log << "\n";
	}
}

ComputeProgram::ComputeShader::~ComputeShader()
{
	glDeleteShader(m_id);
}

ComputeProgram::ComputeProgram(const std::string_view& source)
{
	ComputeShader shader(source.data());
	m_id = glCreateProgram();
	glAttachShader(m_id, shader.GetId());
	glLinkProgram(m_id);
	{
		int  success;
		char info_log[512];
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_id, 512, NULL, info_log);
			std::cout << "Link error:" << info_log << "\n";
		}
	}
	glUseProgram(m_id);
}

std::shared_ptr<ComputeProgram> ComputeProgram::CreateProgramFromFile(const std::string_view& filename)
{
	auto file_content = fs::FileManager::GetFileContent(filename);
	return std::make_shared<ComputeProgram>(file_content);
}


ComputeProgram::~ComputeProgram()
{
	glDeleteProgram(m_id);
}
