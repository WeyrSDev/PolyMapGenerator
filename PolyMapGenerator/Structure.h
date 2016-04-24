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