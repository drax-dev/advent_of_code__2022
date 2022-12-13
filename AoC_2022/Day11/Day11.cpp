#include <deque>
#include <functional>
#include <iostream>
#include "Utils.h"

struct Monkey
{
	std::deque<unsigned long long int> items;
	std::function<unsigned long long int(unsigned long long int)> operation;
	int testDivisor{ 1 };
	std::shared_ptr<Monkey> connectedMonkeyIfTrue;
	std::shared_ptr<Monkey> connectedMonkeyIfFalse;
	std::size_t inspectedItemsCounter{ 0 };
};

void printMonkeysItems(const std::vector<std::shared_ptr<Monkey>>& monkeys)
{
	const auto monkeysSize = monkeys.size();
	for (std::size_t i = 0; i < monkeysSize; ++i)
	{
		std::cout << "Monkey " << i << ": ";
		for (const auto& item : monkeys[i]->items)
		{
			std::cout << item << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void doNextRound(const std::vector<std::shared_ptr<Monkey>>& monkeys, const bool divideWorryLevel, const unsigned long long int mod)
{
	for (const auto& monkey : monkeys)
	{
		for (const auto& item : monkey->items)
		{
			monkey->inspectedItemsCounter++;
			auto worryLevel = monkey->operation(item);

			if(divideWorryLevel)
			{
				worryLevel /= 3;
			}
			else
			{
				worryLevel %= mod;
			}

			if (worryLevel % monkey->testDivisor == 0)
			{
				monkey->connectedMonkeyIfTrue->items.emplace_back(worryLevel);
			}
			else
			{
				monkey->connectedMonkeyIfFalse->items.emplace_back(worryLevel);
			}
		}
		monkey->items.clear();
	}
}

void keepAwayLoop(const std::vector<std::shared_ptr<Monkey>>& monkeys, const std::size_t rounds, const bool divideWorryLevel, const unsigned long long int mod)
{
	for (std::size_t round = 1; round < rounds; ++round)
	{
		doNextRound(monkeys, divideWorryLevel, mod);
		// printMonkeysItems(monkeys);
	}
}

std::deque<unsigned long long int> parseItems(const std::string& itemsLine)
{
	auto lineSplitted = Utils::split_string(itemsLine, ' ');
	lineSplitted.erase(lineSplitted.begin(), lineSplitted.begin() + 2);
	std::deque<unsigned long long int> items;
	for (auto& item : lineSplitted)
	{
		std::erase(item, ',');
		const auto itemNumber = std::stoi(item);
		items.emplace_back(itemNumber);
	}
	return items;
}

std::function<unsigned long long int(unsigned long long int)> parseOperation(const std::string& operationLine)
{
	std::function<unsigned long long int(unsigned long long int)> operation;
	auto lineSplitted = Utils::split_string(operationLine, ' ');
	const auto secondParam = lineSplitted[5];
	if (const auto sign = lineSplitted[4]; sign == "*")
	{
		if (secondParam == "old")
		{
			operation = [=](const unsigned long long int old)
			{
				return old * old;
			};
		}
		else
		{
			const unsigned long long int secondParamNum = std::stoi(secondParam);
			operation = [=](const unsigned long long int old)
			{
				return old * secondParamNum;
			};
		}

	}
	else if (sign == "+")
	{
		if (secondParam == "old")
		{
			operation = [=](const unsigned long long int old)
			{
				return old + old;
			};
		}
		else
		{
			unsigned const long long int secondParamNum = std::stoi(secondParam);
			operation = [=](const unsigned long long int old)
			{
				return old + secondParamNum;
			};
		}

	}
	else
	{
		throw std::runtime_error("Wrong operation in parseOperation().");
	}

	return operation;
}

int parseTestDivisor(const std::string& testDivisorLine)
{
	auto lineSplitted = Utils::split_string(testDivisorLine, ' ');
	return std::stoi(lineSplitted[3]);
}

std::size_t parseConnectedMonkeyIndex(const std::string& throwMonkeyLine)
{
	auto lineSplitted = Utils::split_string(throwMonkeyLine, ' ');
	return std::stoi(lineSplitted[5]);
}

void parseMonkey(const std::vector<std::shared_ptr<Monkey>>& monkeys, const std::vector<std::string>& singleMonkeyLines)
{
	auto lineSplitted = Utils::split_string(singleMonkeyLines[0], ' ');
	lineSplitted[1].pop_back();
	const auto monkeyIndex = std::stoi(lineSplitted[1]);
	monkeys[monkeyIndex]->items = parseItems(singleMonkeyLines[1]);
	monkeys[monkeyIndex]->operation = parseOperation(singleMonkeyLines[2]);
	monkeys[monkeyIndex]->testDivisor = parseTestDivisor(singleMonkeyLines[3]);
	const auto monkeyIndexIfTrue = parseConnectedMonkeyIndex(singleMonkeyLines[4]);
	monkeys[monkeyIndex]->connectedMonkeyIfTrue = monkeys[monkeyIndexIfTrue];
	const auto monkeyIndexIfFalse = parseConnectedMonkeyIndex(singleMonkeyLines[5]);
	monkeys[monkeyIndex]->connectedMonkeyIfFalse = monkeys[monkeyIndexIfFalse];
}

std::vector<std::shared_ptr<Monkey>> parseMonkeys(const std::vector<std::string>& lines)
{
	const auto monkeysNumber = (lines.size() + 1) / 7;
	std::vector<std::shared_ptr<Monkey>> monkeys;
	monkeys.reserve(monkeysNumber);
	for (std::size_t i = 0; i < monkeysNumber; ++i)
	{
		monkeys.emplace_back(std::make_shared<Monkey>());
	}

	const int lineSize = static_cast<int>(lines.size());
	for (int i = 0; i < lineSize; i += 7)
	{
		const auto first = lines.begin() + i;
		const auto last = lines.begin() + i + 6;
		parseMonkey(monkeys, std::vector<std::string>{first, last});
	}

	return monkeys;
}

int main()
{
	//// part 1
	const auto data = Utils::read_file("day11_input.txt");
	const auto lines = Utils::split_string(data);
	//const auto monkeys = parseMonkeys(lines);
	//keepAwayLoop(monkeys, 21, true, 0);

	//std::vector<long long int> inspectedCounters;
	//inspectedCounters.reserve(monkeys.size());
	//for (const auto& monkey : monkeys)
	//{
	//	inspectedCounters.emplace_back(monkey->inspectedItemsCounter);
	//	// std::cout << monkey->inspectedItemsCounter << "\n";
	//}
	//std::ranges::sort(inspectedCounters, std::ranges::greater());
	//const auto monkeyBusiness = inspectedCounters.front() * inspectedCounters[1];

	//std::cout << "\nResult is " << monkeyBusiness << "\n";

	// part 2
	const auto monkeysPart2 = parseMonkeys(lines);

	unsigned long long int mod = 1;
	for(const auto& monkey: monkeysPart2)
	{
		mod *= monkey->testDivisor;
	}
	keepAwayLoop(monkeysPart2, 10001, false, mod);

	std::vector<long long int> inspectedCountersPart2;
	inspectedCountersPart2.reserve(monkeysPart2.size());
	for (const auto& monkey : monkeysPart2)
	{
		inspectedCountersPart2.emplace_back(monkey->inspectedItemsCounter);
		// std::cout << monkey->inspectedItemsCounter << "\n";
	}
	std::ranges::sort(inspectedCountersPart2, std::ranges::greater());
	const auto monkeyBusinessPart2 = inspectedCountersPart2.front() * inspectedCountersPart2[1];

	std::cout << "\nResult is " << monkeyBusinessPart2 << "\n";
}

