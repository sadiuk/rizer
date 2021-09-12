#include "FileManager.h"
#include <fstream>

std::string fs::FileManager::GetFileContent(const std::filesystem::path& filename)
{
	std::ifstream ifs(filename.string().data());
	std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	return content;
}
