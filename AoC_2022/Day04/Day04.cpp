#include <iostream>
#include <utility>
#include <vector>
#include <sstream>

#include "Utils.h"

struct Range
{
	int begin;
	int end;

	[[nodiscard]] bool contains(const Range& other) const
	{
		if(other.begin >= begin && other.end <= end)
		{
			return true;
		}
		return false;
	}
};

Range processRange(const std::string& pairData)
{
	std::stringstream ss(pairData);
	if (std::string first; std::getline(ss, first, '-'))
	{
		first = Utils::trim(first);
		if (std::string second; std::getline(ss, second))
		{
			second = Utils::trim(second);
			// std::cout << first << "-" << second << "\n";
			try
			{
				const auto beginIndex = std::stoi(first);
				const auto endIndex = std::stoi(second);
				return Range(beginIndex, endIndex);
			}
			catch (std::invalid_argument const& ex)
			{
				std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
			}
			catch (std::out_of_range const& ex)
			{
				std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
			}
		}
	}

	throw std::runtime_error("Parse error.");
}

std::vector<std::pair<Range, Range>> processFileData(const std::string& data)
{
	std::stringstream ss(data);
	std::string firstPair;
	std::string secondPair;
	std::vector<std::pair<Range, Range>> results;

	while (std::getline(ss, firstPair, ','))
	{
		firstPair = Utils::trim(firstPair);
		auto firstRange = processRange(firstPair);
		std::getline(ss, secondPair);
		secondPair = Utils::trim(secondPair);
		auto secondRange = processRange(secondPair);
		results.emplace_back(firstRange, secondRange);
		// std::cout << firstPair << "->" << secondPair << "\n";
	}
	return results;
}

unsigned long long int calculatePairs(const std::vector<std::pair<Range, Range>>& pairs)
{
	unsigned long long int counter{};
	for (const auto & [first, second] : pairs)
	{
		if (first.contains(second) || second.contains(first))
		{
			counter++;
		}
	}
	return counter;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day04_input.txt");
	try
	{
		const auto results = processFileData(data);
		const auto pairsNum = calculatePairs(results);
		std::cout << "Result is: " << pairsNum << "\n";
	}
	catch (const std::runtime_error& err)
	{
		std::cout << err.what() << "\n";
	}
}
