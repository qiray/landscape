#include <string>

using namespace std;

#ifndef LANDSCAPE_H

#define LANDSCAPE_H

//sizes: //TODO: use these sizes
#define TINY 1
#define SMALL 2
#define NORMAL 3
#define BIG 4
#define LARGE 5

//algorithms:
namespace landscapeAlgorithm {
	const string diamond_square = "diamond_square";
	const string hill_algorithm = "hill_algorithm";
	const string perlin_noise = "perlin_noise";
	extern string currentAlgorithm;
	void checkAlgorithm();
}

typedef char landscapeCell;
#define MAX_LANDSCAPE_CELL 127

namespace terrain {
	const landscapeCell deepWater = -32;
	const landscapeCell water = 0;
	const landscapeCell coast = 1;
	const landscapeCell hill = 32;
	const landscapeCell highland = 64;
	const landscapeCell everest = 96;
}

#endif
