#include <iostream>
#include <string>
#include <unordered_set>

#include "Utils.h"

unsigned long long int processFileData(const std::string& data, const std::size_t packetSize)
{
	for (std::size_t index = 0; index < data.size() - packetSize - 1; ++index)
	{
		std::string temp = data.substr(index, packetSize);
		auto numUniqeChars = std::unordered_set(std::begin(temp), std::end(temp)).size();
		if (numUniqeChars == packetSize)
		{
			std::cout << "Unique sequence is " << temp << "\n";
			return index + packetSize;
		}
	}
	return 0;
}

int main()
{
	// part 1
	const auto data = Utils::read_file("day06_input.txt");
	auto results = processFileData(data, 4);
	std::cout << "Result is: " << results << "\n";

	// part 2
	results = processFileData(data, 14);
	std::cout << "Result is: " << results << "\n";
}
