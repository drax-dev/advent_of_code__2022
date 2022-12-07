#include <iostream>
#include <memory>
#include <numeric>

#include "Utils.h"


struct Dir
{
	std::string name;
	std::size_t size;

	std::shared_ptr<Dir> parent;
	std::vector<std::shared_ptr<Dir>> children;

	[[nodiscard]] std::shared_ptr<Dir> findChildByName(const std::string& childName) const
	{
		for (const auto& child : children)
		{
			if (child->name == childName)
			{
				return child;
			}
		}
		throw std::runtime_error("child not found!");
	}
};

struct Command
{
	std::string name;
	std::string arg; // only for cd
	unsigned long long int size; // only for ls;
	std::vector<std::string> dirs; // only for ls;
};

struct Filesystem
{
	std::shared_ptr<Dir> topDir;
	std::shared_ptr<Dir> currentDir;

	void initTopDir()
	{
		topDir = std::make_shared<Dir>("/", 0, nullptr, std::vector<std::shared_ptr<Dir>>());
		currentDir = topDir;
	}

	void executeCommand(const Command& command)
	{
		if (command.name == "cd")
		{
			if(const auto name = command.arg; name == "..")
			{
				const auto currentDirSize = currentDir->size;
				currentDir = currentDir->parent;
				if (currentDir != nullptr)
				{
					currentDir->size += currentDirSize;
				}
			}
			else
			{
				currentDir = currentDir->findChildByName(name);
			}
		}
		else if (command.name == "ls")
		{
			currentDir->size += command.size;

			for (const auto& dir : command.dirs)
			{
				currentDir->children.emplace_back(std::make_shared<Dir>(dir, 0, currentDir, std::vector<std::shared_ptr<Dir>>()));
			}
		}
		else
		{
			throw std::runtime_error("wrong command to execute");
		}
	}
};

std::vector<unsigned long long int> traverseAndCollectSizes(const std::shared_ptr<Dir>& dirPtr)
{
	std::vector<unsigned long long int> results;
	for (const auto& child : dirPtr->children)
	{
		if(child->size <= 100000)
		{
			results.emplace_back(child->size);
		}
		results.append_range(traverseAndCollectSizes(child));
	}
	return results;
}

Command parseCdCommand(const std::string& command)
{
	auto arg = command.substr(4);
	arg = Utils::trim(arg);
	return Command{ std::string{"cd"},std::string{arg}, 0, {} };
}

Command parseLsCommand(const std::vector<std::string>& lines)
{
	const auto linesSize = lines.size();
	unsigned long long int fileSizeSum = 0;
	std::vector<std::string> dirs;
	for (std::size_t lineNumber = 1; lineNumber < linesSize; ++lineNumber)
	{
		if (lines[lineNumber].contains("dir "))
		{
			const auto dirName = lines[lineNumber].substr(4);
			dirs.emplace_back(dirName);
		}
		else
		{
			const auto splittedLine = Utils::split_string(lines[lineNumber], ' ');
			try
			{
				const unsigned long long int fileSize = std::stoll(splittedLine.front());
				std::cout << "file=" << splittedLine.back() << " size=" << splittedLine.front() << "\n";
				fileSizeSum += fileSize;
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
	return Command{ std::string{"ls"},std::string{}, fileSizeSum, dirs };
}

Command parseCommand(const std::string& command)
{
	const auto lines = Utils::split_string(command);
	if(lines.empty())
	{
		throw std::runtime_error("parse error, empty command");
	}

	if (lines.front().contains("$ cd "))
	{
		return parseCdCommand(command);
	}

	if (lines.front().contains("$ ls"))
	{
		return parseLsCommand(lines);
	}

	throw std::runtime_error("wrong command!");
}

int main()
{
	// part 1
	Filesystem filesystem;
	filesystem.initTopDir();
	const auto data = Utils::read_file("day07_input.txt");
	auto lines = Utils::split_string(data);
	lines.erase(lines.begin());

	std::string command;
	for (const auto& line : lines)
	{
		if (line.contains("$ ") && !command.empty())
		{
			const auto parsedCommand = parseCommand(command);
			filesystem.executeCommand(parsedCommand);
			command.clear();
		}
		command += line;
		command += "\n";
	}
	auto parsedCommand = parseCommand(command);
	filesystem.executeCommand(parsedCommand);

	while(filesystem.currentDir->parent != nullptr)
	{
		parsedCommand = Command{std::string{"cd"}, std::string{".."}, 0, {}};
		filesystem.executeCommand(parsedCommand);
	}

	const auto results = traverseAndCollectSizes(filesystem.topDir);
	const auto sum = std::accumulate(results.begin(), results.end(), unsigned long long int{0});
	std::cout << "Result is: " << sum << "\n";
}
