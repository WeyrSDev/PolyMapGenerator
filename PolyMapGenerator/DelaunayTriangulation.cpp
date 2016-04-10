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
			
		}
	}
}