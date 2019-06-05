#include <cstring>
#include <cstdlib>
#include <cmath>
#include "cellular_automata.h"

//TODO: make it work
//read - http://ideabyre.com/world-building/erosion/erosion_model

inline int oneStepCell(int *field, int mapSize, int x, int y) {
    int sum = 0;
    int cur = field[x + y*mapSize];
    for (int i = x - 1; i <= x + 1; i++)
        for (int j = y - 1; j <= y + 1; j++) {
            if ((j == y && i == x) || j < 0 || i < 0 || j >= mapSize || i >= mapSize)
                continue;
            sum += field[i + j*mapSize];
        }
    if (sum == 3 && cur == 0)
        return 1;
    if ((sum == 2 || sum == 3) && cur == 1)
        return 1;
    return 0;
}

void oneStep(int *field, float *heights, int mapSize) {
    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++) {
            int tmp = oneStepCell(field, mapSize, i, j);
            heights[i + j*mapSize] += field[i + j*mapSize] = tmp;
        }
}

void CellularAutomaton(float *heights, int mapSize, int gens) {
    int length = mapSize*mapSize;
    int *field = new int [length];
    for (int i = 0; i < length; i++)
        field[i] = static_cast<int>(roundf(minMaxRandom(0, 1)));
    memset(heights, 0, static_cast<size_t>(length)*sizeof(*heights));
    for (int i = 0; i < gens; i++)
        oneStep(field, heights, mapSize);
    delete [] field;
}
