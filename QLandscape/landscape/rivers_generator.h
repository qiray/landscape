#include "landscape.h"
#include "mapfield.h"

#ifndef RIVERS_GENERATOR_H

#define RIVERS_GENERATOR_H

extern float minMaxRandom(float min, float max);

void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish);
int distanceToWater(landscapeCell *landscape, int mapSize, int start, int &finish, int step);

#endif
