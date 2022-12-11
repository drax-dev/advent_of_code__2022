#include <iostream>

#include "Utils.h"

struct Tree
{
	unsigned int height;
	bool isVisible;
};

struct TreeMap
{
	TreeMap() :
		treeMapXSize{}, treeMapYSize{}
	{

	}

	std::vector<std::vector<Tree>> map;
	std::size_t treeMapXSize;
	std::size_t treeMapYSize;
};

TreeMap createTreeMap(const std::vector<std::string>& lines)
{
	TreeMap result;
	std::vector<std::vector<Tree>> map;
	for (const auto& line : lines)
	{
		std::vector<Tree> mapLine;
		for (const auto number : line)
		{
			try
			{
				const auto num = std::stoi(std::string{ number });
				mapLine.emplace_back(num, false);
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
		map.push_back(mapLine);
	}
	result.map = map;
	result.treeMapXSize = map.size();
	result.treeMapYSize = map.front().size();
	return result;
}

bool isVisibleFromLeft(const std::size_t x, const std::size_t y, const TreeMap& treeMap, const unsigned int height)
{
	for (int index = static_cast<int>(y) - 1; index >= 0; --index)
	{
		if (treeMap.map[x][index].height >= height)
		{
			return false;
		}
	}
	return true;
}

bool isVisibleFromRight(const std::size_t x, const std::size_t y, const TreeMap& treeMap, const unsigned int height)
{
	for (int index = static_cast<int>(y) + 1; static_cast<std::size_t>(index) < treeMap.treeMapXSize; ++index)
	{
		if (treeMap.map[x][index].height >= height)
		{
			return false;
		}
	}
	return true;
}

bool isVisibleFromUp(const std::size_t x, const std::size_t y, const TreeMap& treeMap, const unsigned int height)
{
	for (int index = static_cast<int>(x) - 1; index >= 0; --index)
	{
		if (treeMap.map[index][y].height >= height)
		{
			return false;
		}
	}
	return true;
}

bool isVisibleFromDown(const std::size_t x, const std::size_t y, const TreeMap& treeMap, const unsigned int height)
{
	for (int index = static_cast<int>(x) + 1; static_cast<std::size_t>(index) < treeMap.treeMapYSize; ++index)
	{
		if (treeMap.map[index][y].height >= height)
		{
			return false;
		}
	}
	return true;
}

bool isTreeVisibleFromOutside(const std::size_t x, const std::size_t y, const TreeMap& treeMap)
{

	if (x == 0 || y == 0 || x == treeMap.treeMapXSize - 1 || y == treeMap.treeMapYSize - 1)
	{
		return true;
	}
	const auto& [height, isVisible] = treeMap.map[x][y];

	bool isVisibleFlag = false;

	// left
	isVisibleFlag = isVisibleFlag || isVisibleFromLeft(x, y, treeMap, height);

	if (isVisibleFlag)
	{
		return true;
	}

	// right
	isVisibleFlag = isVisibleFlag || isVisibleFromRight(x, y, treeMap, height);

	if (isVisibleFlag)
	{
		return true;
	}

	// up
	isVisibleFlag = isVisibleFlag || isVisibleFromUp(x, y, treeMap, height);

	if (isVisibleFlag)
	{
		return true;
	}

	// down
	isVisibleFlag = isVisibleFlag || isVisibleFromDown(x, y, treeMap, height);

	return isVisibleFlag;
}

void checkTreesVisibility(TreeMap& treeMap)
{
	for (std::size_t x = 0; x < treeMap.treeMapXSize; ++x)
	{
		for (std::size_t y = 0; y < treeMap.treeMapYSize; ++y)
		{
			if (isTreeVisibleFromOutside(x, y, treeMap))
			{
				treeMap.map[x][y].isVisible = true;
			}
		}
	}
}

unsigned long long int countVisibleTrees(const TreeMap& treeMap)
{
	unsigned long long int sum = 0;
	for (const auto& trees : treeMap.map)
	{
		for (const auto& [height, isVisible] : trees)
		{
			if (isVisible)
			{
				sum++;
			}
		}
	}
	return sum;
}

void printVisibilityBoard(const TreeMap& treeMap)
{
	for (const auto& trees : treeMap.map)
	{
		for (const auto& [height, isVisible] : trees)
		{
			std::cout << isVisible;
		}
		std::cout << "\n";
	}
}


unsigned long long int calculateScenicScore(const std::size_t x, const std::size_t y, const TreeMap& treeMap)
{
	const auto& [height, isVisible] = treeMap.map[x][y];
	unsigned long long int left{};
	unsigned long long int right{};
	unsigned long long int up{};
	unsigned long long int down{};

	// left
	for (int index = static_cast<int>(y) - 1; index >= 0; --index)
	{
		left++;
		if (treeMap.map[x][index].height >= height)
		{
			break;
		}
	}

	// right
	for (int index = static_cast<int>(y) + 1; static_cast<std::size_t>(index) < treeMap.treeMapXSize; ++index)
	{
		right++;
		if (treeMap.map[x][index].height >= height)
		{
			break;
		}
	}

	// up
	for (int index = static_cast<int>(x) - 1; index >= 0; --index)
	{
		up++;
		if (treeMap.map[index][y].height >= height)
		{
			break;
		}
	}

	// down
	for (int index = static_cast<int>(x) + 1; static_cast<std::size_t>(index) < treeMap.treeMapYSize; ++index)
	{
		down++;
		if (treeMap.map[index][y].height >= height)
		{
			break;
		}
	}

	return left * right * up * down;
}

unsigned long long int getHighestScenicScore(const TreeMap& treeMap)
{
	unsigned long long int highestScenicScore = 0;
	for (std::size_t x = 0; x < treeMap.treeMapXSize; ++x)
	{
		for (std::size_t y = 0; y < treeMap.treeMapYSize; ++y)
		{
			if (const auto scenicScore = calculateScenicScore(x, y, treeMap); scenicScore > highestScenicScore)
			{
				highestScenicScore = scenicScore;
			}
		}
	}
	return highestScenicScore;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day08_input.txt");
	const auto lines = Utils::split_string(data);
	auto map = createTreeMap(lines);
	checkTreesVisibility(map);
	const auto result = countVisibleTrees(map);
	// printVisibilityBoard(map);
	std::cout << "Result is: " << result << "\n";

	// part 2
	const auto highestScenicScore = getHighestScenicScore(map);
	std::cout << "The tree's highest scenic score is: " << highestScenicScore << "\n";
}
