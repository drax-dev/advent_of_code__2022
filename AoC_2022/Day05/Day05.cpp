#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Utils.h"

struct CraneMove
{
	unsigned int amount;
	unsigned int from;
	unsigned int to;
};

std::vector<std::string> parseHeader(const std::vector<std::string>& header, const unsigned long long int numberOfStacks)
{
	std::vector<std::string> results{numberOfStacks};
	for (const auto& line : header)
	{
		unsigned long long int counter = 0;
		const auto lineSize = line.size();
		for (std::size_t index = 1; index < lineSize; index +=4)
		{
			if (const auto ch = line[index]; !isblank(ch))
			{
				results[counter].insert(0, 1, ch);
			}
			counter++;
		}
	}
	return results;
}

unsigned long long getStackAmount(const std::string& numbers)
{
	const auto numbersSplitted = Utils::split_string(numbers, ' ');
	unsigned long long numberOfStack = 0;
	try
	{
		numberOfStack = std::stoll(numbersSplitted.back());
	}
	catch (std::invalid_argument const& ex)
	{
		std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
	}
	catch (std::out_of_range const& ex)
	{
		std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
	}
	return numberOfStack;
}

void doCraneMoveCrateMover9000(std::vector<std::string>& stacks, const CraneMove& move)
{
	for (std::size_t i = 0; i < move.amount; ++i)
	{
		stacks[move.to] += stacks[move.from].back();
		stacks[move.from].pop_back();
	}
}

void doCraneMoveCrateMover9001(std::vector<std::string>& stacks, const CraneMove& move)
{
	const std::string substring = stacks[move.from].substr(stacks[move.from].size() - move.amount, move.amount);
	stacks[move.to] += substring;
	stacks[move.from].erase(stacks[move.from].size() - move.amount, move.amount);
}

CraneMove parseMove(const std::string& line)
{
	unsigned int amount{};
	unsigned int from{};
	unsigned int to{};

	const auto lineSplitted = Utils::split_string(line, ' ');
	try
	{
		amount = std::stoi(lineSplitted.at(1));
		from = std::stoi(lineSplitted.at(3));
		to = std::stoi(lineSplitted.back());
	}
	catch (std::invalid_argument const& ex)
	{
		std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
	}
	catch (std::out_of_range const& ex)
	{
		std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
	}

	return CraneMove{amount, from - 1, to - 1};
}

std::vector<std::string> getHeaderLines(const std::vector<std::string>& lines)
{
	std::vector<std::string> headerLines;
	for (auto&& line : lines)
	{
		if (line.empty())
		{
			break;
		}
		headerLines.emplace_back(line);
	}

	if (headerLines.empty())
	{
		throw std::runtime_error("Wrong header!");
	}
	return headerLines;
}

std::string processFileData(const std::string& data, const std::function<void(std::vector<std::string>& stacks, const CraneMove& move)>& craneMoveFunction)
{
	auto lines = Utils::split_string(data);
	auto headerLines = getHeaderLines(lines);
	lines.erase(lines.begin(), lines.begin() + static_cast<int>(headerLines.size()) + 1);
	const auto& numbers = headerLines.back();
	const auto numberOfStack = getStackAmount(numbers);
	headerLines.erase(headerLines.end() - 1);
	auto stacks = parseHeader(headerLines, numberOfStack);

	for (const auto& line : lines)
	{
		const auto move = parseMove(line);
		craneMoveFunction(stacks, move);
	}

	std::string resultString{};
	for (const auto & stack : stacks)
	{
		resultString += stack.back();
	}

	return resultString;
}


int main()
{
	// part 1
	const auto data = Utils::read_file("day05_input.txt");
	auto results = processFileData(data, doCraneMoveCrateMover9000);
	std::cout << "Result is: " << results << "\n";

	// part2
	results = processFileData(data, doCraneMoveCrateMover9001);
	std::cout << "Result is: " << results << std::endl;
}
