#include "Utils.h"
#include <sstream>
#include <fstream>

namespace Utils
{
	std::string trim(std::string string_input)
	{
		constexpr auto whitespaces = " \t\n\r\f\v";
		string_input.erase(string_input.find_last_not_of(whitespaces) + 1);
		string_input.erase(0, string_input.find_first_not_of(whitespaces));
		return string_input;
	}

	std::string read_file(const fs::path& path)
	{
		std::string result{};
		if (std::ifstream f{path, std::ios::in | std::ios::binary})
		{
			const auto size = file_size(path);
			result = std::string(size, '\0');
			f.read(result.data(), static_cast<long long>(size));
		}
		return result;
	}

	std::vector<std::string> split_string(const std::string& input_string, const char delimiter)
	{
		std::vector<std::string> results;
		std::string temp_string;
		std::stringstream ss(input_string);
		while (std::getline(ss, temp_string, delimiter))
		{
			temp_string = trim(temp_string);
			results.emplace_back(temp_string);
		}
		return results;
	}
}
