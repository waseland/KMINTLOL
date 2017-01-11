#pragma once
#include "../Domain/CoordinateInt.h"

struct Position;

struct CoordinateDouble {
	double x;
	double y;

	CoordinateDouble();
	CoordinateDouble(double x, double y);
	CoordinateDouble(int x, int y);
	CoordinateDouble(Position& other);
	CoordinateDouble(CoordinateInt other);

	CoordinateDouble operator+(Position& other);
	CoordinateDouble operator+(const CoordinateDouble& other);
	CoordinateDouble operator-(const CoordinateDouble& other);
	bool operator==(const Position& other);
	bool operator!=(const Position& other);

	CoordinateDouble wrap(int max_x, int max_y);
	double get_raw_distance_to_point(CoordinateDouble pos);
};

