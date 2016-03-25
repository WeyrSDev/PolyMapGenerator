#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2
{
public:
	Vector2();
	Vector2(double angle);
	Vector2(double _x, double _y);
	Vector2(const Vector2& v1, const Vector2& v2);
	
	~Vector2();

	Vector2(const Vector2& v);
	Vector2(Vector2&& v);

	Vector2& operator=(const Vector2& v);
	Vector2& operator=(Vector2&& v);
};

#endif