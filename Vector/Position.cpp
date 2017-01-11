#include "Position.h"

Position::Position()
	: x(0)
	, y(0)
{

}

Position::Position(double x, double y)
	: x(x)
	, y(y)
{

}

Position::Position(CoordinateDouble obj)
	: x(obj.x)
	, y(obj.y)
{

}

Position Position::normalise()
{
	return this->truncate(1);
}

double Position::get_length()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

Position Position::operator*(const int multiplication)
{
	return *this * (double)multiplication;
}

Position Position::operator*(const double multiplication)
{
	return{
		this->x * multiplication,
		this->y * multiplication
	};
}

Position Position::operator+(Position other)
{
	return{
		this->x + other.x,
		this->y + other.y
	};
}

Position Position::operator-(Position other)
{
	return{
		this->x - other.x,
		this->y - other.y
	};
}

Position Position::truncate(double value)
{
	double length = this->get_length();
	double div_value = length / value;

	return Position{
		this->x / div_value,
		this->y / div_value
	};
}

Position Position::perpendicular()
{
	return{
		this->x * -1,
		this->y
	};
}

Position Position::operator/(const double division)
{
	return{
		this->x / division,
		this->y / division
	};
}

