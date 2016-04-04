#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H

namespace DelaunayTriangulation
{
	struct Point
	{
		Point() : x(0.0), y(0.0) { }
		Point(double _x, double _y) : x(_x), y(_y) { }

		~Point() { x = 0.0; y = 0.0; }

		Point(const Point& p) : x(p.x), y(p.y) { }
		Point(Point&& p) : x(p.x), y(p.y) { }

		Point& operator=(const Point& p)
		{
			if (this == &p)
			{
				return *this;
			}

			x = p.x;
			y = p.y;

			return *this;
		}

		Point& operator=(Point&& p)
		{
			if (this == &p)
			{
				return *this;
			}

			x = p.x;
			y = p.y;

			return *this;
		}

		Point operator+(const Point& p) const
		{
			return Point(x + p.x, y + p.y);
		}

		Point operator-(const Point& p) const
		{
			return Point(x - p.x, y - p.y);
		}

		double x, y;
	};

	class Vertex
	{
	public:
		Vertex() : m_point(0.0, 0.0) { }
		Vertex(const Point& p) : m_point(p) { }
		Vertex(double x, double y) : m_point(x, y) { }
		Vertex(int x, int y) : m_point(static_cast<double>(x), static_cast<double>(y)) { }
	
		~Vertex() { m_point.x = 0.0; m_point.y = 0.0; }

		Vertex(const Vertex& v) : m_point(v.m_point) { }
		Vertex(Vertex&& v) : m_point(v.m_point) { }

		Vertex& operator=(const Vertex& v)
		{
			if (this == &v)
			{
				return *this;
			}

			m_point = v.m_point;

			return *this;
		}

		Vertex& operator=(Vertex&& v)
		{
			if (this == &v)
			{
				return *this;
			}

			m_point = v.m_point;

			return *this;
		}



	private:
		Point m_point;
	};
}

#endif