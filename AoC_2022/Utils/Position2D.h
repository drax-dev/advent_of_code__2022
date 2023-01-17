#pragma once

struct Position2D
{
	explicit Position2D(const long long int inputX = 0, const long long int inputY = 0)
		: x(inputX), y(inputY)
	{
	}

	long long int x;
	long long int y;
};
