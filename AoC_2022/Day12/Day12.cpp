#include <iostream>
#include <climits>
#include <queue>
#include <unordered_map>

#include "Utils.h"

//struct Position
//{
//	long long int x;
//	long long int y;
//
//	bool operator==(const Position& other) const
//	{
//		return other.x == x && other.y == y;
//	}
//};
//
//struct Board
//{
//	std::vector<std::vector<char>> board;
//	Position startPosition;
//	Position finishPosition;
//	std::size_t xSize;
//	std::size_t ySize;
//
//	[[nodiscard]] Position findFirstOccurrence(const char nodeId) const
//	{
//		for (std::size_t y = 0; y < ySize; ++y)
//		{
//			for (std::size_t x = 0; x < xSize; ++x)
//			{
//				if (nodeId == board[y][x])
//				{
//					return Position{ static_cast<long long int>(x), static_cast<long long int>(y) };
//				}
//			}
//		}
//		return {};
//	}
//
//	[[nodiscard]] bool isPositionValid(const Position& position, const char currentChar) const
//	{
//		if (position.x < 0 || position.y < 0 || position.x >= static_cast<long long int>(xSize) || position.y >= static_cast<long long int>(ySize))
//		{
//			return false;
//		}
//
//		auto positionChar = board[position.y][position.x];
//		if (positionChar == 'E')
//		{
//			positionChar = 'z';
//		}
//
//		if (currentChar < positionChar && positionChar - currentChar > 1)
//		{
//			return false;
//		}
//
//		return true;
//	}
//};
//
//Board processFileData(const std::string& data)
//{
//	Board board{};
//	std::vector<char> results;
//	for (const auto ch : data)
//	{
//		if (isalpha(ch))
//		{
//			results.emplace_back(ch);
//		}
//		if (ch == '\n')
//		{
//			board.board.emplace_back(results);
//			results.clear();
//		}
//	}
//	board.board.emplace_back(results);
//	board.ySize = board.board.size();
//	board.xSize = board.board.front().size();
//
//	return board;
//}
//
//void printPath(const std::vector<std::vector<bool>>& visited)
//{
//	std::cout << '\n';
//	for (const auto& visitedLine : visited)
//	{
//		for (const auto node : visitedLine)
//		{
//			std::cout << node;
//		}
//		std::cout << '\n';
//	}
//	std::cout << '\n';
//}
//
//
//unsigned long long int findShortestPathRecursive(const Board& board, std::vector<std::vector<bool>>& visited, const Position& nextPosition, unsigned long long int& currentPathLength, unsigned long long int shortestPath)
//{
//	currentPathLength++;
//	visited[nextPosition.y][nextPosition.x] = true;
//	if (nextPosition == board.finishPosition)
//	{
//		if (currentPathLength < shortestPath)
//		{
//			std::cout << '\n' << "Path length: " << currentPathLength << '\n';
//			printPath(visited);
//
//			currentPathLength--;
//			visited[nextPosition.y][nextPosition.x] = false;
//
//			return currentPathLength;
//		}
//
//		currentPathLength--;
//		visited[nextPosition.y][nextPosition.x] = false;
//		return shortestPath;
//	}
//
//	unsigned long long pathLength;
//
//	char nextChar{ board.board[nextPosition.y][nextPosition.x] };
//	if (nextChar == 'S')
//	{
//		nextChar = 'a';
//	}
//
//	// up
//	if (const Position upPosition{ nextPosition.x, nextPosition.y - 1 }; board.isPositionValid(upPosition, nextChar) && visited[upPosition.y][upPosition.x] == false)
//	{
//		pathLength = findShortestPathRecursive(board, visited, upPosition, currentPathLength, shortestPath);
//		if (pathLength < shortestPath)
//		{
//			shortestPath = pathLength;
//		}
//	}
//
//	// down
//	if (const Position downPosition{ nextPosition.x, nextPosition.y + 1 }; board.isPositionValid(downPosition, nextChar) && visited[downPosition.y][downPosition.x] == false)
//	{
//		pathLength = findShortestPathRecursive(board, visited, downPosition, currentPathLength, shortestPath);
//		if (pathLength < shortestPath)
//		{
//			shortestPath = pathLength;
//		}
//	}
//
//	// left
//	if (const Position leftPosition{ nextPosition.x - 1, nextPosition.y }; board.isPositionValid(leftPosition, nextChar) && visited[leftPosition.y][leftPosition.x] == false)
//	{
//		pathLength = findShortestPathRecursive(board, visited, leftPosition, currentPathLength, shortestPath);
//		if (pathLength < shortestPath)
//		{
//			shortestPath = pathLength;
//		}
//	}
//
//	// right
//	if (const Position rightPosition{ nextPosition.x + 1, nextPosition.y }; board.isPositionValid(rightPosition, nextChar) && visited[rightPosition.y][rightPosition.x] == false)
//	{
//		pathLength = findShortestPathRecursive(board, visited, rightPosition, currentPathLength, shortestPath);
//		if (pathLength < shortestPath)
//		{
//			shortestPath = pathLength;
//		}
//	}
//	currentPathLength--;
//	visited[nextPosition.y][nextPosition.x] = false;
//	return shortestPath;
//}

//unsigned long long int findShortestPathBruteForce(const Board& board)
//{
//	std::vector visited{ board.ySize, std::vector(board.xSize, false) };
//	unsigned long long currentLength = 0;
//	return findShortestPathRecursive(board, visited, board.startPosition, currentLength, ULLONG_MAX);
//}


