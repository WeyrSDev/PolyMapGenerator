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

	Vector2& operator+=(const Vector2& v);
	Vector2& operator+=(const double f);

	Vector2& operator-=(const Vector2& v);
	Vector2& operator-=(const double f);

	Vector2& operator*=(const double f);

	Vector2& operator/=(const double f);

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	void Normalize();
	void Reflect(const Vector2& v);
	void Reverse();
	void Truncate(double maxLength);

	void RotateByDegree(double degree);
	void RotateByRadian(double radian);

	double DotProduct(const Vector2& v) const;
	double CrossProduct(const Vector2& v) const;

	double Length() const;
	double LengthSqrt() const;

	double Distance(const Vector2& v) const;
	double DistanceSqrt(const Vector2& v) const;

	double GetAngleByDegree() const;
	double GetAngleByDegree(const Vector2& v) const;
	double GetAngleByRadian() const;
	double GetAngleByRadian(const Vector2& v) const;

	bool Sign(const Vector2& v) const;
	bool IsZero() const;

	double x, y;
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(const double fac, const Vector2& rhs);
Vector2 operator*(const Vector2& lhs, const double fac);
Vector2 operator/(const Vector2& lhs, const double fac);

Vector2 Normalize(const Vector2& v);
Vector2 Reflect(const Vector2& v1, const Vector2& v2);
Vector2 Reverse(const Vector2& v);
Vector2 Truncate(const Vector2& v, double maxLength);

Vector2 RotateByDegree(const Vector2& v, double degree);
Vector2 RotateByRadian(const Vector2& v, double radian);

double Distance(const Vector2& v1, const Vector2& v2);

#endif