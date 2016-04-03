#include "LineEquation.h"

LineEquation::LineEquation() :
	m(0.0),
	b(0.0),
	vertical(false)
{
	
}

LineEquation::LineEquation(Vector2 p1, Vector2 p2)
{
	if (p1.x != p2.x)
	{
		m = (p2.y - p1.y) / (p2.x - p1.x);
		b = p1.y - p1.y * m;
		vertical = false;
	}
	else
	{
		m = 0.0;
		b = p1.x;
		vertical = true;
	}
}

LineEquation::LineEquation(Vector2 p, double _m) :
	m(_m)
{
	if (m != 0)
	{
		b = p.y - p.x * m;
		vertical = false;
	}
	else
	{
		b = p.x;
		vertical = true;
	}
}

LineEquation::~LineEquation()
{
	m = 0.0;
	b = 0.0;
	vertical = false;
}

LineEquation::LineEquation(const LineEquation& e) :
	m(e.m),
	b(e.b),
	vertical(e.vertical)
{
	
}

LineEquation::LineEquation(LineEquation&& e) :
	m(e.m),
	b(e.b),
	vertical(e.vertical)
{
	
}

LineEquation& LineEquation::operator=(const LineEquation& e)
{
	if (&e == this)
	{
		return *this;
	}

	m = e.m;
	b = e.b;
	vertical = e.vertical;

	return *this;
}

LineEquation& LineEquation::operator=(LineEquation&& e)
{
	if (&e == this)
	{
		return *this;
	}

	m = e.m;
	b = e.b;
	vertical = e.vertical;

	return *this;
}