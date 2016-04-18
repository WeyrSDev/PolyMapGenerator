#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <SFML/System.hpp>

#include "Math/Vector2.h"

// Forward Declaration
struct AABB;

template <typename T>
class QuadTree
{
public:

private:
	std::vector<T> m_elements;
	std::vector<AABB> m_elementRegions;

	QuadTree* m_northWest;
	QuadTree* m_northEast;
	QuadTree* m_southEast;
	QuadTree* m_southWest;

	bool m_divided;
	int m_branchDepth;
	int m_elementsBranch;
};

#endif