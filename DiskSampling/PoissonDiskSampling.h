#ifndef POISSON_DISK_SAMPLING_H
#define POISSON_DISK_SAMPLING_H

#include <vector>

class PoissonDiskSampling
{
public:
	PoissonDiskSampling() = default;

	~PoissonDiskSampling() = default;

	PoissonDiskSampling(const PoissonDiskSampling& rhs) = default;
	PoissonDiskSampling(const PoissonDiskSampling&& rhs) = default;

	PoissonDiskSampling& operator=(const PoissonDiskSampling& rhs) = default;
	PoissonDiskSampling& operator=(const PoissonDiskSampling&& rhs) = default;

public:
	PoissonDiskSampling(int pointWidth, int pointHeight, double pointMinDist, double pointCount);

	std::vector<std::pair<double, double>> Generate();
};

#endif