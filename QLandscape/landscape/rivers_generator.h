#include <vector>
#include "landscape.h"
#include "mapfield.h"

#ifndef RIVERS_GENERATOR_H

#define RIVERS_GENERATOR_H

#include "landscape.h"

int getVectorIndex (const std::vector<int>&array, int value);

class RiverGenerator {
    std::vector<std::vector<int>> rivers;

    bool isWater(landscapeCell *landscape, int mapSize, int x, int y,  int &finish);
    int distanceToWater(landscapeCell *landscape, int mapSize, int start, int &finish, int step);
    int getNeighbourWaterCount(landscapeCell *landscape, int fullSize, int mapSize, int index);
    int calcWidth(int maxwidth, int length);
    int findRiverLength(const std::vector<std::vector<int>> &rivers, int index, int notThisRiver);
    float roughnessByLength(int mapSize, int start, int finish);
    void makeRivers(int number, int length, landscapeCell *landscape, int mapSize);
    void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish);
    void landscapeInfluence(int width, landscapeCell *landscape, int mapSize);
public:
    RiverGenerator();
    void generateRivers(int number, int length, int width, landscapeCell *landscape, int mapSize);
};

#endif
