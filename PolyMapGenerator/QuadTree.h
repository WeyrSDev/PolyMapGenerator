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
	QuadTree() :
		m_northWest(nullptr), m_northEast(nullptr),
		m_southEast(nullptr), m_southWest(nullptr),
		m_divided(false), m_branchDepth(0), m_elementsBranch(0) { }
	QuadTree(AABB boundary, int depth) :
		m_boundary(boundary), m_divided(false), m_branchDepth(depth), m_elementsBranch(0) { }

	~QuadTree()
	{
		if (m_divided)
		{
			delete m_northWest;
			delete m_northEast;
			delete m_southEast;
			delete m_southWest;
		}
	}

	QuadTree(const QuadTree& qt) = delete;
	QuadTree(QuadTree&& qt) = delete;

	QuadTree& operator=(const QuadTree& qt) = delete;
	QuadTree& operator=(QuadTree&& qt) = delete;

	bool Insert(const T element, Vector2 pos)
	{
		if (!m_boundary.IsContain(pos))
		{
			return false;
		}

		if (!m_divided)
		{
			if (m_elements.size() < MAX_TREE_DEPTH)
			{
				m_elements.push_back(std::make_pair(element, pos));
				return true;
			}

			Subdivide();
		}

		if (m_northWest->Insert(element, pos) || m_northEast->Insert(element, pos) ||
			m_southEast->Insert(element, pos) || m_southWest->Insert(element, pos))
		{
			return true;
		}

		return false;
	}

	bool Insert(const T element, AABB range)
	{
		if (!m_boundary.IsIntersect(range))
		{
			return false;
		}

		if (!m_divided)
		{
			if (m_elements.size() < 4)
			{
				m_elements.push_back(std::make_pair(element, range));
				return true;
			}

			Subdivide();
		}

		if (m_northWest->m_boundary.IsIntersect(range))
		{
			m_northWest->Insert(element, range);
		}
		if (m_northEast->m_boundary.IsIntersect(range))
		{
			m_northEast->Insert(element, range);
		}
		if (m_southEast->m_boundary.IsIntersect(range))
		{
			m_southEast->Insert(element, range);
		}
		if (m_southWest->m_boundary.IsIntersect(range))
		{
			m_southWest->Insert(element, range);
		}

		return true;
	}

	bool Insert2(const T element, AABB range)
	{
		if (!m_boundary.IsIntersect(range))
		{
			return false;
		}

		m_elementsBranch++;

		if (m_branchDepth == MAX_TREE_DEPTH)
		{
			m_elements.push_back(element);
			m_elementRegions.push_back(range);
			return true;
		}

		if (!m_divided)
		{
			SubDivide2();
		}

		if (m_northWest->m_boundary.IsIntersect(range))
		{
			m_northWest->Insert2(element, range);
		}
		if (m_northEast->m_boundary.IsIntersect(range))
		{
			m_northEast->Insert2(element, range);
		}
		if (m_southEast->m_boundary.IsIntersect(range))
		{
			m_southEast->Insert2(element, range);
		}
		if (m_southWest->m_boundary.IsIntersect(range))
		{
			m_southWest->Insert2(element, range);
		}

		return true;
	}

	std::vector<T> QueryRange(Vector2 pos)
	{
		QuadTree* currentLeaf = this;

		while (currentLeaf->m_divided)
		{
			if (currentLeaf->m_northWest->m_boundary.IsContain(pos))
			{
				currentLeaf = currentLeaf->m_northWest;
			}
			else if (currentLeaf->m_northEast->m_boundary.IsContain(pos))
			{
				currentLeaf = currentLeaf->m_northEast;
			}
			else if (currentLeaf->m_southEast->m_boundary.IsContain(pos))
			{
				currentLeaf = currentLeaf->m_southEast;
			}
			else if (currentLeaf->m_southWest->m_boundary.IsContain(pos))
			{
				currentLeaf = currentLeaf->m_southWest;
			}
			else
			{
				return std::vector<T>();
			}
		}

		std::vector<T> elements;

		for (int i = 0 ; i < currentLeaf->m_elements.size(); ++i)
		{
			if (currentLeaf->m_elementRegions[i].IsContain(pos))
			{
				elements.push_back(currentLeaf->m_elements[i]);
			}
		}

		return elements;
	}

	static void SetMaxDepth(int d)
	{
		if (d > 0)
		{
			MAX_TREE_DEPTH = d;
		}
	}

	AABB m_boundary;

