#include "Sphere.h"
#include <cmath>

Sphere::Sphere(CoordinateDouble pos, int radius)
	: position(pos)
	, radius(radius)
{

}

bool Sphere::do_intersect(Sphere& a, Sphere& b)
{
	double dx = a.position.x - b.position.x;
	double dy = a.position.y - b.position.y;
	int combined_radius = a.radius + b.radius;

	return pow(dx, 2) + pow(dy, 2) < pow(combined_radius, 2);
}
