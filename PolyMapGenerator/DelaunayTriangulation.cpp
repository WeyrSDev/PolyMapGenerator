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

	class VertexIsInCircumstanceCircle
	{
	public:
		VertexIsInCircumstanceCircle(cVertexIterator iterVertex, EdgeSet& edges) :
			m_iterVertex(iterVertex), m_edges(edges) { }
		
		bool operator()(const Triangle& tri) const
		{
			bool b = tri.CCEncompasses(m_iterVertex);
			
			if (b)
			{
				HandleEdge(tri.GetVertex(0), tri.GetVertex(1));
				HandleEdge(tri.GetVertex(1), tri.GetVertex(2));
				HandleEdge(tri.GetVertex(2), tri.GetVertex(0));
			}

			return b;
		}

	private:
		void HandleEdge(const Vertex* p0, const Vertex* p1) const
		{
			const Vertex* pVertex0(nullptr);
			const Vertex* pVertex1(nullptr);

			if (*p0 < *p1)
			{
				pVertex0 = p0;
				pVertex1 = p1;
			}
			else
			{
				pVertex0 = p1;
				pVertex1 = p0;
			}

			Edge e(pVertex0, pVertex1);
			EdgeIterator isFound = m_edges.find(e);

			if (isFound == m_edges.end())
			{
				m_edges.insert(e);
			}
			else
			{
				m_edges.erase(isFound);
			}
		}

		cVertexIterator m_iterVertex;
		EdgeSet& m_edges;
	};

	void Delaunay::Triangulate(const VertexSet& vertices, TriangleSet& output)
	{
		if (vertices.size() < 3)
		{
			return;
		}

		cVertexIterator iterVertex = vertices.begin();

		double xMin = iterVertex->GetX();
		double yMin = iterVertex->GetY();
		double xMax = xMin;
		double yMax = yMin;

		++iterVertex;

		for (; iterVertex != vertices.end(); ++iterVertex)
		{
			xMax = iterVertex->GetX();
			double y = iterVertex->GetY();

			if (y < yMin)
			{
				yMin = y;
			}
			if (y > yMax)
			{
				yMax = y;
			}
		}

		double dx = xMax - xMin;
		double dy = yMax - yMin;

		double ddx = dx * 0.01;
		double ddy = dy * 0.01;

		xMin -= ddx;
		xMax += ddx;
		dx += 2 * ddx;

		yMin -= ddy;
		yMax += ddy;
		dy += 2 * ddy;

		Vertex vSuper[3];
		vSuper[0] = Vertex(xMin - dy * sqrt3 / 3.0, yMin);
		vSuper[1] = Vertex(xMax + dy * sqrt3 / 3.0, yMin);
		vSuper[2] = Vertex((xMin + xMax) * 0.5, yMax + dx * sqrt3 * 0.5);

		TriangleSet workset;
		workset.insert(Triangle(vSuper));

		for (iterVertex = vertices.begin(); iterVertex != vertices.end(); ++iterVertex)
		{
			TriangleIsCompleted pred1(iterVertex, output, vSuper);
			TriangleSet::iterator iter = workset.begin();

			while (iter != workset.end())
			{
				if (pred1(*iter))
				{
					iter = workset.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			EdgeSet edges;

			VertexIsInCircumstanceCircle pred2(iterVertex, edges);
			iter = workset.begin();

			while (iter != workset.end())
			{
				if (pred2(*iter))
				{
					iter = workset.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			for (EdgeIterator edgeIter = edges.begin(); edgeIter != edges.end(); ++edgeIter)
			{
				workset.insert(Triangle(edgeIter->m_pv0, edgeIter->m_pv1, &(*iterVertex)));
			}
		}

		TriangleIterator where = output.begin();
		TriangleHasVertex pred(vSuper);
		for (auto t : workset)
		{
			if (!pred(t))
			{
				output.insert(output.begin(), t);
			}
		}
	}

	void Delaunay::TrianglesToEdges(const TriangleSet& triangles, EdgeSet& edges)
	{
		for (cTriangleIterator iter = triangles.begin(); iter != triangles.end(); ++iter)
		{
			HandleEdge(iter->GetVertex(0), iter->GetVertex(1), edges);
			HandleEdge(iter->GetVertex(1), iter->GetVertex(2), edges);
			HandleEdge(iter->GetVertex(2), iter->GetVertex(0), edges);
		}
	}

	void Delaunay::HandleEdge(const Vertex* p0, const Vertex* p1, EdgeSet& edges)
	{
		const Vertex* pv0(nullptr);
		const Vertex* pv1(nullptr);

		if (*p0 < *p1)
		{
			pv0 = p0;
			pv1 = p1;
		}
		else
		{
			pv0 = p1;
			pv1 = p0;
		}

		edges.insert(Edge(pv0, pv1));
	}
}