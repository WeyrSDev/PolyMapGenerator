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