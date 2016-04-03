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

double LineEquation::operator()(const double x) const
{
	return x * m + b;
}

void LineEquation::Move(const Vector2 v)
{
	Vector2 p0, p1;

	if (vertical)
	{
		p0 = Vector2(b, 0);
		p1 = Vector2(b, 1);
	}
	else
	{
		p0 = Vector2(0, b);
		p1 = Vector2(1, m + b);
	}

	p0 += Vector2(v.x, v.y);
	p1 += Vector2(v.x, v.y);

	*this = LineEquation(p0, p1);
}

Vector2 LineEquation::Intersection(LineEquation& e) const
{
	double x, y;

	if (m != e.m)
	{
		if (vertical)
		{
			x = b;
			y = e(x);
		}
		else if (e.vertical)
		{
			x = e.b;
			y = x * m + b;
		}
		else
		{
			x = (e.b - b) / (m - e.m);
			y = e(x);
		}
	}
	else
	{
		if (vertical == e.vertical)
		{
			x = 0.0;
			y = 0.0;
		}
		else
		{
			if (vertical)
			{
				x = b;
				y = e.b;
			}
			else
			{
				x = e.b;
				y = b;
			}
		}
	}

	return Vector2(x, y);
}

bool LineEquation::IsHorizontal() const
{
	return !vertical && m == 0;
}

bool LineEquation::IsVertical() const
{
	return vertical;
}
