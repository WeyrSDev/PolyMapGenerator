#ifndef POISSON_DISK_SAMPLING_H
#define POISSON_DISK_SAMPLING_H

#include <vector>
#include <memory>

class PoissonDiskSampling
{
public:
	PoissonDiskSampling() = default;
	PoissonDiskSampling(int pointWidth, int pointHeight, double pointMinDist, double pointCount);

	~PoissonDiskSampling() = default;

	PoissonDiskSampling(const PoissonDiskSampling& pds) = delete;
	PoissonDiskSampling(PoissonDiskSampling&& pds) = delete;

	PoissonDiskSampling& operator=(const PoissonDiskSampling& rhs) = delete;
	PoissonDiskSampling& operator=(PoissonDiskSampling&& rhs) = delete;

	std::vector<std::pair<double, double>> Generate();

	struct Point
	{
		Point() : x(0.0), y(0.0) { }
		Point(double _x, double _y) : x(_x), y(_y) { }

		~Point() = default;

		Point(const Point& p) : x(p.x), y(p.y) { }
		Point(Point&& p) : x(p.x), y(p.y) { }

		Point& operator=(const Point& rhs)
		{
			if (this == &rhs)
			{
				return *this;
			}

			x = rhs.x;
			y = rhs.y;

			return *this;
		}
		Point& operator=(Point&& rhs)
		{
			if (this == &rhs)
			{
				return *this;
			}

			x = rhs.x;
			y = rhs.y;

			return *this;
		}

		int GetGridIndex(double cellSize, int mapWidth) const
		{
			int indexX = static_cast<int>(x / cellSize);
			int indexY = static_cast<int>(y / cellSize);

			return indexX + indexY * mapWidth;
		}

		double Distance(Point p) const
		{
			return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
		}

		double x, y;
	};

private:
	std::vector<std::vector<std::shared_ptr<Point>>> m_grid;
	std::vector<Point> m_process;
	std::vector<std::pair<double, double>> m_sample;

	int m_width;
	int m_height;
	double m_minDist;
	int m_pointCount;
	double m_cellSize;
	int m_gridWidth;
	int m_gridHeight;

	Point GeneratePointAround(Point p) const;
	bool IsInRectangle(Point p) const;
	bool IsInNeighbourhood(Point p);
	std::vector<std::shared_ptr<Point>> GetCellsAround(Point p);
};

#endif