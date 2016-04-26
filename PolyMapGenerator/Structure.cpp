#include "Math/LineEquation.h"
#include "Structure.h"

Edge::Edge(unsigned int index, Center* center1, Center* center2, Corner* corner1, Corner* corner2) :
	m_index(index), m_d0(center1), m_d1(center2), m_v0(corner1), m_v1(corner2), m_riverVolume(0.0)
{
	
}