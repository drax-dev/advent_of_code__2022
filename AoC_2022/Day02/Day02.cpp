#include <functional>
#include <iostream>

#include "Utils.h"


int roundResultForPart1(const char opponent, const char player)
{
	// points:
	// for player character: 1 for Rock, 2 for Paper, and 3 for Scissors
	// for result: 0 if you lost, 3 if the round was a draw, and 6 if you won
	switch (opponent)
	{
	case 'A': // rock vs. 
		switch (player)
		{
		case 'X': // rock
			return 4;
		case 'Y': // paper
			return 8;
		case 'Z': // scissors
			return 3;
		default:
			throw std::runtime_error("wrong player character");
		}

	case 'B': // paper vs.
		switch (player)
		{
		case 'X': // rock
			return 1;
		case 'Y': // paper
			return 5;
		case 'Z': // scissors
			return 9;
		default:
			throw std::runtime_error("wrong player character");
		}

	case 'C': // scissors vs.
		switch (player)
		{
		case 'X': // rock
			return 7;
		case 'Y': // paper
			return 2;
		case 'Z': // scissors
			return 6;
		default:
			throw std::runtime_error("wrong player character");
		}

	default:
		throw std::runtime_error("wrong opponent character");
	}
}

int roundResultForPart2(const char opponent, const char result)
{
	// points:
	// for player character: 1 for Rock, 2 for Paper, and 3 for Scissors
	// for result: 0 if you lost, 3 if the round was a draw, and 6 if you won
	//
	// X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win
	switch (opponent)
	{
	case 'A': // rock
		switch (result)
		{
		case 'X': // lose - scissors
			return 3;
		case 'Y': // draw - rock
			return 4;
		case 'Z': // win - paper
			return 8;
		default:
			throw std::runtime_error("wrong player character");
		}

	case 'B': // paper
		switch (result)
		{
		case 'X': // lose - rock
			return 1;
		case 'Y': // draw - paper
			return 5;
		case 'Z': // win - scissors
			return 9;
		default:
			throw std::runtime_error("wrong player character");
		}

	case 'C': // scissors
		switch (result)
		{
		case 'X': // lose - paper
			return 2;
		case 'Y': // draw - scissors
			return 6;
		case 'Z': // win - rock
			return 7;
		default:
			throw std::runtime_error("wrong player character");
		}

	default:
		throw std::runtime_error("wrong opponent character");
	}
}

int processFileData(const std::string& data, const std::function<int(char, char)>& calculateFunction)
{
	int result{};
	try
	{
		bool isOpponentLoaded = false;
		char opponent = '\0';
		for(const auto ch : data)
		{
			if (std::isalpha(ch))
			{
				if (!isOpponentLoaded)
				{
					isOpponentLoaded = true;
					opponent = ch;
				}
				else
				{
					const char player = ch;
					result += calculateFunction(opponent, player);
					isOpponentLoaded = false;
				}
			}
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cout << err.what() << "\n";
		return -1;
	}
	return result;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day02_input.txt");
	auto result = processFileData(data, roundResultForPart1);
	std::cout << "Result is: " << result << "\n";

	// part 2
	result = processFileData(data, roundResultForPart2);
	std::cout << "Result is: " << result << "\n";
}
