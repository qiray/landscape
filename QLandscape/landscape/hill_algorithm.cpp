#include "hill_algorithm.h"

//hill algorithm http://www.ixbt.com/video/3dterrains-generation.shtml

void initHeights(float *heights, int mapSize) {
    int len = mapSize*mapSize;
    for (int i = 0; i < len; i++)
        heights[i] = 0;
}

inline void setHillHeigths(float *heights, int mapSize, float maxHeight, int x, int y, int xorigin, int yorigin) {
    if (x >= 0 && y >= 0 && x < mapSize && y < mapSize) {
        int xdiff = x - xorigin, ydiff = y - yorigin, dist = xdiff*xdiff + ydiff*ydiff;
        float result = maxHeight - dist;
        if (result > 0)
            heights[x + y*mapSize] += result;
    }
}

void makeHill(float *heights, int mapSize, int x, int y, int range) {
    float maxHeight = range*range;
    heights[x + y*mapSize] += maxHeight;
    int step = 1;
    while (step < range) {
        for (int i = x - step; i <= x + step; i++) {
            setHillHeigths(heights, mapSize, maxHeight, i, y + step, x, y);
            setHillHeigths(heights, mapSize, maxHeight, i, y - step, x, y);
        }
        for (int i = y - step + 1; i <= y + step - 1; i++) {
            setHillHeigths(heights, mapSize, maxHeight, x + step, i, x, y);
            setHillHeigths(heights, mapSize, maxHeight, x - step, i, x, y);
        }
        step++;
    }
}

void liftRegion(float *heights, int mapSize, int x, int y, int steps, int range, int radius) {
    for (int i = 0; i < steps; i++) {
        int coordX = 0, coordY = 0;
        do {
            coordX = static_cast<int>(minMaxRandom(x - range, x + range));
            coordY = static_cast<int>(minMaxRandom(y - range, y + range));
        } while (coordX < 0 || coordX > mapSize || coordY < 0 || coordY > mapSize);
        makeHill(heights, mapSize, coordX, coordY, rand()%radius);
    }
}

void Hill_algorithm(float *heights, int mapSize, int islands, float roughness, int steps, int maxRange, int maxHeight) {
    if (islands <= 1) {
        for (int i = 0; i < steps; i++)
            makeHill(heights, mapSize, rand()%mapSize, rand()%mapSize, rand()%maxRange);
    } else {
        int x = static_cast<int>(1.0/4*mapSize), y = static_cast<int>(1.0/4*mapSize);
        float cosPhi = static_cast<float>(cos(2*M_PI/islands)), sinPhi = static_cast<float>(sin(2*M_PI/islands));
        for (int i = 0; i < islands; i++) {
            liftRegion(heights, mapSize, x + mapSize/2, y + mapSize/2, mapSize/islands, mapSize/(islands*2), 100);
            int prevX = x;
            x = static_cast<int>(x*cosPhi - y*sinPhi); //calc position for a new island: "rotate" it
            y = static_cast<int>(prevX*sinPhi + y*cosPhi);
        }
    }
    int len = mapSize*mapSize;
    float max = 0, min = 0;
    for (int i = 0; i < len; i++) {
        if (heights[i] > max)
            max = heights[i];
        if (heights[i] < max)
            min = heights[i];
    }
    for (int i = 0; i < len; i++)
        heights[i] = roughness*(powf((heights[i] - min)/(max - min), 1)*2*maxHeight - maxHeight);
}
