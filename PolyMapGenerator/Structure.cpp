#include "Math/LineEquation.h"
#include "Structure.h"

Edge::Edge(unsigned int index, Center* center1, Center* center2, Corner* corner1, Corner* corner2) :
	m_index(index), m_d0(center1), m_d1(center2), m_v0(corner1), m_v1(corner2), m_riverVolume(0.0)
{

}

bool Center::RemoveEdge(Edge* e)
{
	for (auto edgeIter = m_edges.begin(); edgeIter != m_edges.end(); ++edgeIter)
	{
		if (*edgeIter == e)
		{
			m_edges.erase(edgeIter);
			return true;
		}
	}

	return false;
}

bool Center::RemoveCorner(Corner* c)
{
	for (auto iter = m_corners.begin(); iter != m_corners.end(); ++iter)
	{
		if (*iter == c)
		{
			m_corners.erase(iter);
			return true;
		}
	}

	return false;
}

Edge* Center::GetEdgeWith(Center* c)
{
	for (auto iter = m_edges.begin(); iter != m_edges.end(); ++iter)
	{
		if ((*iter)->m_d0 == c || (*iter)->m_d1 == c)
		{
			return *iter;
		}
	}

	return nullptr;
}

void Center::MakeBorder()
{
	m_border = true;
	m_water = true;
	m_ocean = true;

	for (auto iter = m_corners.begin(); iter != m_corners.end(); ++iter)
	{
		(*iter)->m_border = true;
		(*iter)->m_water = true;
		(*iter)->m_ocean = true;
	}
}

bool Center::IsInsideBoundingBox(int width, int height) const
{
	if (m_position.x < 0 || m_position.x >= width || m_position.y < 0 || m_position.y >= height)
	{
		return false;
	}

	return true;
}

bool Center::IsContain(Vector2 pos)
{
	if (m_corners.size() < 3)
	{
		return false;
	}

	Vector2 firstSec(m_corners[0]->m_position, m_corners[1]->m_position);
	Vector2 firstPos(m_corners[0]->m_position, pos);
	bool sign = firstSec.CrossProduct(firstPos) > 0;

	for (auto iter = m_corners.begin() + 1; iter != m_corners.end() - 1; ++iter)
	{
		Vector2 ab((*iter)->m_position, (*(iter + 1))->m_position);
		Vector2 ap((*iter)->m_position, pos);

		if (sign != ab.CrossProduct(ap) > 0)
		{
			return false;
		}
	}

	return true;
}

std::pair<Vector2, Vector2> Center::GetBoundingBox()
{
	double minX = m_corners[0]->m_position.x, maxX = m_corners[0]->m_position.x;
	double minY = m_corners[0]->m_position.y, maxY = m_corners[0]->m_position.y;

	for (auto iter = m_corners.begin() + 1; iter != m_corners.end(); ++iter)
	{
		if ((*iter)->m_position.x < minX)
		{
			minX = (*iter)->m_position.x;
		}
		else if ((*iter)->m_position.x > maxX)
		{
			maxX = (*iter)->m_position.x;
		}

		if ((*iter)->m_position.y < minY)
		{
			minY = (*iter)->m_position.y;
		}
		else if ((*iter)->m_position.y > maxY)
		{
			maxY = (*iter)->m_position.y;
		}
	}

	Vector2 minPos(minX, minY);
	Vector2 maxPos(maxX, maxY);
	Vector2 halfDiagonal(Vector2(minPos, maxPos) / 2);

	return std::make_pair(minPos + halfDiagonal, halfDiagonal);
}

void Center::SortCorners()
{
	Corner* item = nullptr;
	int hole = 0;

	for (int i = 1; i < m_corners.size(); ++i)
	{
		item = m_corners[i];
		hole = i;
		
		while (hole > 0 && IsGoesBefore(item->m_position, m_corners[hole - 1]->m_position))
		{
			m_corners[hole] = m_corners[hole - 1];
			hole--;
		}

		m_corners[hole] = item;
	}
}

bool Center::IsGoesBefore(Vector2 a, Vector2 b) const
{
	if ((a - m_position).x >= 0 && (b - m_position).x < 0)
	{
		return true;
	}

	if (a.x == 0 && b.x == 0)
	{
		return a.y < b.y;
	}

	Vector2 ca(m_position, a);
	Vector2 cb(m_position, b);

	return ca.CrossProduct(cb) > 0;
}

bool Edge::Legalize()
{
	if (m_v0 == nullptr || m_v1 == nullptr)
	{
		return false;
	}

	if (m_v0->IsPointInCircumstanceCircle(m_v1->GetOppositeCenter(this)->m_position))
	{
		return this->Flip();
	}

	return false;
}

