#include "diamond_square.h"

inline float squareStep(float *heights, int mapSize, float R, int leftX, int topY, int rightX, int bottomY) {
    float l = sqrtf((leftX - rightX)*(leftX - rightX) + (topY - bottomY)*(topY - bottomY));
    float result = (heights[leftX + mapSize*topY] + heights[rightX + mapSize*topY] + heights[leftX + mapSize*bottomY] + heights[rightX + mapSize*bottomY])/4 + minMaxRandom(-R*l, R*l);
    heights[(rightX + leftX)/2 + mapSize*(bottomY + topY)/2] = result;
    return result;
}

inline float getHeight(float *heights, int mapSize, int x, int y, float outHeight) {
    if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
        return outHeight;
    else	
        return heights[x + y*mapSize];
}

inline void diamondStep(float *heights, int mapSize, float R, int x, int y, int l, float outHeight) {
    int topX = x, topY = y - l/2;
    int rightX = x + l/2, rightY = y;
    int bottomX = x, bottomY = y + l/2;
    int leftX = x - l/2, leftY = y;
    float top = getHeight(heights, mapSize, topX, topY, outHeight);
    float left = getHeight(heights, mapSize, leftX, leftY, outHeight);
    float right = getHeight(heights, mapSize, rightX, rightY, outHeight);
    float bottom = getHeight(heights, mapSize, bottomX, bottomY, outHeight);
    float result = (top + right + bottom + left)/4 + minMaxRandom(-R*l, R*l);
    heights[x + y*mapSize] = result;
}

void diamondSquare(float *heights, int mapSize, float R, int leftX, int topY, int rightX, int bottomY, float outHeight) {
    int l = rightX - leftX;
    if (l < 2)
        return;
    outHeight = squareStep(heights, mapSize, R, leftX, topY, rightX, bottomY);
    diamondStep(heights, mapSize, R, leftX, (bottomY + topY)/2, l, outHeight); //left
    diamondStep(heights, mapSize, R, (leftX + rightX)/2, topY, l, outHeight); //top
    diamondStep(heights, mapSize, R, rightX, (bottomY + topY)/2, l, outHeight); //right
    diamondStep(heights, mapSize, R, (leftX + rightX)/2, bottomY, l, outHeight); //bottom
}

void generateDiamondSquareHeights(float *heights, int mapSize, int initHeight, float R, int size, float outHeight, int leftX, int topY, int rightX, int bottomY) {
    int len = mapSize*mapSize;
    for (int i = 0; i < len; i++)
        heights[i] = 0;
    if (initHeight == 0)
        initHeight = 1;
    heights[leftX + mapSize*topY] = rand()%initHeight; //init random heights
    heights[rightX + mapSize*topY] = rand()%initHeight;
    heights[leftX + mapSize*bottomY] = rand()%initHeight;
    heights[rightX + mapSize*bottomY] = rand()%initHeight;
    int step = size;//rightX - leftX;
    while (step >= 1) {
        for (int i = leftX; i < rightX; i += step)
            for (int j = topY; j < bottomY; j += step)
                diamondSquare(heights, mapSize, R, i, j, i + step, j + step, outHeight);
        step /= 2;
    }
}
