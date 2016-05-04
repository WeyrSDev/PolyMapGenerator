#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H

#include <set>
#include <cassert>

namespace DelaunayTriangulation
{
	const double EPSILON = 1.192092896e-07F;

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

		bool operator==(const Vertex& v) const
		{
			return m_point.x == v.m_point.x && m_point.y == v.m_point.y;
		}

		bool operator<(const Vertex& v) const
		{
			if (m_point.x == v.m_point.x) return m_point.y < v.m_point.y;
			return m_point.x < v.m_point.x;
		}

		double GetX() const { return m_point.x; }
		double GetY() const { return m_point.y; }

		void SetX(double x) { m_point.x = x; }
		void SetY(double y) { m_point.y = y; }

		const Point& GetPoint() const { return m_point; }

	private:
		Point m_point;
	};

	using VertexSet = std::set<Vertex>;
	using VertexIterator = std::set<Vertex>::iterator;
	using cVertexIterator = std::set<Vertex>::const_iterator;

	class Triangle
	{
	public:
		Triangle() : m_center(0.0, 0.0), m_r(0.0), m_rSquare(0.0) { }
		Triangle(const Vertex* p0, const Vertex* p1, const Vertex* p2) : m_center(0.0, 0.0), m_r(0.0), m_rSquare(0.0)
		{
			m_vertices[0] = p0;
			m_vertices[1] = p1;
			m_vertices[2] = p2;

			SetCircumstanceCircle();
		}
		Triangle(const Vertex* pVertex)
		{
			for (int i = 0; i < 3; ++i)
			{
				m_vertices[i] = pVertex++;
			}

			SetCircumstanceCircle();
		}

		~Triangle()
		{
			for (int i = 0; i < 3; ++i)
			{
				m_vertices[i] = nullptr;
			}

			m_center = Point(0.0, 0.0);
			m_r = 0.0;
			m_rSquare = 0.0;
		}

		Triangle(const Triangle& tri) : m_center(tri.m_center), m_r(tri.m_r), m_rSquare(tri.m_rSquare)
		{
			for (int i = 0; i < 3; ++i)
			{
				m_vertices[i] = tri.m_vertices[i];
			}
		}
		Triangle(Triangle&& tri) : m_center(tri.m_center), m_r(tri.m_r), m_rSquare(tri.m_rSquare)
		{
			for (int i = 0; i < 3; ++i)
			{
				m_vertices[i] = std::move(tri.m_vertices[i]);
			}
		}

		Triangle& operator=(const Triangle& tri) = delete;
		Triangle& operator=(Triangle&& tri) = delete;

		bool operator<(const Triangle& tri) const
		{
			if (m_center.x == tri.m_center.x)
			{
				return m_center.y < tri.m_center.y;
			}

			return m_center.x < m_center.y;
		}

		const Vertex* GetVertex(int i) const
		{
			assert(i >= 0);
			assert(i < 3);

			return m_vertices[i];
		}

		bool IsLeftOf(cVertexIterator iterVertex) const
		{
			return iterVertex->GetPoint().x > m_center.x + m_r;
		}

		bool CCEncompasses(cVertexIterator iterVertex) const
		{
			Point dist = iterVertex->GetPoint() - m_center;
			double distSquare = dist.x * dist.x + dist.y * dist.y;

			return distSquare <= m_rSquare;
		}
	
	private:
		const Vertex* m_vertices[3];
		Point m_center;
		double m_r;
		double m_rSquare;

		void SetCircumstanceCircle();
	};

	using TriangleSet = std::multiset<Triangle>;
	using TriangleIterator = std::multiset<Triangle>::iterator;
	using cTriangleIterator = std::multiset<Triangle>::const_iterator;

	class Edge
	{
	public:
		Edge() : m_pv0(nullptr), m_pv1(nullptr) { }
		Edge(const Vertex* pv0, const Vertex* pv1) : m_pv0(pv0), m_pv1(pv1) { }

		~Edge()
		{
			m_pv0 = nullptr;
			m_pv1 = nullptr;
		}

		Edge(const Edge& e) : m_pv0(e.m_pv0), m_pv1(e.m_pv1) { }
		Edge(Edge&& e) : m_pv0(e.m_pv0), m_pv1(e.m_pv1) { }

		Edge& operator=(const Edge& e) = delete;
		Edge& operator=(Edge&& e) = delete;

		bool operator<(const Edge& e) const
		{
			if (*m_pv0 == *e.m_pv0)
			{
				return *m_pv1 < *e.m_pv1;
			}

			return *m_pv0 < *m_pv0;
		}

		const Vertex* m_pv0;
		const Vertex* m_pv1;
	};

	using EdgeSet = std::set<Edge>;
	using EdgeIterator = std::set<Edge>::iterator;
	using cEdgeIterator = std::set<Edge>::const_iterator;

	class Delaunay
	{
	public:
		void Triangulate(const VertexSet& vertices, TriangleSet& output);
		void TrianglesToEdges(const TriangleSet& triangles, EdgeSet& edges);

	private:
		void HandleEdge(const Vertex* p0, const Vertex* p1, EdgeSet& edges);
	};
}

#endif