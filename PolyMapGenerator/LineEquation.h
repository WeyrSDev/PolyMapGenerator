#ifndef LINE_EQUATION_H
#define LINE_EQUATION_H

#include "Vector2.h"

class LineEquation
{
public:
	LineEquation();
	LineEquation(Vector2 p1, Vector2 p2);
	LineEquation(Vector2 p, double _m);
	
	~LineEquation();

	LineEquation(const LineEquation& e);
	LineEquation(LineEquation&& e);

	LineEquation& operator=(const LineEquation& e);
	LineEquation& operator=(LineEquation&& e);

	double operator()(const double x) const;

	void Move(const Vector2 v);
	Vector2 Intersection(LineEquation& e) const;
	bool IsHorizontal();
	bool IsVertical();

	double m;
	double b;
	bool vertical;
};

#endif