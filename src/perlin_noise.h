#include <cmath>
#include <cstdlib>

#ifndef PERLIN_NOISE_H

#define PERLIN_NOISE_H

extern float minMaxRandom(float min, float max);

void PerlinNoise(float *heights, int mapSize, float factor, int steps, float persistence, float frequency, float amplitude);

#endif