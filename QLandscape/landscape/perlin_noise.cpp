#include "perlin_noise.h"

//http://www.gamedev.ru/code/articles/?id=4212

inline float Noise2D(int x, int y) {
    int n = x + y * 57;
    n = (n<<13) ^ n;
    return 1.0f - ((n*(n*n*15731 + 789221) + 1376312589)&0x7fffffff)/1073741824.0f;
}

inline float Cosine_Interpolate(float a, float b, float x) {
    //return (1.0 - x)*a + x*b;
    float ft = static_cast<float>(M_PI)*x;
    float f = (1 - cosf(ft))*0.5f;
    return a*(1 - f) + b*f;
}

inline float Fast_Interpolate(float a, float b, float x) {
    float fac1 = 3*powf(1 - x, 2) - 2*powf(1 - x,3);
    float fac2 = 3*powf(x, 2) - 2*powf(x, 3);
    return a*fac1 + b*fac2;
}

float SmoothedNoise2D(int x, int y) {
    float corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) + Noise2D(x - 1, y + 1) + Noise2D(x + 1, y + 1))/16;
    float sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) + Noise2D(x, y - 1) + Noise2D(x, y + 1))/8;
    float center = Noise2D(x, y)/4;
    return corners + sides + center;
}

float CompileNoise(float x, float y) {
    int int_X = int(x);
    float fractional_X = x - int_X;
    int int_Y = int(y);
    float fractional_Y = y - int_Y;
    
    float v1 = SmoothedNoise2D(int_X, int_Y);
    float v2 = SmoothedNoise2D(int_X + 1, int_Y);
    float v3 = SmoothedNoise2D(int_X, int_Y + 1);
    float v4 = SmoothedNoise2D(int_X + 1, int_Y + 1);

    float i1 = Cosine_Interpolate(v1, v2, fractional_X);
    float i2 = Cosine_Interpolate(v3, v4, fractional_X);
    return Cosine_Interpolate(i1 , i2 , fractional_Y);
}

float PerlinNoise_2D(int x, int y, float factor, int steps, float persistence, float frequency, float amplitude) {
    float total = 0;
    x += factor;
    y += factor;
    for(int i = 0; i < steps; i++) {
        total += CompileNoise(x*frequency, y*frequency)*amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return total*512;
}

void PerlinNoise(float *heights, int mapSize, float factor, int steps, float persistence, float frequency, float amplitude) {
    int maxRand = 32768;
    int shiftX = rand()%maxRand, shiftY = rand()%maxRand;
    for(int i = 0; i < mapSize; i++)
        for(int j = 0; j < mapSize; j++)
            heights[i + j*mapSize] += PerlinNoise_2D(i + shiftX, j + shiftY, factor, steps, persistence, frequency, amplitude);
}
