#include <cstdlib>
#include <cmath>

#ifndef HILL_ALGORITHM_H

#define HILL_ALGORITHM_H

#include "landscape.h"

void makeHill(float *heights, int mapSize, int x, int y, int range);
void Hill_algorithm(float *heights, int mapSize, int islands, float roughness, int steps, int maxRange, int maxHeight);

#endif
