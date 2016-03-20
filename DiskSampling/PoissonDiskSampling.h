#ifndef POISSON_DISK_SAMPLING_H
#define POISSON_DISK_SAMPLING_H

#include <vector>

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
				return;
			}

			x = rhs.x;
			y = rhs.y;

			return *this;
		}
		Point& operator=(Point&& rhs)
		{
			if (this == &rhs)
			{
				return;
			}

			x = rhs.x;
			y = rhs.y;

			return *this;
		}

		double x, y;
	};
};

#endif