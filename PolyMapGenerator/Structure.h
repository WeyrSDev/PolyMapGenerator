#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>

#include "Math/Vector2.h"

enum class BiomeType
{
	Snow,
	Tundra,
	Mountain,
	Taiga,
	Shrubland,
	TemprateDesert,
	TemprateRainForest,
	TemprateDeciduousForest,
	Grassland,
	TropicalRainForest,
	TropicalSeasonalForest,
	SubtropicalDesert,
	Ocean,
	Lake,
	Beach,

	Size,
	None
};

// Forward Declaration
struct Edge;
struct Corner;

struct Center
{
	Center() :
		m_index(0), m_position(0, 0), m_water(false), m_ocean(false), m_coast(false), m_border(false),
		m_biome(BiomeType::None), m_elevation(0.0), m_moisture(0.0) { }
	Center(unsigned int index, Vector2 position) :
		m_index(index), m_position(position), m_water(false), m_ocean(false), m_coast(false), m_border(false),
		m_biome(BiomeType::None), m_elevation(0.0), m_moisture(0.0) { }

	~Center() = default;

	Center(const Center& center) = default;
	Center(Center&& center) = default;

	Center& operator=(const Center& center) = default;
	Center& operator=(Center&& center) = default;

	bool RemoveEdge(Edge* e);
	bool RemoveCorner(Corner* c);
	Edge* GetEdgeWith(Center* c);
	void MakeBorder();
	bool IsInsideBoundingBox(int width, int height);
	bool IsContain(Vector2 pos);
	std::pair<Vector2, Vector2> GetBoundingBox();
	void SortCorners();
	bool IsGoesBefore(Vector2 a, Vector2 b);

	unsigned int m_index;
	Vector2 m_position;

	bool m_water;
	bool m_ocean;
	bool m_coast;
	bool m_border;
	BiomeType m_biome;
	double m_elevation;
	double m_moisture;

	std::vector<Edge*> m_edges;
	std::vector<Corner*> m_corners;
	std::vector<Center*> m_centers;

	using CenterIterator = std::vector<Center*>::iterator;
};

#endif