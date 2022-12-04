#include <iostream>
#include <set>
#include <ranges>

#include "Utils.h"

unsigned int calculatePriority(const std::string::value_type ch)
{
	if (isupper(ch))
	{
		return ch - 'A' + 27;
	}

	return ch - 'a' + 1;
}

unsigned long long int processFileDataPart1(const std::vector<std::string>& splittedData)
{
	unsigned long long int sum = 0;
	for (const auto& line : splittedData)
	{
		std::set<char> uniqueCharacters;
		const auto lineSize = line.size();
		const auto halfOfSize = lineSize / 2;
		for (const auto firstSubstring = line.substr(0, halfOfSize); const auto ch : firstSubstring)
		{
			uniqueCharacters.insert(ch);
		}

		for (const auto secondSubString = line.substr(halfOfSize, lineSize); const auto ch : secondSubString)
		{
			if (uniqueCharacters.contains(ch))
			{
				const unsigned int priority = calculatePriority(ch);
				//std::cout << "first: " << firstSubstring << " size=" << firstSubstring.size() << "\n";
				//std::cout << "second: " << secondSubString << " size=" << secondSubString.size() << "\n";
				//std::cout << priority << " (" << ch << ")" << "\n";
				sum += priority;
				break;
			}
		}
	}
	return sum;
}

unsigned long long int processFileDataPart2(const std::vector<std::string>& splittedData)
{
	unsigned long long int sum = 0;
	for (std::size_t index = 0; index < splittedData.size() - 2; index += 3)
	{
		std::vector firstLine(splittedData[index].begin(), splittedData[index].end());
		std::ranges::sort(firstLine);
		std::vector secondLine(splittedData[index + 1].begin(), splittedData[index + 1].end());
		std::ranges::sort(secondLine);
		std::vector thirdLine(splittedData[index + 2].begin(), splittedData[index + 2].end());
		std::ranges::sort(thirdLine);

		std::vector<char> output;
		std::ranges::set_intersection(firstLine, secondLine, std::back_inserter(output));
		std::vector<char> output2;
		std::ranges::set_intersection(output, thirdLine, std::back_inserter(output2));

		std::set uniqueCharacters(output2.begin(), output2.end());

		if (uniqueCharacters.size() > 1)
		{
			throw std::runtime_error("Matched too much characters!");
		}

		if (uniqueCharacters.empty())
		{
			throw std::runtime_error("Empty result!");
		}

		const auto ch= output2.front();
		//std::cout << "badge item type: " << ch << "\n";
		const unsigned int priority = calculatePriority(ch);
		sum += priority;
	}
	return sum;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day03_input.txt");
	const auto splitResults = Utils::split_string(data);
	auto result = processFileDataPart1(splitResults);
	std::cout << "Result is: " << result << "\n";

	// part 2
	try 
	{
		result = processFileDataPart2(splitResults);
		std::cout << "Result is: " << result << "\n";
	}
	catch (const std::runtime_error& err)
	{
		std::cout << err.what() << "\n";
	}
}
