#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <algorithm>

#include "Structure.h"
#include "Math/Vector2.h"

namespace ConvexHull
{
	inline double Cross(const Vector2& O, const Vector2& A, const Vector2& B)
	{
		return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
	}

	inline std::vector<Corner*> CalculateConvexHull(std::vector<Corner*> P)
	{
		int n = P.size(), k = 0;
		std::vector<Corner*> H(2 * n);

		// Sort points lexicographically
		sort(P.begin(), P.end(), [](Corner* c1, Corner* c2)
		{
			return c1->m_position.x < c2->m_position.x ||
				(c1->m_position.x == c2->m_position.x && c1->m_position.y < c2->m_position.y);
		});

		// Build lower hull
		for (int i = 0; i < n; ++i)
		{
			while (k >= 2 && Cross(H[k - 2]->m_position, H[k - 1]->m_position, P[i]->m_position) <= 0)
			{
				k--;
			}

			H[k++] = P[i];
		}

		// Build upper hull
		for (int i = n - 2, t = k + 1; i >= 0; i--)
		{
			while (k >= t && Cross(H[k - 2]->m_position, H[k - 1]->m_position, P[i]->m_position) <= 0)
			{
				k--;
			}

			H[k++] = P[i];
		}

		H.resize(k - 1);

		return H;
	}
};

#endif