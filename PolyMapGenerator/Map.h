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

class Map
{
public:
	Map();
	Map(int width, int height, double pointSpread, std::string seed);

	~Map();

	Map(const Map& map) = delete;
	Map(Map&& map) = delete;

	Map& operator=(const Map& map) = delete;
	Map& operator=(Map&& map) = delete;
};

#endif