private:
	void Subdivide()
	{
		m_divided = true;

		Vector2 newHalf = m_boundary.m_half / 2;

		Vector2 nwPos(m_boundary.m_pos - newHalf);
		AABB northWest(nwPos, newHalf);
		m_northWest = new QuadTree<T>(northWest, m_branchDepth + 1);

		Vector2 nePos(nwPos.x + m_boundary.m_half.x, nwPos.y);
		AABB northEast(nePos, newHalf);
		m_northEast = new QuadTree<T>(northEast, m_branchDepth + 1);

		Vector2 sePos(m_boundary.m_pos + newHalf);
		AABB southEast(sePos, newHalf);
		m_southEast = new QuadTree<T>(southEast, m_branchDepth + 1);

		Vector2 swPos(nwPos.x, nwPos.y + m_boundary.m_half.y);
		AABB southWest(swPos, newHalf);
		m_southWest = new QuadTree<T>(southWest, m_branchDepth + 1);

		typename std::vector<std::pair<T, AABB>>::iterator iter;

		for (iter = m_elements.begin(); iter != m_elements.end(); ++iter)
		{
			if (m_northWest->m_boundary.IsIntersect(iter->second))
			{
				m_northWest->Insert(iter->first, iter->second);
			}
			if (m_northEast->m_boundary.IsIntersect(iter->second))
			{
				m_northEast->Insert(iter->first, iter->second);
			}
			if (m_southEast->m_boundary.IsIntersect(iter->second))
			{
				m_southEast->Insert(iter->first, iter->second);
			}
			if (m_southWest->m_boundary.IsIntersect(iter->second))
			{
				m_southWest->Insert(iter->first, iter->second);
			}
		}

		m_elements.clear();
	}

	void Subdivide2()
	{
		m_divided = true;

		Vector2 newHalf = m_boundary.m_half / 2;

		Vector2 nwPos(m_boundary.m_pos - newHalf);
		AABB northWest(nwPos, newHalf);
		m_northWest = new QuadTree<T>(northWest, m_branchDepth + 1);

		Vector2 nePos(nwPos.x + m_boundary.m_half.x, nwPos.y);
		AABB northEast(nePos, newHalf);
		m_northEast = new QuadTree<T>(northEast, m_branchDepth + 1);

		Vector2 sePos(m_boundary.m_pos + newHalf);
		AABB southEast(sePos, newHalf);
		m_southEast = new QuadTree<T>(southEast, m_branchDepth + 1);

		Vector2 swPos(nwPos.x, nwPos.y + m_boundary.m_half.y);
		AABB southWest(swPos, newHalf);
		m_southWest = new QuadTree<T>(southWest, m_branchDepth + 1);
	}

	static int MAX_TREE_DEPTH;

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

template <typename T>
int QuadTree<T>::MAX_TREE_DEPTH = 6;

// Axis-Aligned Bounding Box (AABB)
struct AABB
{
	AABB() = default;
	AABB(Vector2 pos, Vector2 half) : m_pos(pos), m_half(half) { }

	~AABB() = default;

	AABB(const AABB& aabb) = default;
	AABB(AABB&& aabb) = default;

	AABB& operator=(const AABB& aabb) = default;
	AABB& operator=(AABB&& aabb) = default;

	bool IsContain(const Vector2 point) const
	{
		Vector2 minPoint = m_pos - m_half;
		if (point.x >= minPoint.x && point.y >= minPoint.y)
		{
			Vector2 maxPoint = m_pos + m_half;
			return point.x <= maxPoint.x && point.y <= maxPoint.y;
		}

		return false;
	}

	bool IsIntersect(const AABB& sec) const
	{
		double diffX = abs(m_pos.x - sec.m_pos.x);
		double diffY = abs(m_pos.y - sec.m_pos.y);

		if (diffX > m_half.x + sec.m_half.x || diffY > m_half.y + sec.m_half.y)
		{
			return false;
		}

		return true;
	}

	Vector2 m_pos;
	Vector2 m_half;
};

#endif