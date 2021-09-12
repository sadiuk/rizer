#pragma once 
#include <string>
#include <concepts>
#include <filesystem>

namespace fs
{
	template<typename T>
	concept Integral = std::is_integral_v<T> && !std::_Is_character<T>::value;
	class FileManager
	{
	public:
		static std::string GetFileContent(const std::filesystem::path& filename);
		template<typename... Args>
		static std::string FillStringWithData(const std::string_view& str, Args ... args)
		{
			size_t length = GetCharCount(args...);
			size_t template_param_count = sizeof...(args);
			std::string out_str(str);
			out_str.resize(str.size() + length); // -2 * tpc cause we dont wanna include those '%symbol' formatters (roughly)
			snprintf(out_str.data(), out_str.size(), str.data(), args...);
			return out_str;
		}


		template<typename... Args>	static size_t GetCharCount(Args ... args) { return (... + GetCharCount<Args>(args)); }
		template<typename T> static size_t GetCharCount(T data) { static_assert(false, "Not supported type"); }
		template<> static size_t GetCharCount(char data) { return 1; }
		template<> static size_t GetCharCount(const char* data) { return strlen(data); }
		template<std::floating_point T> static size_t GetCharCount(const T& data) { return std::to_string(data).length(); }
		template<Integral I> static size_t GetCharCount(const I& data) { return std::to_string(data).length(); }
	};
}