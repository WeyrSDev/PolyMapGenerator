#include "PoissonDiskSampling.h"

PoissonDiskSampling::PoissonDiskSampling(int pointWidth, int pointHeight, double pointMinDist, double pointCount) :
	m_width(pointWidth),
	m_height(pointHeight),
	m_minDist(pointMinDist),
	m_pointCount(pointCount),
	m_cellSize(m_minDist / 1.414214f),
	m_gridWidth(ceil(m_width / m_cellSize)),
	m_gridHeight(ceil(m_height / m_cellSize)),
	m_grid(std::vector<std::vector<std::shared_ptr<Point>>>(m_gridWidth, std::vector<std::shared_ptr<Point>>(m_gridHeight, nullptr)))
{
	
}

