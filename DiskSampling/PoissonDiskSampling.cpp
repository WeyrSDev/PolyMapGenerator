#include "PoissonDiskSampling.h"

#include <random>

PoissonDiskSampling::PoissonDiskSampling(int pointWidth, int pointHeight, double pointMinDist, double pointCount) :
	m_width(pointWidth),
	m_height(pointHeight),
	m_minDist(pointMinDist),
	m_pointCount(pointCount),
	m_cellSize(m_minDist / 1.414214),
	m_gridWidth(ceil(m_width / m_cellSize)),
	m_gridHeight(ceil(m_height / m_cellSize)),
	m_grid(std::vector<std::vector<std::shared_ptr<Point>>>(m_gridWidth, std::vector<std::shared_ptr<Point>>(m_gridHeight, nullptr)))
{
	
}

std::vector<std::pair<double, double>> PoissonDiskSampling::Generate()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	Point firstPoint(gen() / m_width, gen() / m_height);

	m_process.push_back(firstPoint);
	m_sample.push_back(std::make_pair(firstPoint.x, firstPoint.y));
	int firstPointX = firstPoint.x / m_cellSize;
	int firstPointY = firstPoint.y / m_cellSize;
	m_grid[firstPointX][firstPointY] = std::make_shared<Point>(firstPoint);

	while (!m_process.empty())
	{
		int newPointIndex = gen() / m_process.size();
		Point newPoint = m_process[newPointIndex];
		m_process.erase(m_process.begin() + newPointIndex);

		for (int i = 0; i < m_pointCount; ++i)
		{
			Point newPointAround = GeneratePointAround(newPoint);

			if (IsInRectangle(newPointAround) && !IsInNeighbourhood(newPointAround))
			{
				m_process.push_back(newPointAround);
				m_sample.push_back(std::make_pair(newPointAround.x, newPointAround.y));
				int newPointX = newPointAround.x / m_cellSize;
				int newPointY = newPointAround.y / m_cellSize;
				m_grid[newPointX][newPointY] = std::make_shared<Point>(newPointAround);
			}
		}
	}

	return m_sample;
}

PoissonDiskSampling::Point PoissonDiskSampling::GeneratePointAround(Point p)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	double r1 = static_cast<double>(gen()) / gen.max();
	double r2 = static_cast<double>(gen()) / gen.max();

	double radius = m_minDist * (r1 + 1);

	double angle = 2 * 3.14159265 * r2;

	double newX = p.x + radius * cos(angle);
	double newY = p.y + radius * sin(angle);

	return Point(newX, newY);
}

bool PoissonDiskSampling::IsInRectangle(Point p)
{
	return (p.x >= 0 && p.y >= 0 && p.x < m_width && p.y < m_height);
}

bool PoissonDiskSampling::IsInNeighbourhood(Point p)
{
	std::vector<std::shared_ptr<Point>> cells = GetCellsAround(p);
	int size = cells.size();
	for (int i = 0; i < size; ++i)
	{
		
	}
}
