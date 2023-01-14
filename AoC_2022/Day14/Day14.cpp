#include <filesystem>
#include <iostream>
#include <climits>
#include "Utils.h"

struct Position
{
	int x;
	int y;

	bool operator==(const Position& other) const
	{
		return x == other.x && y == other.y;
	}
};

struct RockShape
{
	std::vector<Position> positions;
};

struct Board
{
	std::vector<std::vector<char>> board;
	int minX = INT_MAX;
	int maxX = 500;
	int minY = INT_MAX;
	int maxY = -1;
};

std::vector<RockShape> parseRocks(const std::vector<std::string>& lines)
{
	std::vector<std::string> rocks;
	std::vector<RockShape> rockShapes;
	for (const auto& line : lines)
	{
		std::string tempString;
		std::stringstream ss(line);
		if (std::getline(ss, tempString, '-'))
		{
			tempString = Utils::trim(tempString);
			rocks.emplace_back(tempString);
		}
		while (std::getline(ss, tempString, '-'))
		{
			tempString.erase(0, 1);
			tempString = Utils::trim(tempString);
			rocks.emplace_back(tempString);
		}

		RockShape rockShape{};
		for (const auto& rock : rocks)
		{
			std::string first;
			if (std::stringstream ss2(rock); std::getline(ss2, first, ','))
			{
				first = Utils::trim(first);
				std::string second;
				std::getline(ss2, second);
				second = Utils::trim(second);
				try
				{
					const auto firstNum = std::stoi(first);
					const auto secondNum = std::stoi(second);
					rockShape.positions.emplace_back(Position{ firstNum, secondNum });
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
		rockShapes.push_back(rockShape);
		rocks.clear();
	}
	return rockShapes;
}

Board createBoard(const std::vector<std::string>& lines)
{
	Board board;
	const auto rocksShapes = parseRocks(lines);

	for (const auto& [positions] : rocksShapes)
	{
		for (const auto& [x, y] : positions)
		{
			if (y > board.maxY)
			{
				board.maxY = y;
			}
			if (y < board.minY)
			{
				board.minY = y;
			}
			if (x > board.maxX)
			{
				board.maxX = x;
			}
			if (x < board.minX)
			{
				board.minX = x;
			}
		}
	}

	board.board = std::vector(board.maxY + 1, std::vector(2 * (board.maxX + 1), '.'));

	for (const auto& [positions] : rocksShapes)
	{
		const auto rockShapePositionsSize = positions.size();
		for (std::size_t i = 0; i < rockShapePositionsSize - 1; i++)
		{
			const auto diffX = positions[i].x - positions[i + 1].x;
			const auto diffY = positions[i].y - positions[i + 1].y;
			if (diffX != 0)
			{
				if (diffX < 0)
				{
					for (int x = 0; x <= -diffX; x++)
					{
						board.board[positions[i].y][positions[i].x + x] = '#';
					}
				}
				else
				{
					for (int x = 0; x >= -diffX; x--)
					{
						board.board[positions[i].y][positions[i].x + x] = '#';
					}
				}
			}
			else if (diffY != 0)
			{
				if (diffY < 0)
				{
					for (int y = 0; y <= -diffY; y++)
					{
						board.board[positions[i].y + y][positions[i].x] = '#';
					}
				}
				else
				{
					for (int y = 0; y >= -diffY; y--)
					{
						board.board[positions[i].y + y][positions[i].x] = '#';
					}
				}
			}
		}
	}
	return board;
}

void drawWholeBoard(const Board& board)
{
	std::cout << '\n';
	for (const auto& row : board.board)
	{
		for (const char field : row)
		{
			std::cout << field;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void drawRangeOfBoard(const Board& board)
{
	std::cout << '\n';
	for (int y = 0; y < board.maxY + 1; y++)
	{
		for (int x = board.minX; x < board.maxX + 1; x++)
		{
			std::cout << board.board[y][x];
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

bool isCollision(const Position& position, const Board& board)
{
	const auto value = board.board[position.y][position.x];
	return value != '.';
}

bool isOutOfBound(const Position& position, const Board& board)
{
	if (position.y >= board.maxY)
	{
		return true;
	}
	return false;
}

Position moveSand(const Position& oldPosition, Position& nextPosition, Board& board)
{
	if (!isCollision(nextPosition, board))
	{
		board.board[oldPosition.y][oldPosition.x] = '.';
		board.board[nextPosition.y][nextPosition.x] = 'o';
		// drawRangeOfBoard(board);
		return nextPosition;
	}

	nextPosition.x--;
	if (!isCollision(nextPosition, board))
	{
		board.board[oldPosition.y][oldPosition.x] = '.';
		board.board[nextPosition.y][nextPosition.x] = 'o';
		// drawRangeOfBoard(board);
		return nextPosition;
	}

	nextPosition.x += 2;
	if (!isCollision(nextPosition, board))
	{
		board.board[oldPosition.y][oldPosition.x] = '.';
		board.board[nextPosition.y][nextPosition.x] = 'o';
		// drawRangeOfBoard(board);
		return nextPosition;
	}

	return { 0, 0 };
}

int pourSand(Board& board)
{
	constexpr Position startingPosition{ 500, 0 };
	Position currentPosition{ startingPosition };
	int sandCounter = 0;
	while (true)
	{
		Position oldPosition{ currentPosition };
		Position nextPosition{ currentPosition };
		nextPosition.y++;
		currentPosition = moveSand(currentPosition, nextPosition, board);

		if (isOutOfBound(currentPosition, board))
		{
			break;
		}

		if (currentPosition == Position{ 0, 0 })
		{
			if (oldPosition == startingPosition)
			{
				break;
			}

			sandCounter++;
			// drawRangeOfBoard(board);
			currentPosition = startingPosition;
		}
	}

	return sandCounter;
}

int main()
{
	const auto data = Utils::read_file("day14_input.txt");
	const auto lines = Utils::split_string(data, '\n');

	// part 1
	auto board = createBoard(lines);
	// drawRangeOfBoard(board);
	auto result = pourSand(board);
	std::cout << "Result is " << result << "\n";
	std::cout << '\n';


	// part 2
	board = createBoard(lines);
	board.board.emplace_back(2 * (board.maxX + 1), '.');
	board.board.emplace_back(2 * (board.maxX + 1), '#');
	board.maxY += 2;
	board.minX = 0;
	board.maxX = 2 * (board.maxX + 1) - 1;
	result = pourSand(board);
	//drawRangeOfBoard(board);

	std::cout << "Result is " << result + 1 << "\n";
	std::cout << '\n';
}
