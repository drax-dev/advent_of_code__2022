#include <array>
#include <functional>
#include <iostream>

#include "Utils.h"


struct Position
{
	std::size_t x;
	std::size_t y;
};

struct Board
{
	std::vector<std::vector<char>> grid;
	std::size_t gridXSize{};
	std::size_t gridYSize{};
	Position currentHeadPosition{};
	std::array<Position, 9> currentTailsPositions{};

	void drawWithTailAndHead() const
	{
		std::cout << "==================\n";
		for (std::size_t y = 0; y < gridYSize; ++y)
		{
			for (std::size_t x = 0; x < gridXSize; ++x)
			{
				if (currentHeadPosition.x == x && currentHeadPosition.y == y)
				{
					std::cout << 'H';
				}
				else if (currentTailsPositions[0].x == x && currentTailsPositions[0].y == y)
				{
					std::cout << 'T';
				}
				else
				{
					std::cout << grid[y][x];
				}
			}
			std::cout << "\n";
		}
		std::cout << "==================\n";
	}

	void drawWithTailsAndHead() const
	{
		std::cout << "==================\n";
		for (std::size_t y = 0; y < gridYSize; ++y)
		{
			for (std::size_t x = 0; x < gridXSize; ++x)
			{
				if (currentHeadPosition.x == x && currentHeadPosition.y == y)
				{
					std::cout << 'H';
				}
				else
				{
					//for (std::size_t i = 0; i < currentTailsPositions.size(); ++i)
					//{
					//	if (currentTailsPositions[i].x == x && currentTailsPositions[i].y == y)
					//	{
					//		std::cout << i + 1;
					//		break;
					//	}
					//}
					if (auto result = std::ranges::find_if(currentTailsPositions, [x, y](const Position& position){return position.x == x && position.y == y;});
						result != std::end(currentTailsPositions))
					{
						const auto index = std::distance(std::begin(currentTailsPositions), result);
						std::cout << index + 1;
					}
					else
					{
						std::cout << grid[y][x];
					}
				}
			}
			std::cout << "\n";
		}
		std::cout << "==================\n";
	}

	void drawBoard() const
	{
		std::cout << "==================\n";
		for (const auto& line : grid)
		{
			for (const auto& field : line)
			{
				std::cout << field;
			}
			std::cout << "\n";
		}
		std::cout << "==================\n";
	}
};

void moveTail(Board& board)
{
	const auto diffX = std::abs(static_cast<int>(board.currentHeadPosition.x) - static_cast<int>(board.currentTailsPositions[0].x));
	const auto diffY = std::abs(static_cast<int>(board.currentHeadPosition.y) - static_cast<int>(board.currentTailsPositions[0].y));
	if (diffX > 1)
	{
		if (board.currentHeadPosition.x > board.currentTailsPositions[0].x)
		{
			board.currentTailsPositions[0].x++;
		}
		else
		{
			board.currentTailsPositions[0].x--;
		}

		if (board.currentHeadPosition.y != board.currentTailsPositions[0].y)
		{
			board.currentTailsPositions[0].y = board.currentHeadPosition.y;
		}
	}

	if (diffY > 1)
	{
		if (board.currentHeadPosition.y > board.currentTailsPositions[0].y)
		{
			board.currentTailsPositions[0].y++;
		}
		else
		{
			board.currentTailsPositions[0].y--;
		}

		if (board.currentHeadPosition.x != board.currentTailsPositions[0].x)
		{
			board.currentTailsPositions[0].x = board.currentHeadPosition.x;
		}
	}
	board.grid[board.currentTailsPositions[0].y][board.currentTailsPositions[0].x] = '#';
}

void moveTails(Board& board)
{
	for (std::size_t i = 0; i < board.currentTailsPositions.size(); ++i)
	{
		std::size_t leaderX;
		std::size_t leaderY;
		if (i == 0)
		{
			leaderX = board.currentHeadPosition.x;
			leaderY = board.currentHeadPosition.y;
		}
		else
		{
			leaderX = board.currentTailsPositions[i - 1].x;
			leaderY = board.currentTailsPositions[i - 1].y;
		}

		const auto diffX = std::abs(static_cast<int>(leaderX) - static_cast<int>(board.currentTailsPositions[i].x));
		const auto diffY = std::abs(static_cast<int>(leaderY) - static_cast<int>(board.currentTailsPositions[i].y));

		if(diffX > 1 && diffY > 1)
		{
			if (leaderX > board.currentTailsPositions[i].x)
			{
				board.currentTailsPositions[i].x++;
			}
			else
			{
				board.currentTailsPositions[i].x--;
			}

			if (leaderY > board.currentTailsPositions[i].y)
			{
				board.currentTailsPositions[i].y++;
			}
			else
			{
				board.currentTailsPositions[i].y--;
			}
		}
		else if (diffX > 1)
		{
			if (leaderX > board.currentTailsPositions[i].x)
			{
				board.currentTailsPositions[i].x++;
			}
			else
			{
				board.currentTailsPositions[i].x--;
			}

			if (leaderY != board.currentTailsPositions[i].y)
			{
				board.currentTailsPositions[i].y = leaderY;
			}
		}
		else if (diffY > 1)
		{
			if (leaderY > board.currentTailsPositions[i].y)
			{
				board.currentTailsPositions[i].y++;
			}
			else
			{
				board.currentTailsPositions[i].y--;
			}

			if (leaderX != board.currentTailsPositions[i].x)
			{
				board.currentTailsPositions[i].x = leaderX;
			}
		}
		if (i == 8)
		{
			board.grid[board.currentTailsPositions[i].y][board.currentTailsPositions[i].x] = '#';
		}
	}
}

