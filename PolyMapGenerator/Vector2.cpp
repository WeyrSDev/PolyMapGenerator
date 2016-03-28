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

Vector2::Vector2(const Vector2& v) :
	x(v.x),
	y(v.y)
{

}

Vector2::Vector2(Vector2&& v) :
	x(v.x),
	y(v.y)
{

}

Vector2& Vector2::operator=(const Vector2& v)
{
	if (&v == this)
	{
		return *this;
	}

	x = v.x;
	y = v.y;

	return *this;
}

Vector2& Vector2::operator=(Vector2&& v)
{
	if (&v == this)
	{
		return *this;
	}

	x = v.x;
	y = v.y;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2& Vector2::operator+=(const double f)
{
	x += f;
	y += f;

	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Vector2& Vector2::operator-=(const double f)
{
	x -= f;
	y -= f;

	return *this;
}

Vector2& Vector2::operator*=(const double f)
{
	x *= f;
	y *= f;

	return *this;
}

Vector2& Vector2::operator/=(const double f)
{
	x /= f;
	y /= f;

	return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
	double diffX = abs(x - v.x);
	double diffY = abs(y - v.y);

	return diffX < EQ_THRESHOLD && diffY < EQ_THRESHOLD;
}

bool Vector2::operator!=(const Vector2& v) const
{
	return !(*this == v);
}

void Vector2::Normalize()
{
	double mod = Length();

	if (mod > 0)
	{
		x /= mod;
		y /= mod;
	}
}

void Vector2::Reflect(const Vector2& v)
{
	*this += 2.0 * DotProduct(v) * Vector2(v.x * -1, v.y * -1);
}

void Vector2::Reverse()
{
	x *= -1;
	y *= -1;
}

void Vector2::Truncate(double maxLength)
{
	if (Length() > maxLength)
	{
		Normalize();
		*this *= maxLength;
	}
}

void Vector2::RotateByDegree(double degree)
{
	RotateByRadian(degree * M_PI / 180);
}

void Vector2::RotateByRadian(double radian)
{
	double newX = x * cos(radian) - y * sin(radian);
	double newY = x * sin(radian) + y * cos(radian);

	x = newX;
	y = newY;
}

double Vector2::DotProduct(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

double Vector2::CrossProduct(const Vector2& v) const
{
	return x * v.y - v.x * y;
}

double Vector2::Length() const
{
	return sqrt(x * x + y * y);
}

double Vector2::LengthSqrt() const
{
	return x * x + y * y;
}

double Vector2::Distance(const Vector2& v) const
{
	Vector2 dist(*this, v);
	return dist.Length();
}

double Vector2::DistanceSqrt(const Vector2& v) const
{
	Vector2 dist(*this, v);
	return dist.LengthSqrt();
}

double Vector2::GetAngleByDegree() const
{
	return GetAngleByRadian() * 180 / M_PI;
}

double Vector2::GetAngleByDegree(const Vector2& v) const
{
	return GetAngleByRadian(v) * 180 / M_PI;
}