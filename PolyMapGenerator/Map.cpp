#include <chrono>
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