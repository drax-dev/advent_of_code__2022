#include "Utils.h"

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
}