struct Point
{
	Point() : x(0), y(0), cost(0), value('a'), priority(0)
	{
	}

	Point(const int y, const int x, const int cost, const char value) :
		x(x), y(y), cost(cost), value(value)
	{
	}

	int x;
	int y;
	int cost;
	char value;
	int priority;

	bool operator==(const Point& other) const
	{
		return this->x == other.x && this->y == other.y && this->cost == other.cost;
	}
};

template <>
struct std::hash<Point>
{
	std::size_t operator()(const Point& p) const noexcept
	{
		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return ((std::hash<int>()(p.x)
			^ (std::hash<int>()(p.y) << 1)) >> 1)
			^ (std::hash<int>()(p.cost) << 1);
	}
};

bool isSafe(const int x, const int y, const int max_x, const int max_y)
{
	return x >= 0 && x < max_x&& y >= 0 && y < max_y;
}

bool isValid(const Point& p, const Point& neighborPoint)
{
	const auto pointValue = p.value;

	if (const auto neighborValue = neighborPoint.value; pointValue < neighborValue && neighborValue - pointValue > 1)
	{
		return false;
	}
	return true;
}

std::vector<std::pair<int, int>> moves{ {1,0},{-1,0},{0,1},{0,-1} };

std::vector<Point> get_neighbors(const Point p, const std::vector<std::vector<Point>>& input_data)
{
	std::vector<Point> neighbors;
	const auto inputDataSizeX = input_data.size();
	const auto inputDataSizeY = input_data.front().size();
	for (const auto& [x, y] : moves)
	{
		if (isSafe(p.x + x, p.y + y, static_cast<int>(inputDataSizeX), static_cast<int>(inputDataSizeY)))
		{
			if (isValid(p, input_data[p.x + x][p.y + y]))
			{
				neighbors.emplace_back(input_data[p.x + x][p.y + y]);
			}
		}
	}
	return neighbors;
}

inline int heuristic(const Point a, const Point b)
{
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct CompareStruct
{
	bool operator() (const Point& lhs, const Point& rhs) const
	{
		return lhs.priority > rhs.priority;
	}
};


void a_star_search(const std::vector<std::vector<Point>>& input_data, const Point start, const Point goal, std::unordered_map<Point, Point>& came_from, std::unordered_map<Point, int>& cost_so_far)
{
	std::priority_queue<Point, std::vector<Point>, CompareStruct> frontier;
	frontier.push(start);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty())
	{
		Point current = frontier.top();
		frontier.pop();

		if (current == goal)
		{
			break;
		}

		for (auto neighbors = get_neighbors(current, input_data);
			auto& next : neighbors)
		{
			if (const int newCost = cost_so_far[current] + next.cost; 
				!cost_so_far.contains(next) || newCost < cost_so_far[next])
			{
				cost_so_far[next] = newCost;
				const int priority = newCost + heuristic(next, goal);
				next.priority = priority;
				frontier.push(next);
				came_from[next] = current;
			}
		}
	}
}


std::vector<std::vector<Point>> parse_input(const std::string& input_data)
{
	std::vector<std::vector<Point>> input;
	std::stringstream ss(input_data);
	std::string line;

	auto lineCounter = 0;
	while (std::getline(ss, line))
	{
		std::vector<Point> transformedLine;
		line = Utils::trim(line);
		const auto lineSize = line.size();
		for (std::size_t i = 0; i < lineSize; i++)
		{
			constexpr auto cost = 1;
			transformedLine.emplace_back(i, lineCounter, cost, line[i]);
		}
		input.push_back(transformedLine);
		lineCounter++;
	}

	return input;
}


 Point findFirstOccurrence(const char nodeId, const std::vector<std::vector<Point>>& parsedInput)
{
	const auto inputDataSizeX = parsedInput.size();
	const auto inputDataSizeY = parsedInput.front().size();
	for (std::size_t x = 0; x < inputDataSizeX; ++x)
	{
		for (std::size_t y = 0; y < inputDataSizeY; ++y)
		{
			if (nodeId == parsedInput[x][y].value)
			{
				return parsedInput[x][y];
			}
		}
	}
	return {};
}

int main()
{
	// part1 brute force
	//const auto data = Utils::read_file("day12_small.txt");
	//auto board = processFileData(data);
	//board.startPosition = board.findFirstOccurrence('S');
	//board.finishPosition = board.findFirstOccurrence('E');
	//const auto result = findShortestPathBruteForce(board);
	//std::cout << "\nResult is " << result << "\n";

	// part1 a star
	const auto data = Utils::read_file("day12_input.txt");
	auto parsedInput = parse_input(data);
	std::unordered_map<Point, Point> came_from;
	std::unordered_map<Point, int> cost_so_far;
	const auto startPoint = findFirstOccurrence('S', parsedInput);
	parsedInput[startPoint.x][startPoint.y].cost = 0; //First point has no cost ! 
	parsedInput[startPoint.x][startPoint.y].value = 'a';
	const auto endPoint = findFirstOccurrence('E', parsedInput);
	parsedInput[endPoint.x][endPoint.y].value = 'z';

	a_star_search(parsedInput, parsedInput[startPoint.x][startPoint.y], parsedInput[endPoint.x][endPoint.y], came_from, cost_so_far);
	std::cout << cost_so_far[parsedInput[endPoint.x][endPoint.y]] << std::endl;
}
