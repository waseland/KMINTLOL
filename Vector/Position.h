#pragma once
#include "../Domain/CoordinateDouble.h"
#include <cmath>

struct Position {
	Position();
	Position(double x, double y);
	Position(CoordinateDouble obj);

	double x;
	double y;

	Position normalise();
	Position truncate(double value);
	Position perpendicular();
	double get_length();
	Position operator*(const int multiplication);
	Position operator*(const double multiplication);
	Position operator/(const double division);
	Position operator+(Position other);
	Position operator-(Position other);
};
