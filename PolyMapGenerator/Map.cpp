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

void Map::GeneratePolygons()
{
	sf::Clock timer;

	GeneratePoints();
	std::cout << "Point placement: " << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;
	timer.restart();

	Triangulate(m_points);
	std::cout << "Triangulation: " << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;
	timer.restart();

	FinishInfo();
	std::cout << "Finishing touches: " << timer.getElapsedTime().asMicroseconds() / 1000.0 << " ms." << std::endl;
}

void Map::GenerateLand()
{
	m_noiseMap = new noise::module::Perlin();

	for (auto corner : m_corners)
	{
		if (!corner->IsInsideBoundingBox(m_mapWidth, m_mapHeight))
		{
			corner->m_border = true;
			corner->m_ocean = true;
			corner->m_water = true;
		}
	}

	for (auto corner : m_corners)
	{
		corner->m_water = !IsIsland(corner->m_position);
	}
}

std::vector<Edge*> Map::GetEdges() const
{
	return m_edges;
}

std::vector<Corner*> Map::GetCorners() const
{
	return m_corners;
}

std::vector<Center*> Map::GetCenters() const
{
	return m_centers;
}

Center* Map::GetCenterAt(Vector2 pos)
{
	Center* center = nullptr;
	std::vector<Center*> auxCenters = m_centersQuadTree.QueryRange(pos);

	if (auxCenters.size() > 0)
	{
		double minDist = Vector2(auxCenters[0]->m_position, pos).Length();
		center = auxCenters[0];

		for (auto auxCenter : auxCenters)
		{
			double newDist = Vector2(auxCenter->m_position, pos).Length();
			if (newDist < minDist)
			{
				minDist = newDist;
				center = auxCenter;
			}
		}
	}

	return center;
}

bool Map::IsIsland(Vector2 position)
{
	double waterThreshold = 0.075;

	if (position.x < m_mapWidth * waterThreshold || position.y < m_mapHeight * waterThreshold ||
		position.x > m_mapWidth * (1 - waterThreshold) || position.y > m_mapHeight * (1 - waterThreshold))
	{
		return false;
	}

	Vector2 centerPos = Vector2(m_mapWidth / 2.0, m_mapHeight / 2.0);
	position -= centerPos;

	double xCoord = (position.x / m_mapWidth) * 4;
	double yCoord = (position.y / m_mapHeight) * 4;
	double noiseVal = m_noiseMap->GetValue(xCoord, yCoord, m_zCoord);

	position /= std::min(m_mapWidth, m_mapHeight);
	double radius = position.Length();

	double factor = radius - 0.5;

	return noiseVal >= 0.3 * radius + factor;
}

void Map::CalculateDownslopes()
{
	for (auto c : m_corners)
	{
		Corner* d = c;
		for (auto q : c->m_corners)
		{
			if (q->m_elevation < d->m_elevation)
			{
				d = q;
			}
		}

		c->m_downslope = d;
	}
}

void Map::GenerateRivers()
{
	std::mt19937 mt_rand(HashString(m_seed));
	int numRivers = m_centers.size() / 3;

	for (int i = 0; i < numRivers; ++i)
	{
		Corner* q = m_corners[mt_rand() % m_corners.size()];

		if (q->m_ocean || q->m_elevation < 0.3 || q->m_elevation > 0.9)
		{
			continue;
		}

		while (!q->m_coast)
		{
			if (q == q->m_downslope)
			{
				break;
			}

			Edge* e = q->GetEdgeWith(q->m_downslope);
			e->m_riverVolume += 1;
			q->m_riverVolume += 1;
			q->m_downslope->m_riverVolume += 1;
			q = q->m_downslope;
		}
	}
}

void Map::AssignOceanCoastLand()
{
	std::queue<Center*> centersQueue;

	for (auto c : m_centers)
	{
		int adjacentWater = 0;
		
		for (auto q : c->m_centers)
		{
			if (q->m_border)
			{
				c->m_border = true;
				c->m_ocean = true;
				q->m_water = true;
				centersQueue.push(c);
			}

			if (q->m_water)
			{
				adjacentWater++;
			}
		}

		c->m_water = (c->m_ocean || adjacentWater >= c->m_corners.size() * 0.5);
	}

	while (!centersQueue.empty())
	{
		Center* c = centersQueue.front();
		centersQueue.pop();

		for (auto r : c->m_centers)
		{
			if (r->m_water && !r->m_ocean)
			{
				r->m_ocean = true;
				centersQueue.push(r);
			}
		}
	}

	for (auto p : m_centers)
	{
		int numOcean = 0;
		int numLand = 0;

		for (auto q : p->m_centers)
		{
			numOcean += static_cast<int>(q->m_ocean);
			numLand += static_cast<int>(!q->m_water);
		}

		p->m_coast = numLand > 0 && numOcean > 0;
	}

	for (auto c : m_corners)
	{
		int adjOcean = 0;
		int adjLand = 0;

		for (auto p : c->m_centers)
		{
			adjOcean += static_cast<int>(p->m_ocean);
			adjLand += static_cast<int>(!p->m_water);
		}

		c->m_ocean = adjOcean == c->m_centers.size();
		c->m_coast = adjLand > 0 && adjOcean > 0;
		c->m_water = c->m_border || (adjLand != c->m_centers.size() && !c->m_coast);
	}
}

