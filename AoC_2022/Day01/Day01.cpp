// Day01.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

std::string trim(std::string string_input)
{
	constexpr auto whitespaces = " \t\n\r\f\v";
	string_input.erase(string_input.find_last_not_of(whitespaces) + 1);
	string_input.erase(0, string_input.find_first_not_of(whitespaces));
	return string_input;
}

std::string read_file(const fs::path& path)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	const auto sz = fs::file_size(path);
	std::string result(sz, '\0');
	f.read(result.data(), static_cast<long long>(sz));
	return result;
}

std::vector<int> processFileData(const std::string& data)
{
	std::vector<int> results;
	std::string buffer{};
	std::size_t counter = 0;
	unsigned long long int caloriesSum = 0;
	for (const auto ch : data)
	{
		if (ch == '\r' || ch == '\n')
		{
			counter++;
		}
		else
		{
			buffer += ch;
			if (counter == 2)
			{
				counter = 0;
			}
		}

		if (counter == 2)
		{
			std::size_t pos{};
			int singleCaloriesData{};
			try
			{
				//std::cout << "std::stoi('" << buffer << "'): ";
				singleCaloriesData = std::stoi(buffer, &pos);
				//std::cout << singleCaloriesData << "; pos: " << pos << '\n';
			}
			catch (std::invalid_argument const& ex)
			{
				std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
			}
			catch (std::out_of_range const& ex)
			{
				std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
				const long long ll{ std::stoll(buffer, &pos) };
				std::cout << "std::stoll('" << buffer << "'): " << ll << "; pos: " << pos << '\n';
			}
			caloriesSum += singleCaloriesData;
			buffer = "";
		}
		else if (counter == 4)
		{
			results.emplace_back(caloriesSum);
			caloriesSum = 0;
			counter = 0;
		}
	}

	return results;
}


int main()
{
	// part 1
	const auto data = read_file("day01_input.txt");
	auto results = processFileData(data);
	std::ranges::sort(results);

	std::cout << "Max calories sum is: " << results.back();
}
