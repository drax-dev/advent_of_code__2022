#include <iostream>
#include <string>
#include <vector>

#include "Utils.h"

std::pair<std::string, long long int> parseCommand(const std::string& line)
{
	const auto& [commandName, numberStr] = Utils::create_pair_from_line(line);
	int number{};
	if (commandName == "addx")
	{
		try
		{
			number = std::stoi(numberStr);
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

	return std::make_pair(commandName, number);
}

long long int runCommand(const std::pair<std::string, long long int>& pair, std::size_t& additionalCycles)
{
	const auto& [command, value] = pair;
	if(command == "addx")
	{
		additionalCycles = 2;
		return value;
	}

	if(command == "noop")
	{
		additionalCycles = 1;
		return 0;
	}

	throw std::runtime_error("Wrong command.");
}

long long int cpuLoop(const std::vector<std::string>& lines)
{
	long long int xRegisterNewValue = 0;
	long long int xRegister = 1;

	std::size_t nextCommandCycle = 1;
	std::size_t nextCommandCycleNewValue = 0;

	std::size_t nextCommandIndex = 0;

	std::size_t nextFrequencyCheck = 20;
	long long int sum = 0;

	for (std::size_t cycle = 1; cycle < 221; ++cycle)
	{
		if(cycle == nextCommandCycle && nextCommandIndex < lines.size())
		{
			xRegister += xRegisterNewValue;

			const auto pair = parseCommand(lines[nextCommandIndex]);
			xRegisterNewValue = runCommand(pair, nextCommandCycleNewValue);
			nextCommandCycle += nextCommandCycleNewValue;
			nextCommandCycleNewValue = 0;
			nextCommandIndex++;
		}

		if (nextFrequencyCheck == cycle)
		{
			sum += xRegister * static_cast<long long int>(cycle);
			std::cout << "cycle=" << cycle << " value=" << xRegister << "\n";
			nextFrequencyCheck += 40;
		}
		// std::cout << "cycle=" << cycle << " xRegister=" << xRegister << "\n";
	}
	return sum;
}

void crtLoop(const std::vector<std::string>& lines)
{
	long long int xRegisterNewValue = 0;
	long long int xRegister = 1;

	std::size_t nextCommandCycle = 1;
	std::size_t nextCommandCycleNewValue = 0;

	std::size_t nextCommandIndex = 0;
	std::size_t nextFrequencyCheck = 40;

	for (std::size_t cycle = 1; cycle < 241; ++cycle)
	{
		if (cycle == nextCommandCycle && nextCommandIndex < lines.size())
		{
			xRegister += xRegisterNewValue;

			const auto pair = parseCommand(lines[nextCommandIndex]);
			xRegisterNewValue = runCommand(pair, nextCommandCycleNewValue);
			nextCommandCycle += nextCommandCycleNewValue;
			nextCommandCycleNewValue = 0;
			nextCommandIndex++;
		}

		if (const auto modulo = cycle % 40; modulo == xRegister || modulo == xRegister + 1 || modulo == xRegister + 2)
		{
			std::cout << '#';
		}
		else
		{
			std::cout << '.';
		}

		if (nextFrequencyCheck == cycle)
		{
			std::cout << "\n";
			nextFrequencyCheck += 40;
		}
	}
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day10_input.txt");
	const auto lines = Utils::split_string(data);
	const auto result = cpuLoop(lines);

    std::cout << "Result is " << result << "\n";

	// part 2
	std::cout << '\n';
	crtLoop(lines);
}