void Map::RedistributeElevations()
{
	std::vector<Corner*> locations = GetLandCorners();
	const double SCALE_FACTOR = 1.05;

	sort(locations.begin(), locations.end(), &Corner::SortByElevation);

	for (int i = 0; i < locations.size(); ++i)
	{
		double y = static_cast<double>(i) / (locations.size() - 1);
		double x = sqrt(SCALE_FACTOR) - sqrt(SCALE_FACTOR * (1 - y));
		x = std::min(x, 1.0);
		locations[i]->m_elevation = x;
	}
}

void Map::AssignCornerElevations()
{
	std::queue<Corner*> cornersQueue;

	for (auto q : m_corners)
	{
		if (q->m_border)
		{
			q->m_elevation = 0.0;
			cornersQueue.push(q);
		}
		else
		{
			q->m_elevation = 99999;
		}
	}

	while (!cornersQueue.empty())
	{
		Corner* q = cornersQueue.front();
		cornersQueue.pop();

		for (auto s : q->m_corners)
		{
			double newElevation = q->m_elevation + 0.01;

			if (!q->m_water && !s->m_water)
			{
				newElevation += 1;
			}
			
			if (newElevation < s->m_elevation)
			{
				s->m_elevation = newElevation;
				cornersQueue.push(s);
			}
		}
	}

	for (auto q : m_corners)
	{
		if (q->m_water)
		{
			q->m_elevation = 0.0;
		}
	}
}

void Map::AssignPolygonElevations()
{
	for (auto p : m_centers)
	{
		double sumElevation = 0.0;
		for (auto q : p->m_corners)
		{
			sumElevation += q->m_elevation;
		}

		p->m_elevation = sumElevation / p->m_corners.size();
	}
}

void Map::RedistributeMoisture()
{
	std::vector<Corner*> locations = GetLandCorners();

	sort(locations.begin(), locations.end(), &Corner::SortByMoisture);

	for (int i = 0; i < locations.size(); ++i)
	{
		locations[i]->m_moisture = static_cast<double>(i) / (locations.size() - 1);
	}
}

void Map::AssignCornerMoisture()
{
	std::queue<Corner*> cornersQueue;

	for (auto c : m_corners)
	{
		if ((c->m_water || c->m_riverVolume > 0) && !c->m_ocean)
		{
			c->m_moisture = c->m_riverVolume > 0 ? std::min(3.0, 0.2 * c->m_riverVolume) : 1.0;
			cornersQueue.push(c);
		}
		else
		{
			c->m_moisture = 0.0;
		}
	}

	while (!cornersQueue.empty())
	{
		Corner* c = cornersQueue.front();
		cornersQueue.pop();

		for (auto r : c->m_corners)
		{
			double newMoisture = c->m_moisture * 0.9;
			
			if (newMoisture > r->m_moisture)
			{
				r->m_moisture = newMoisture;
				cornersQueue.push(r);
			}
		}
	}

	for (auto r : m_corners)
	{
		if (r->m_ocean)
		{
			r->m_moisture = 1.0;
			cornersQueue.push(r);
		}
	}

	while (!cornersQueue.empty())
	{
		Corner* c = cornersQueue.front();
		cornersQueue.pop();

		for (auto r : c->m_corners)
		{
			double newMoisture = c->m_moisture * 0.3;

			if (newMoisture > r->m_moisture)
			{
				r->m_moisture = newMoisture;
				cornersQueue.push(r);
			}
		}
	}
}

void Map::AssignPolygonMoisture()
{
	for (auto p : m_centers)
	{
		double newMoistrue = 0.0;

		for (auto q : p->m_corners)
		{
			if (q->m_moisture > 1.0)
			{
				q->m_moisture = 1.0;
			}

			newMoistrue += q->m_moisture;
		}

		p->m_moisture = newMoistrue / p->m_corners.size();
	}
}

