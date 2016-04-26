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
	bool IsInsideBoundingBox(int width, int height) const;
	bool IsContain(Vector2 pos);
	std::pair<Vector2, Vector2> GetBoundingBox();
	void SortCorners();
	bool IsGoesBefore(Vector2 a, Vector2 b) const;

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

struct Edge
{
	Edge() : m_index(0), m_d0(nullptr), m_d1(nullptr), m_v0(nullptr), m_v1(nullptr), m_riverVolume(0.0) { }
	Edge(unsigned int index, Center* center1, Center* center2, Corner* corner1, Corner* corner2);

	~Edge() = default;

	Edge(const Edge& center) = default;
	Edge(Edge&& center) = default;

	Edge& operator=(const Edge& center) = default;
	Edge& operator=(Edge&& center) = default;

	bool Legalize();
	bool Flip();
	void SwitchCorner(Corner* oldCorner, Corner* newCorner);
	Corner* GetOppositeCorner(Corner* c);
	Center* GetOppositeCenter(Center* c);

	unsigned int m_index;

	Center* m_d0;
	Center* m_d1;
	Corner* m_v0;
	Corner* m_v1;

	Vector2 m_voronoiMidpoint;
	double m_riverVolume;

	using EdgeIterator = std::vector<Edge*>::iterator;
};

struct Corner
{
	Corner() :
		m_index(0), m_position(0, 0), m_water(false), m_ocean(false), m_coast(false), m_border(false),
		m_elevation(0.0), m_moisture(0.0), m_riverVolume(0.0), m_downslope(nullptr) { }
	Corner(unsigned int index, Vector2 position) :
		m_index(index), m_position(position), m_water(false), m_ocean(false), m_coast(false), m_border(false),
		m_elevation(0.0), m_moisture(0.0), m_riverVolume(0.0), m_downslope(nullptr) { }

	bool IsPointInCircumstanceCircle(Vector2 p);
	Vector2 CalculateCircumstanceCenter();
	Center* GetOppositeCenter(Center* c0, Center* c1);
	void SwitchAdjacent(Corner* oldCorner, Corner* newCorner);
	bool TouchesCenter(Center* c);
	Edge* GetEdgeConnecting(Center* c0, Center* c1);
	Center* GetOppositeCenter(Edge* e);
	bool IsInsideBoundingBox(Corner* c);
	Edge* GetEdgeWith(Corner* c);

	static bool SortByElevation(Corner* c1, Corner* c2);
	static bool SortByMoisture(Corner* c1, Corner* c2);

	unsigned int m_index;
	Vector2 m_position;

	bool m_water;
	bool m_ocean;
	bool m_coast;
	bool m_border;
	double m_elevation;
	double m_moisture;
	double m_riverVolume;
	Corner* m_downslope;

	std::vector<Edge*> m_edges;
	std::vector<Corner*> m_corners;
	std::vector<Center*> m_centers;

	using CornerIterator = std::vector<Corner*>::iterator;
};

#endif