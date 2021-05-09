#include "FileManager.h"
#include <fstream>

std::string fs::FileManager::GetFileContent(const std::string_view& filename)
{
	std::ifstream ifs(filename.data());
	std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	return content;
}