void Map::AssignBiomes()
{
	for (auto center : m_centers)
	{
		if (center->m_ocean)
		{
			center->m_biome = BiomeType::Ocean;
		}
		else if (center->m_water)
		{
			center->m_biome = BiomeType::Lake;
		}
		else if (center->m_coast && center->m_moisture < 0.6)
		{
			center->m_biome = BiomeType::Beach;
		}
		else
		{
			int elevationIndex = 0;
			
			if (center->m_elevation > 0.85)
			{
				elevationIndex = 3;
			}
			else if (center->m_elevation > 0.6)
			{
				elevationIndex = 2;
			}
			else if (center->m_elevation > 0.3)
			{
				elevationIndex = 1;
			}
			else
			{
				elevationIndex = 0;
			}

			int moistureIndex = std::min(static_cast<int>(floor(center->m_moisture * 6)), 5);
			center->m_biome = m_elevationMoistureMatrix[moistureIndex][elevationIndex];
		}
	}
}

void Map::GeneratePoints()
{
	PoissonDiskSampling pds(800, 600, m_pointSpread, 10);
	std::vector<std::pair<double, double>> newPoints = pds.Generate();
	std::cout << "Generating " << newPoints.size() << " points..." << std::endl;

	for (auto point : newPoints)
	{
		m_points.push_back(DelaunayTriangulation::Vertex(static_cast<int>(point.first), static_cast<int>(point.second)));
	}

	m_points.push_back(DelaunayTriangulation::Vertex(-m_mapWidth, -m_mapHeight));
	m_points.push_back(DelaunayTriangulation::Vertex(2 * m_mapWidth, -m_mapHeight));
	m_points.push_back(DelaunayTriangulation::Vertex(2 * m_mapWidth, 2 * m_mapHeight));
	m_points.push_back(DelaunayTriangulation::Vertex(-m_mapWidth, 2 * m_mapHeight));
}

void Map::Triangulate(std::vector<DelaunayTriangulation::Vertex> points)
{
	int cornerIndex = 0, centerIndex = 0, edgeIndex = 0;
	m_corners.clear();
	m_centers.clear();
	m_edges.clear();
	m_posCenterMap.clear();

	DelaunayTriangulation::VertexSet vertices(points.begin(), points.end());
	DelaunayTriangulation::TriangleSet triangles;
	DelaunayTriangulation::EdgeSet edges;
	DelaunayTriangulation::Delaunay delaunay;

	delaunay.Triangulate(vertices, triangles);

	for (auto t : triangles)
	{
		Vector2 posCenter0(t.GetVertex(0)->GetX(), t.GetVertex(0)->GetY());
		Vector2 posCenter1(t.GetVertex(1)->GetX(), t.GetVertex(1)->GetY());
		Vector2 posCenter2(t.GetVertex(2)->GetX(), t.GetVertex(2)->GetY());

		Center* c1 = GetCenter(posCenter0);
		if (c1 == nullptr)
		{
			c1 = new Center(centerIndex++, posCenter0);
			m_centers.push_back(c1);
			AddCenter(c1);
		}

		Center* c2 = GetCenter(posCenter1);
		if (c2 == nullptr)
		{
			c2 = new Center(centerIndex++, posCenter1);
			m_centers.push_back(c2);
			AddCenter(c2);
		}

		Center* c3 = GetCenter(posCenter2);
		if (c3 == nullptr)
		{
			c3 = new Center(centerIndex++, posCenter2);
			m_centers.push_back(c3);
			AddCenter(c3);
		}

		Corner* c = new Corner(cornerIndex++, Vector2());
		m_corners.push_back(c);
		c->m_centers.push_back(c1);
		c->m_centers.push_back(c2);
		c->m_centers.push_back(c3);
		c1->m_corners.push_back(c);
		c2->m_corners.push_back(c);
		c3->m_corners.push_back(c);
		c->m_position = c->CalculateCircumstanceCenter();

		Edge* e12 = c1->GetEdgeWith(c2);
		if (e12 == nullptr)
		{
			e12 = new Edge(edgeIndex++, c1, c2, nullptr, nullptr);
			e12->m_v0 = c;
			m_edges.push_back(e12);
			c1->m_edges.push_back(e12);
			c2->m_edges.push_back(e12);
		}
		else
		{
			e12->m_v1 = c;
		}
		c->m_edges.push_back(e12);

		Edge* e23 = c2->GetEdgeWith(c3);
		if (e23 == nullptr)
		{
			e23 = new Edge(edgeIndex++, c2, c3, nullptr, nullptr);
			e23->m_v0 = c;
			m_edges.push_back(e23);
			c2->m_edges.push_back(e23);
			c3->m_edges.push_back(e23);
		}
		else
		{
			e23->m_v1 = c;
		}
		c->m_edges.push_back(e23);

		Edge* e31 = c3->GetEdgeWith(c1);
		if (e31 == nullptr)
		{
			e31 = new Edge(edgeIndex++, c3, c1, nullptr, nullptr);
			e31->m_v0 = c;
			m_edges.push_back(e31);
			c3->m_edges.push_back(e31);
			c1->m_edges.push_back(e31);
		}
		else
		{
			e31->m_v1 = c;
		}
		c->m_edges.push_back(e31);
	}
}

