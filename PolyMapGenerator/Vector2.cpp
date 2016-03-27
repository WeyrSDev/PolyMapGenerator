#include "Vector2.h"

#include <ostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

#ifndef EQ_THRESHOLD
#define EQ_THRESHOLD 0.00001
#endif

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

Vector2::Vector2() :
	x(0.0),
	y(0.0)
{
	
}

Vector2::Vector2(double angle) :
	x(cos(angle * M_PI / 180)),
	y(sin(angle * M_PI / 180))
{
	
}

Vector2::Vector2(double _x, double _y) :
	x(_x),
	y(_y)
{
	
}

Vector2::Vector2(const Vector2& v1, const Vector2& v2) :
	x(v2.x - v1.x),
	y(v2.y - v1.y)
{
	
}

Vector2::~Vector2()
{

}