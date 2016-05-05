#include "PoissonDiskSampling.h"

#include <random>
#include <algorithm>

PoissonDiskSampling::PoissonDiskSampling(int pointWidth, int pointHeight, double pointMinDist, double pointCount) :
	m_width(pointWidth),
	m_height(pointHeight),
	m_minDist(pointMinDist),
	m_pointCount(static_cast<int>(pointCount)),
	m_cellSize(m_minDist / 1.414214),
	m_gridWidth(static_cast<int>(ceil(m_width / m_cellSize))),
	m_gridHeight(static_cast<int>(ceil(m_height / m_cellSize)))
{
	m_grid = std::vector<std::vector<Point*>>(m_gridWidth, std::vector<Point*>(m_gridHeight, nullptr));
}

std::vector<std::pair<double, double>> PoissonDiskSampling::Generate()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	Point firstPoint(gen() % m_width, gen() % m_height);

	m_process.push_back(firstPoint);
	m_sample.push_back(std::make_pair(firstPoint.x, firstPoint.y));
	int firstPointX = static_cast<int>(firstPoint.x / m_cellSize);
	int firstPointY = static_cast<int>(firstPoint.y / m_cellSize);
	m_grid[firstPointX][firstPointY] = new Point(firstPoint);

	while (!m_process.empty())
	{
		int newPointIndex = gen() % m_process.size();
		Point newPoint = m_process[newPointIndex];
		m_process.erase(m_process.begin() + newPointIndex);

		for (int i = 0; i < m_pointCount; ++i)
		{
			Point newPointAround = GeneratePointAround(newPoint);

			if (IsInRectangle(newPointAround) && !IsInNeighbourhood(newPointAround))
			{
				m_process.push_back(newPointAround);
				m_sample.push_back(std::make_pair(newPointAround.x, newPointAround.y));
				int newPointX = static_cast<int>(newPointAround.x / m_cellSize);
				int newPointY = static_cast<int>(newPointAround.y / m_cellSize);
				m_grid[newPointX][newPointY] = new Point(newPointAround);
			}
		}
	}

	return m_sample;
}

PoissonDiskSampling::Point PoissonDiskSampling::GeneratePointAround(Point p) const
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

bool PoissonDiskSampling::IsInRectangle(Point p) const
{
	return (p.x >= 0 && p.y >= 0 && p.x < m_width && p.y < m_height);
}

bool PoissonDiskSampling::IsInNeighbourhood(Point p)
{
	std::vector<Point*> cells = GetCellsAround(p);
	int size = cells.size();
	for (int i = 0; i < size; ++i)
	{
		if (cells[i]->Distance(p) < m_minDist)
		{
			return true;
		}
	}

	return false;
}

std::vector<PoissonDiskSampling::Point*> PoissonDiskSampling::GetCellsAround(Point p)
{
	std::vector<Point*> cells;

	int indexX = static_cast<int>(p.x / m_cellSize);
	int indexY = static_cast<int>(p.y / m_cellSize);

	int minX = std::max(0, indexX - 1);
	int maxX = std::min(m_gridWidth - 1, indexX + 1);

	int minY = std::max(0, indexY - 1);
	int maxY = std::min(m_gridHeight - 1, indexY + 1);

	for (int i = minX; i <= maxX; ++i)
	{
		for (int j = minY; j <= maxY; ++j)
		{
			if (m_grid[i][j] != nullptr)
			{
				cells.push_back(m_grid[i][j]);
			}
		}
	}

	return cells;
}
