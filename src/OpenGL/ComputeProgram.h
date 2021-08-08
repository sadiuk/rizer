#pragma once 
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include <glad/glad.h>

#include "Texture2D.h"
#include "Buffer.h"
#include "Program.h"
#include "../fs/FileManager.h"


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
	template<typename ...Args>
	static std::shared_ptr<ComputeProgram> CreateProgramFromFileWithParams(const std::string_view& filename, Args&&... args)
	{
		auto file_content = fs::FileManager::GetFileContent(filename);
		std::string filled_src = fs::FileManager::FillStringWithData(file_content, std::forward<Args>(args)...);
		return std::shared_ptr<ComputeProgram>(new ComputeProgram(filled_src));
	}
	ComputeProgram(const std::string_view& source);
	~ComputeProgram();
	[[nodiscard]] GLuint GetId() const override { return m_id; }
};
