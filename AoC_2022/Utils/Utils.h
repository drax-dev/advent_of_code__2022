#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace Utils
{
	namespace fs = std::filesystem;

	std::string trim(std::string string_input);
	std::string read_file(const fs::path& path);
	std::vector<std::string> split_string(const std::string& input_string, char delimiter = '\n');
	std::pair<std::string, std::string> create_pair_from_line(const std::string& line, char delimiter = ' ');
}