void Map::FinishInfo()
{
	for (auto center : m_centers)
	{
		center->SortCorners();
	}

	for (auto c : m_centers)
	{
		for (auto e : c->m_edges)
		{
			Center* auxCenter = e->GetOppositeCenter(c);
			if (auxCenter != nullptr)
			{
				c->m_centers.push_back(auxCenter);
			}
		}
	}

	for (auto c : m_corners)
	{
		for (auto e : c->m_edges)
		{
			Corner* auxCorner = e->GetOppositeCorner(c);
			if (auxCorner != nullptr)
			{
				c->m_corners.push_back(auxCorner);
			}
		}
	}
}

void Map::AddCenter(Center* c)
{
	std::map<double, std::map<double, Center*>>::const_iterator iter = m_posCenterMap.find(c->m_position.x);

	if (iter != m_posCenterMap.end())
	{
		m_posCenterMap[static_cast<int>(c->m_position.x)][static_cast<int>(c->m_position.y)] = c;
	}
	else
	{
		m_posCenterMap[static_cast<int>(c->m_position.x)] = std::map<double, Center*>();
		m_posCenterMap[static_cast<int>(c->m_position.x)][static_cast<int>(c->m_position.y)] = c;
	}
}

Center* Map::GetCenter(Vector2 position)
{
	std::map<double, std::map<double, Center*>>::const_iterator iter = m_posCenterMap.find(position.x);

	if (iter != m_posCenterMap.end())
	{
		std::map<double, Center*>::const_iterator iter2 = iter->second.find(position.y);

		if (iter2 != iter->second.end())
		{
			return iter2->second;
		}
	}

	return nullptr;
}

std::vector<Corner*> Map::GetLandCorners()
{
	std::vector<Corner*> landCorners;

	for (auto c : m_corners)
	{
		if (!c->m_water)
		{
			landCorners.push_back(c);
		}
	}

	return landCorners;
}

std::vector<Corner*> Map::GetLakeCorners()
{
	std::vector<Corner*> lakeCorners;

	for (auto c : m_corners)
	{
		if (!c->m_water && !c->m_ocean)
		{
			lakeCorners.push_back(c);
		}
	}

	return lakeCorners;
}

void Map::LloydRelaxation()
{
	std::vector<DelaunayTriangulation::Vertex> newPoints;

	for (auto p : m_centers)
	{
		if (!p->IsInsideBoundingBox(m_mapWidth, m_mapHeight))
		{
			newPoints.push_back(DelaunayTriangulation::Vertex(static_cast<int>(p->m_position.x), static_cast<int>(p->m_position.y)));
			continue;
		}

		Vector2 centerCentroid;

		for (auto q : p->m_corners)
		{
			if (q->IsInsideBoundingBox(m_mapWidth, m_mapHeight))
			{
				centerCentroid += q->m_position;
			}
			else
			{
				Vector2 cornerPos = q->m_position;

				if (cornerPos.x < 0)
				{
					cornerPos.x = 0;
				}
				else if (cornerPos.x >= m_mapWidth)
				{
					cornerPos.x = m_mapWidth;
				}

				if (cornerPos.y < 0)
				{
					cornerPos.y = 0;
				}
				else if (cornerPos.y >= m_mapHeight)
				{
					cornerPos.y = m_mapHeight;
				}

				centerCentroid += cornerPos;
			}
		}

		centerCentroid /= p->m_corners.size();
		newPoints.push_back(DelaunayTriangulation::Vertex(static_cast<int>(centerCentroid.x), static_cast<int>(centerCentroid.y)));
	}

	Triangulate(newPoints);
}

std::string Map::CreateSeed(int length)
{
	std::mt19937 mt_rand;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string seed;

	for (int i = 0; i < length; ++i)
	{
		seed.push_back(alphanum[mt_rand() % (sizeof(alphanum) - 1)]);
	}

	return seed;
}

static unsigned int HashString(std::string seed);