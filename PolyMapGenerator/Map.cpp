#include <iostream>
#include <random>
#include <queue>
#include <SFML/System.hpp>

#include "Map.h"
#include "PoissonDiskSampling/PoissonDiskSampling.h"
#include "Math/Vector2.h"
#include "Noise/Noise.h"

const std::vector<std::vector<BiomeType>> Map::m_elevationMoistureMatrix = MakeBiomeMatrix();

std::vector<std::vector<BiomeType>> Map::MakeBiomeMatrix()
{
	std::vector<std::vector<BiomeType>> matrix;
	std::vector<BiomeType> biomeVector;

	biomeVector.push_back(BiomeType::SubtropicalDesert);
	biomeVector.push_back(BiomeType::TemprateDesert);
	biomeVector.push_back(BiomeType::TemprateDesert);
	biomeVector.push_back(BiomeType::Mountain);
	matrix.push_back(biomeVector);
	biomeVector.clear();

	biomeVector.push_back(BiomeType::Grassland);
	biomeVector.push_back(BiomeType::Grassland);
	biomeVector.push_back(BiomeType::TemprateDesert);
	biomeVector.push_back(BiomeType::Mountain);
	matrix.push_back(biomeVector);
	biomeVector.clear();

	biomeVector.push_back(BiomeType::TropicalSeasonalForest);
	biomeVector.push_back(BiomeType::Grassland);
	biomeVector.push_back(BiomeType::Shrubland);
	biomeVector.push_back(BiomeType::Tundra);
	matrix.push_back(biomeVector);
	biomeVector.clear();

	biomeVector.push_back(BiomeType::TropicalSeasonalForest);
	biomeVector.push_back(BiomeType::TemprateDeciduousForest);
	biomeVector.push_back(BiomeType::Shrubland);
	biomeVector.push_back(BiomeType::Snow);
	matrix.push_back(biomeVector);
	biomeVector.clear();

	biomeVector.push_back(BiomeType::TropicalRainForest);
	biomeVector.push_back(BiomeType::TemprateDeciduousForest);
	biomeVector.push_back(BiomeType::Taiga);
	biomeVector.push_back(BiomeType::Snow);
	matrix.push_back(biomeVector);
	biomeVector.clear();

	biomeVector.push_back(BiomeType::TropicalRainForest);
	biomeVector.push_back(BiomeType::TemprateRainForest);
	biomeVector.push_back(BiomeType::Taiga);
	biomeVector.push_back(BiomeType::Snow);
	matrix.push_back(biomeVector);
	biomeVector.clear();

	return matrix;
}

Map::Map(int width, int height, double pointSpread, std::string seed) :
	m_mapWidth(width), m_mapHeight(height), m_pointSpread(pointSpread), m_zCoord(0.0),
	m_noiseMap(nullptr), m_seed(seed), m_centersQuadTree(AABB(Vector2(width / 2, height / 2), Vector2(width / 2, height / 2)), 1)
{
	double approxPointCount = (2 * m_mapWidth * m_mapHeight) / (3.1416 * m_pointSpread * m_pointSpread);
	int maxTreeDepth = floor((log(approxPointCount) / log(4)) + 0.5);
	QuadTree<Center*>::SetMaxDepth(maxTreeDepth);

	m_seed = seed != "" ? seed : CreateSeed(20);
	std::mt19937 mt_rand(HashString(m_seed));

	m_zCoord = mt_rand();
	std::cout << "Seed: " << m_seed << "(" << HashString(m_seed) << ")" << std::endl;
}

void Map::Generate()
{
	sf::Clock timer;

	GeneratePolygons();

	std::cout << "Land distribution: ";
	timer.restart();
	GenerateLand();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;


	// Elevation
	std::cout << "Coast assignment: ";
	timer.restart();
	AssignOceanCoastLand();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Corner altitude: ";
	timer.restart();
	AssignCornerElevations();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Altitude redistribution: ";
	timer.restart();
	RedistributeElevations();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Center altitude: ";
	timer.restart();
	AssignPolygonElevations();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	// Moisture
	std::cout << "Downslopes: ";
	timer.restart();
	CalculateDownslopes();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "River generation: ";
	timer.restart();
	GenerateRivers();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Corner moisture: ";
	timer.restart();
	AssignCornerMoisture();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Moisture redistribution: ";
	timer.restart();
	RedistributeMoisture();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Center moisture: ";
	timer.restart();
	AssignPolygonMoisture();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	// Biomes
	std::cout << "Biome assignment: ";
	timer.restart();
	AssignBiomes();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;

	std::cout << "Populate Quadtree: ";
	timer.restart();
	for (auto center : m_centers)
	{
		std::pair<Vector2, Vector2> aabb(center->GetBoundingBox());
		m_centersQuadTree.Insert2(center, AABB(aabb.first, aabb.second));
	}
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;
}