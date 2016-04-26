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