bool Edge::Flip()
{
	Center* center0 = m_v0->GetOppositeCenter(m_d0, m_d1);
	Center* center1 = m_v1->GetOppositeCenter(m_d0, m_d1);

	if (center0 == nullptr || center1 == nullptr)
	{
		return false;
	}

	Edge* e00 = center0->GetEdgeWith(m_d0);
	Edge* e01 = center0->GetEdgeWith(m_d1);
	Edge* e10 = center1->GetEdgeWith(m_d0);
	Edge* e11 = center1->GetEdgeWith(m_d1);

	e00->SwitchCorner(m_v0, m_v1);
	e11->SwitchCorner(m_v1, m_v0);

	m_d0->RemoveEdge(this);
	m_d1->RemoveEdge(this);

	m_d0->RemoveCorner(m_v0);
	m_d1->RemoveCorner(m_v1);

	center0->m_corners.emplace_back(m_v1);
	center1->m_corners.emplace_back(m_v0);

	center0->m_edges.emplace_back(this);
	center1->m_edges.emplace_back(this);

	m_v0->m_centers.clear();
	m_v0->m_edges.clear();

	m_v0->m_centers.emplace_back(center0);
	m_v0->m_centers.emplace_back(center1);
	m_v0->m_centers.emplace_back(m_d1);
	m_v0->m_position = m_v0->CalculateCircumstanceCenter();

	m_v0->m_edges.emplace_back(this);
	m_v0->m_edges.emplace_back(e01);
	m_v0->m_edges.emplace_back(e11);

	m_v1->m_centers.clear();
	m_v1->m_edges.clear();

	m_v1->m_centers.emplace_back(center0);
	m_v1->m_centers.emplace_back(m_d0);
	m_v1->m_centers.emplace_back(center1);
	m_v1->m_position = m_v1->CalculateCircumstanceCenter();

	m_v1->m_edges.emplace_back(this);
	m_v1->m_edges.emplace_back(e00);
	m_v1->m_edges.emplace_back(e10);

	m_d0 = center0;
	m_d1 = center1;

	e00->Legalize();
	e01->Legalize();
	e10->Legalize();
	e11->Legalize();

	return true;
}

void Edge::SwitchCorner(Corner* oldCorner, Corner* newCorner)
{
	if (m_v0 == oldCorner)
	{
		m_v0 = newCorner;
	}
	else if (m_v1 == oldCorner)
	{
		m_v1 = newCorner;
	}
}

Corner* Edge::GetOppositeCorner(Corner* c) const
{
	if (m_v0 == c)
	{
		return m_v1;
	}
	
	if (m_v1 == c)
	{
		return m_v0;
	}

	return nullptr;
}

Center* Edge::GetOppositeCenter(Center* c) const
{
	if (m_d0 == c)
	{
		return m_d1;
	}

	if (m_d1 == c)
	{
		return m_d0;
	}

	return nullptr;
}

bool Corner::IsPointInCircumstanceCircle(Vector2 p)
{
	if (m_centers.empty())
	{
		return false;
	}

	Center* pointCircumference = m_centers[0];

	Vector2 cornerCenter(m_position, pointCircumference->m_position);
	Vector2 cornerP(m_position, p);

	return cornerCenter.Length() >= cornerP.Length();
}

Vector2 Corner::CalculateCircumstanceCenter()
{
	if (m_centers.size() != 3)
	{
		return Vector2();
	}

	Vector2 a = m_centers[0]->m_position;
	Vector2 b = m_centers[1]->m_position;
	Vector2 c = m_centers[2]->m_position;

	Vector2 abMidpoint((a.x + b.x) / 2, (a.y + b.y) / 2);
	LineEquation abEquation(a, b);

	LineEquation abBisector;
	if (abEquation.IsVertical())
	{
		abBisector = LineEquation(abMidpoint, Vector2(abMidpoint.x + 1, abMidpoint.y));
	}
	else if (abEquation.IsHorizontal())
	{
		abBisector = LineEquation(abMidpoint, Vector2(abMidpoint.x, abMidpoint.y + 1));
	}
	else
	{
		double abBisectorSlope = abEquation.m == 0 ? 0 : -1 / abEquation.m;
		abBisector = LineEquation(abMidpoint, abBisectorSlope);
	}

	Vector2 bcMidpoint((b.x + c.x) / 2, (b.y + c.y) / 2);
	LineEquation bcEquation(b, c);

	LineEquation bcBisector;
	if (bcEquation.IsVertical())
	{
		bcBisector = LineEquation(bcMidpoint, Vector2(bcMidpoint.x + 1, bcMidpoint.y));
	}
	else if (bcEquation.IsHorizontal())
	{
		bcBisector = LineEquation(bcMidpoint, Vector2(bcMidpoint.x, bcMidpoint.y + 1));
	}
	else
	{
		double bcBisectorSlope = bcEquation.m == 0 ? 0 : -1 / bcEquation.m;
		bcBisector = LineEquation(bcMidpoint, bcBisectorSlope);
	}

	return abBisector.Intersection(bcBisector);
}

Center* Corner::GetOppositeCenter(Center* c0, Center* c1)
{
	for (auto center : m_centers)
	{
		if (center != c0 && center != c1)
		{
			return center;
		}
	}

	return nullptr;
}

void SwitchAdjacent(Corner* oldCorner, Corner* newCorner);
bool TouchesCenter(Center* c);
Edge* GetEdgeConnecting(Center* c0, Center* c1);
Center* GetOppositeCenter(Edge* e);
bool IsInsideBoundingBox(Corner* c);
Edge* GetEdgeWith(Corner* c);

static bool SortByElevation(Corner* c1, Corner* c2);
static bool SortByMoisture(Corner* c1, Corner* c2);