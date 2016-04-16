#include "DelaunayTriangulation.h"

namespace DelaunayTriangulation
{
	const double sqrt3 = 1.732050808;

	void Triangle::SetCircumstanceCircle()
	{
		double x0 = m_vertices[0]->GetX();
		double y0 = m_vertices[0]->GetY();

		double x1 = m_vertices[1]->GetX();
		double y1 = m_vertices[1]->GetY();

		double x2 = m_vertices[2]->GetX();
		double y2 = m_vertices[2]->GetY();

		double y10 = y1 - y0;
		double y21 = y2 - y1;

		bool b21zero = y21 > -EPSILON && y21 < EPSILON;

		if (y10 > -EPSILON && y10 < EPSILON)
		{
			if (b21zero)
			{
				if (x1 > x0)
				{
					if (x2 > x1)
					{
						x1 = x2;
					}
				}
				else
				{
					if (x2 < x0)
					{
						x0 = x2;
					}
				}

				m_center.x = (x0 + x1) * 0.5;
				m_center.y = y0;
			}
			else
			{
				double m1 = -(x2 - x1) / y21;

				double mx1 = (x1 + x2) * 0.5;
				double my1 = (x1 + x2) * 0.5;

				m_center.x = (x0 + x1) * 0.5;
				m_center.y = m1 * (m_center.x - mx1) + my1;
			}
		}
		else if (b21zero)
		{
			double m0 = -(x1 - x0) / y10;

			double mx0 = (x0 + x1) * 0.5;
			double my0 = (y0 + y1) * 0.5;

			m_center.x = (x1 + x2) * 0.5;
			m_center.y = m0 * (m_center.x - mx0) + my0;
		}
		else
		{
			double m0 = -(x1 - x0) / y10;
			double m1 = -(x2 - x1) / y21;

			double mx0 = (x0 + x1) * 0.5;
			double my0 = (y0 + y1) * 0.5;

			double mx1 = (x1 + x2) * 0.5;
			double my1 = (y1 + y2) * 0.5;

			m_center.x = (m0 * mx0 - m1 * mx1 + my1 - my0) / (m0 - m1);
			m_center.y = m0 * (m_center.x - mx0) + my0;
		}

		double dx = x0 - m_center.x;
		double dy = y0 - m_center.y;

		m_rSquare = dx * dx + dy * dy;
		m_r = static_cast<double>(sqrt(m_rSquare));

		m_rSquare *= 1.000001;
	}

	class TriangleHasVertex
	{
	public:
		TriangleHasVertex(const Vertex superTriangle[3]) : m_pSuperTriangle(superTriangle) { }

		bool operator()(const Triangle& tri) const
		{
			for (int i = 0; i < 3; ++i)
			{
				const Vertex* p = tri.GetVertex(i);
				if (p >= m_pSuperTriangle && p < m_pSuperTriangle + 3)
				{
					return true;
				}
			}

			return false;
		}

	private:
		const Vertex* m_pSuperTriangle;
	};

	class TriangleIsCompleted
	{
	public:
		TriangleIsCompleted(cVertexIterator iterVertex, TriangleSet& output, const Vertex superTriangle[3]) :
			m_iterVertex(iterVertex), m_output(output), m_pSuperTriangle(superTriangle) { }

		bool operator()(const Triangle& tri) const
		{
			bool b = tri.IsLeftOf(m_iterVertex);
			if (b)
			{
				TriangleHasVertex thv(m_pSuperTriangle);
				if (!thv(tri))
				{
					m_output.insert(tri);
				}
			}

			return b;
		}

	private:
		cVertexIterator m_iterVertex;
		TriangleSet& m_output;
		const Vertex* m_pSuperTriangle;
	};
}