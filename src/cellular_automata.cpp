#include <cstring>
#include <cstdlib>
#include "cellular_automata.h"

inline float oneStepCell(float *heights, int mapSize, int x, int y) {
	float sum = 0;
	int counter = 0;
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++) {
			if (j == y && i == x || j < 0 || i < 0 || j >= mapSize || i >= mapSize)
				continue;
			counter++;
			sum += heights[i + j*mapSize];
		}
	return sum/counter;
}

void oneStep(float *heights, int mapSize) {
	int length = mapSize*mapSize;
	float *newHeights = new float[length];
	for (int i = 0; i < mapSize; i++)
		for (int j = 0; j < mapSize; j++)
			newHeights[i + j*mapSize] = oneStepCell(heights, mapSize, i, j);
	memcpy(heights, newHeights, length*sizeof(float));
	delete [] newHeights;
}

void CellularAutomaton(float *heights, int mapSize, int gens) {
	int length = mapSize*mapSize;
	for (int i = 0; i < length; i++)
		heights[i] = minMaxRandom(-127, 127);
	for (int i = 0; i < gens; i++)
		oneStep(heights, mapSize);
}
