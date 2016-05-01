#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>

#include "DelaunayTriangulation.h"
#include "Structure.h"
#include "QuadTree.h"

// Forward Declaration
class Vector2;
namespace noise
{
	namespace module
	{
		class Perlin;
	}
}

class Map
{
public:
	Map() = default;
	Map(int width, int height, double pointSpread, std::string seed);

	~Map() = default;

	Map(const Map& map) = delete;
	Map(Map&& map) = delete;

	Map& operator=(const Map& map) = delete;
	Map& operator=(Map&& map) = delete;

	void Generate();

	void GeneratePolygons();
	void GenerateLand();

	bool LoadFile(std::string fileName);
	bool WriteFile(std::string fileName);

	std::vector<Edge*> GetEdges();
	std::vector<Corner*> GetCorners();
	std::vector<Center*> GetCenters();

	Center* GetCenterAt(Vector2 pos);

private:
	int m_mapWidth;
	int m_mapHeight;
	double m_pointSpread;
	double m_zCoord;
	noise::module::Perlin* m_noiseMap;
	std::string m_seed;
	QuadTree<Center*> m_centersQuadTree;

	std::vector<DelaunayTriangulation::Vertex> m_points;

	std::map<double, std::map<double, Center*>> m_posCenterMap;
	std::vector<Edge*> m_edges;
	std::vector<Corner*> m_corners;
	std::vector<Center*> m_centers;

	static const std::vector<std::vector<BiomeType>> m_elevationMoistureMatrix;
	static std::vector<std::vector<BiomeType>> MakeBiomeMatrix();

	bool IsIsland(Vector2 position);
	void CalculateDownslopes();
	void GenerateRivers();
	void AssignOceanCoastLand();
	void RedistributeElevations();
	void AssignCornerElevations();
	void AssignPolygonElevations();
	void RedistributeMoisture();
	void AssignCornerMoisture();
	void AssignPolygonMoisture();
	void AssignBiomes();

	void GeneratePoints();
	void Triangulate(std::vector<DelaunayTriangulation::Vertex> points);
	void FinishInfo();
	void AddCenter(Center* c);
	Center* GetCenter(Vector2 position);
	void OrderPoints(std::vector<Corner*>& corners);

	std::vector<Corner*> GetLandCorners();
	std::vector<Corner*> GetLakeCorners();
	void LloydRelaxation();
	std::string CreateSeed(int length);

	static unsigned int HashString(std::string seed);
};

#endif