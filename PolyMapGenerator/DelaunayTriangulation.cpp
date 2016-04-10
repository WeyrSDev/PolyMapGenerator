#include "DelaunayTriangulation.h"

namespace DelaunayTriangulation
{
	const double sqtr3 = 1.732050808;

	void Triangle::SetCircumstanceCircle()
	{
		double x0 = m_vertices[0]->GetX();
		double y0 = m_vertices[0]->GetY();

		double x1 = m_vertices[1]->GetX();
		double y1 = m_vertices[1]->GetY();

		double x2 = m_vertices[2]->GetX();
		double y2 = m_vertices[2]->GetY();
	}

}