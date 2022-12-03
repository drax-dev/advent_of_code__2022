#pragma once
#include <filesystem>
#include <string>

namespace Utils
{
	namespace fs = std::filesystem;

	std::string trim(std::string string_input);
	std::string read_file(const fs::path& path);
}