#include <iostream>
#include <climits>
#include "Utils.h"

struct Position
{
	long long int x;
	long long int y;

	bool operator==(const Position& other) const
	{
		return other.x == x && other.y == y;
	}
};

struct Board
{
	std::vector<std::vector<char>> board;
	Position startPosition;
	Position finishPosition;
	std::size_t xSize;
	std::size_t ySize;

	[[nodiscard]] Position findFirstOccurrence(const char nodeId) const
	{
		for (std::size_t y = 0; y < ySize; ++y)
		{
			for (std::size_t x = 0; x < xSize; ++x)
			{
				if (nodeId == board[y][x])
				{
					return Position{ static_cast<long long int>(x), static_cast<long long int>(y) };
				}
			}
		}
		return {};
	}

	[[nodiscard]] bool isPositionValid(const Position& position, const char currentChar) const
	{
		if (position.x < 0 || position.y < 0 || position.x >= static_cast<long long int>(xSize) || position.y >= static_cast<long long int>(ySize))
		{
			return false;
		}

		auto positionChar = board[position.y][position.x];
		if (positionChar == 'E')
		{
			positionChar = 'z';
		}

		if (currentChar < positionChar && positionChar - currentChar > 1)
		{
			return false;
		}

		return true;
	}
};

Board processFileData(const std::string& data)
{
	Board board{};
	std::vector<char> results;
	for (const auto ch : data)
	{
		if (isalpha(ch))
		{
			results.emplace_back(ch);
		}
		if (ch == '\n')
		{
			board.board.emplace_back(results);
			results.clear();
		}
	}
	board.board.emplace_back(results);
	board.ySize = board.board.size();
	board.xSize = board.board.front().size();

	return board;
}

void printPath(const std::vector<std::vector<bool>>& visited)
{
	std::cout << '\n';
	for (const auto& visitedLine : visited)
	{
		for (const auto node : visitedLine)
		{
			std::cout << node;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}


unsigned long long int findShortestPathRecursive(const Board& board, std::vector<std::vector<bool>>& visited, const Position& nextPosition, unsigned long long int& currentPathLength, unsigned long long int shortestPath)
{
	currentPathLength++;
	visited[nextPosition.y][nextPosition.x] = true;
	if (nextPosition == board.finishPosition)
	{
		if (currentPathLength < shortestPath)
		{
			std::cout << '\n' << "Path length: " << currentPathLength << '\n';
			printPath(visited);

			currentPathLength--;
			visited[nextPosition.y][nextPosition.x] = false;

			return currentPathLength;
		}

		currentPathLength--;
		visited[nextPosition.y][nextPosition.x] = false;
		return shortestPath;
	}

	unsigned long long pathLength;

	char nextChar{ board.board[nextPosition.y][nextPosition.x] };
	if (nextChar == 'S')
	{
		nextChar = 'a';
	}

	// up
	if (const Position upPosition{ nextPosition.x, nextPosition.y - 1 }; board.isPositionValid(upPosition, nextChar) && visited[upPosition.y][upPosition.x] == false)
	{
		pathLength = findShortestPathRecursive(board, visited, upPosition, currentPathLength, shortestPath);
		if (pathLength < shortestPath)
		{
			shortestPath = pathLength;
		}
	}

	// down
	if (const Position downPosition{ nextPosition.x, nextPosition.y + 1 }; board.isPositionValid(downPosition, nextChar) && visited[downPosition.y][downPosition.x] == false)
	{
		pathLength = findShortestPathRecursive(board, visited, downPosition, currentPathLength, shortestPath);
		if (pathLength < shortestPath)
		{
			shortestPath = pathLength;
		}
	}

	// left
	if (const Position leftPosition{ nextPosition.x - 1, nextPosition.y }; board.isPositionValid(leftPosition, nextChar) && visited[leftPosition.y][leftPosition.x] == false)
	{
		pathLength = findShortestPathRecursive(board, visited, leftPosition, currentPathLength, shortestPath);
		if (pathLength < shortestPath)
		{
			shortestPath = pathLength;
		}
	}

	// right
	if (const Position rightPosition{ nextPosition.x + 1, nextPosition.y }; board.isPositionValid(rightPosition, nextChar) && visited[rightPosition.y][rightPosition.x] == false)
	{
		pathLength = findShortestPathRecursive(board, visited, rightPosition, currentPathLength, shortestPath);
		if (pathLength < shortestPath)
		{
			shortestPath = pathLength;
		}
	}
	currentPathLength--;
	visited[nextPosition.y][nextPosition.x] = false;
	return shortestPath;
}

unsigned long long int findShortestPath(const Board& board)
{
	std::vector visited{ board.ySize, std::vector(board.xSize, false) };
	unsigned long long currentLength = 0;
	return findShortestPathRecursive(board, visited, board.startPosition, currentLength, ULLONG_MAX);
}

int main()
{
	const auto data = Utils::read_file("day12_input.txt");
	auto board = processFileData(data);
	board.startPosition = board.findFirstOccurrence('S');
	board.finishPosition = board.findFirstOccurrence('E');
	const auto result = findShortestPath(board);
	std::cout << "\nResult is " << result << "\n";
}
