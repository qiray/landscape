#include <cmath>
#include <cstdlib>

#ifndef DIAMOND_SQUARE_H

#define DIAMOND_SQUARE_H

#include "landscape.h"

void diamondSquare(float *heights, int mapSize, float R, int leftX, int topY, int rightX, int bottomY, float outHeight);
void generateDiamondSquareHeights(float *heights, int mapSize, int initHeight, float R, int size, float outHeight, int leftX, int topY, int rightX, int bottomY);

#endif
