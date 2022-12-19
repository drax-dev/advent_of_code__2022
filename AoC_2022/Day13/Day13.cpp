#include <iostream>
#include <stack>

#include "Utils.h"
//
//int parseCharacter(std::string& buf, const char tempChar)
//{
//	int value = -1;
//	if (tempChar == '[')
//	{
//		buf.clear();
//	}
//	else if (tempChar == ',')
//	{
//		if (!buf.empty())
//		{
//			value = std::stoi(buf);
//		}
//		buf.clear();
//	}
//	else if (isdigit(tempChar))
//	{
//		buf += tempChar;
//	}
//	return value;
//}
//
//void parseInputData(const std::vector<std::string>& lines)
//{
//	const auto linesSize = lines.size();
//	for (std::size_t i = 0; i < linesSize - 1; i += 3)
//	{
//		const auto& leftLine = lines[i];
//		const auto& rightLine = lines[i + 1];
//		const auto minSize = std::min(leftLine.size(), rightLine.size());
//		std::string leftBuf, rightBuf;
//		int leftValue{};
//		int rightValue{};
//		for (std::size_t charIndex = 0; charIndex < minSize; ++charIndex)
//		{
//			const auto tempLeftChar = leftLine[charIndex];
//			const auto tempRightChar = rightLine[charIndex];
//			leftValue = parseCharacter(leftBuf, tempLeftChar);
//			rightValue = parseCharacter(rightBuf, tempRightChar);
//
//			if ( leftValue == rightValue)
//			{
//				
//			}
//			else if
//		}
//
//	}
//}

struct Node
{
	std::shared_ptr<Node> parent;
	std::vector<std::shared_ptr<Node>> children;
	int value;
};



struct Tree
{
	std::shared_ptr<Node> root;
};

//void parseInputData(const std::vector<std::string>& lines)
//{
//	std::deque<std::vector<int>> left;
//	std::deque<std::vector<int>> right;
//
//	const auto linesSize = lines.size();
//	for (std::size_t i = 0; i < linesSize - 1; i += 3)
//	{
//		const auto& leftLine = lines[i];
//		for (std::size_t charIndex = 1; charIndex < leftLine.size(); ++charIndex)
//		{
//			std::vector<int> tmpVec;
//			const auto tempLeftChar = leftLine[charIndex];
//			if (tempLeftChar == '[')
//			{
//			}
//			else if(tempLeftChar == ']')
//			{
//				
//			}
//			else if (tempChar == ',')
//			{
//				if (!buf.empty())
//				{
//					value = std::stoi(buf);
//				}
//				buf.clear();
//			}
//			else if (isdigit(tempChar))
//			{
//				buf += tempChar;
//			}
//		}
//
//		const auto& rightLine = lines[i + 1];
//	}
//}

void parseInputData(const std::vector<std::string>& lines)
{
	Tree tree;
	tree.root = std::make_shared<Node>(nullptr);
	std::shared_ptr<Node> currentNode = tree.root;

	const auto linesSize = lines.size();
	std::string buf;
	for (std::size_t i = 3; i < linesSize - 1; i += 3)
	{
		const auto& leftLine = lines[i];
		for (std::size_t charIndex = 1; charIndex < leftLine.size(); ++charIndex)
		{
			if (const auto tempLeftChar = leftLine[charIndex]; tempLeftChar == '[')
			{
				currentNode->children.emplace_back(std::make_shared<Node>());
				currentNode->children.back()->parent = currentNode;
				currentNode = currentNode->children.back();
			}
			else if(tempLeftChar == ']')
			{
				if (!buf.empty())
				{
					const auto value = std::stoi(buf);
					currentNode->children.emplace_back(std::make_shared<Node>());
					currentNode->children.back()->value = value;
					currentNode->children.back()->parent = currentNode;
				}
				buf.clear();
				currentNode = currentNode->parent;
			}
			else if (tempLeftChar == ',')
			{
				if (!buf.empty())
				{
					const auto value = std::stoi(buf);
					currentNode->children.emplace_back(std::make_shared<Node>());
					currentNode->children.back()->value = value;
					currentNode->children.back()->parent = currentNode;
				}
				buf.clear();
			}
			else if (isdigit(tempLeftChar))
			{
				buf += tempLeftChar;
			}
		}

		//const auto& rightLine = lines[i + 1];
	}
}


int main()
{
	// part1
	const auto data = Utils::read_file("day13_small.txt");
	const auto lines = Utils::split_string(data);
	parseInputData(lines);
}