void moveRight(Board& board, const unsigned int distance, const std::function<void(Board&)>& moveTailFunc)
{
	for (std::size_t x = 0; x < distance; ++x)
	{
		if (board.gridXSize - 1 == board.currentHeadPosition.x)
		{
			for (std::size_t y = 0; y < board.gridYSize; ++y)
			{
				board.grid[y].emplace_back('.');
			}
			board.gridXSize++;
		}
		board.currentHeadPosition.x++;
		moveTailFunc(board);
		// board.drawWithTailsAndHead();
	}
}

void moveLeft(Board& board, const unsigned int distance, const std::function<void(Board&)>& moveTailFunc)
{
	for (std::size_t x = 0; x < distance; ++x)
	{
		if (board.currentHeadPosition.x == 0)
		{
			for (std::size_t y = 0; y < board.gridYSize; ++y)
			{
				board.grid[y].insert(board.grid[y].begin(), '.');
			}
			board.gridXSize++;
			board.currentHeadPosition.x++;
			for (auto& currentTailsPosition : board.currentTailsPositions)
			{
				currentTailsPosition.x++;
			}
		}
		board.currentHeadPosition.x--;
		moveTailFunc(board);
		// board.drawWithTailsAndHead();
	}
}

void moveUp(Board& board, const unsigned int distance, const std::function<void(Board&)>& moveTailFunc)
{
	for (std::size_t y = 0; y < distance; ++y)
	{
		if (board.gridYSize - 1 == board.currentHeadPosition.y)
		{
			board.grid.emplace_back(board.gridXSize, '.');
			board.gridYSize++;
		}
		board.currentHeadPosition.y++;
		moveTailFunc(board);
		// board.drawWithTailsAndHead();
	}
}

void moveDown(Board& board, const unsigned int distance, const std::function<void(Board&)>& moveTailFunc)
{
	for (std::size_t y = 0; y < distance; ++y)
	{
		if (board.currentHeadPosition.y == 0)
		{
			board.grid.insert(board.grid.begin(), std::vector(board.gridXSize, '.'));
			board.gridYSize++;
			board.currentHeadPosition.y++;
			for (auto& currentTailsPosition : board.currentTailsPositions)
			{
				currentTailsPosition.y++;
			}
		}
		board.currentHeadPosition.y--;
		moveTailFunc(board);
		// board.drawWithTailsAndHead();
	}
}

void moveHead(Board& board, const std::pair<std::string, std::string>& movePair, const std::function<void(Board&)>& moveTailFunc)
{
	const auto& [direction, distanceStr] = movePair;
	unsigned int distance{};
	try
	{
		distance = std::stoi(distanceStr);
	}
	catch (std::invalid_argument const& ex)
	{
		std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
	}
	catch (std::out_of_range const& ex)
	{
		std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
	}

	switch (direction.front())
	{
	case 'R':
		moveRight(board, distance, moveTailFunc);
		break;
	case 'L':
		moveLeft(board, distance, moveTailFunc);
		break;
	case 'U':
		moveUp(board, distance, moveTailFunc);
		break;
	case 'D':
		moveDown(board, distance, moveTailFunc);
		break;
	default:
		throw std::runtime_error("Wrong direction!");
	}
}

Board initGrid()
{
	Board board;
	board.currentHeadPosition = { 0, 0 };
	board.grid.emplace_back(1, 's');
	board.gridXSize = 1;
	board.gridYSize = 1;
	return board;
}

Board processFileData(const std::vector<std::string>& lines, const std::function<void(Board&)>& moveTailFunc)
{
	auto board = initGrid();
	// board.drawWithTailAndHead();
	// board.drawWithTailsAndHead();

	for (const auto& line : lines)
	{
		const auto pair = Utils::create_pair_from_line(line);
		moveHead(board, pair, moveTailFunc);
		// board.drawWithTailAndHead();
		// board.drawWithTailsAndHead();
	}
	return board;
}

unsigned long long int calculateFieldsVisitedByTail(const Board& board)
{
	unsigned long long int result{};
	for (const auto& line : board.grid)
	{
		for (const auto& field : line)
		{
			if (field == '#')
			{
				result++;
			}
		}
	}
	return result;
}


int main()
{
	const auto data = Utils::read_file("day09_input.txt");
	const auto lines = Utils::split_string(data);
	// part 1
	//const auto boardPart1 = processFileData(lines, moveTail);
	//// boardPart1.drawBoard();
	//auto result = calculateFieldsVisitedByTail(boardPart1);
	//std::cout << "Result is: " << result << "\n";

	// part 2
	const auto boardPart2 = processFileData(lines, moveTails);
	// boardPart2.drawBoard();
	const auto result = calculateFieldsVisitedByTail(boardPart2);
	std::cout << "Result is: " << result << "\n";
}
