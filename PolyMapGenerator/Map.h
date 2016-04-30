#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>

#include "DelaunayTriangulation.h"
#include "Structure.h"
#include "QuadTree.h"

using pCenterQT = QuadTree<Center*>;

// Forward Declaration
class Vector2;
namespace Noise
{
	namespace Module
	{
		class Perlin;
	}
}

#endif