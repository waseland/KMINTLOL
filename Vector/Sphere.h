#pragma once
#include "../Domain/CoordinateDouble.h"

class Sphere {
public:
	Sphere(CoordinateDouble pos, int radius);

	CoordinateDouble position;
	int radius;

	static bool do_intersect(Sphere& a, Sphere& b);
private:
};
