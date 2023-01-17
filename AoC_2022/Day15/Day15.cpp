#include <iostream>
#include "Utils.h"
#include "Position2D.h"


struct Sensor
{
	Position2D position;
};

struct Beacon
{
	Position2D position;
};


void parseInput(const std::vector<std::string>& lines)
{
	for (const auto& line : lines)
	{
		const auto splitResult = Utils::split_string(line, ' ');
		const auto sensorX = splitResult[2];
		const auto sensorY = splitResult[3];
		const auto beaconX = splitResult[8];
		const auto beaconY = splitResult[9];


	}
}

int main()
{
	const auto data = Utils::read_file("day15_simple.txt");
	const auto lines = Utils::split_string(data, '\n');
	parseInput(lines);
	// part 1

	// part 2
}
