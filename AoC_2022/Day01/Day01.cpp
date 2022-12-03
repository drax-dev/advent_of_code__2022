#include <iostream>
#include <vector>
#include <numeric>
#include <string>

#include "Utils.h"

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
	const auto data = Utils::read_file("day01_input.txt");
	auto results = processFileData(data);
	std::ranges::sort(results);

	std::cout << "Max calories sum is: " << results.back() << "\n";

	// part 2
	const auto resultSize = std::accumulate(results.end() - 3, results.end(), 0);
	std::cout << "Top three calories sum is: " << resultSize << std::endl;
}
