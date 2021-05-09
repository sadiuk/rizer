#pragma once 
#include <string>

namespace fs
{
	class FileManager
	{
	public:
		static std::string GetFileContent(const std::string_view& filename);
	};
}