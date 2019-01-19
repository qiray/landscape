#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "rivers_generator.h"

static std::vector<std::vector<int>> rivers;

//TODO: rewrite it

inline bool isWater(landscapeCell *landscape, int mapSize, int x, int y,  int &finish) {
    if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
        return false;
    if((landscape[y*mapSize + x] <= -1 || landscape[y*mapSize + x] == 0) && rand()%101 > 90) {
        for (int i = x - 1; i <= x + 1; i += 2)
            if (i >= 0 && i < mapSize && landscape[y*mapSize + i] > 0) {
                finish = y*mapSize + i;
                return true;
            }
        for(int i = y - 1; i <= y + 1; i += 2)
            if (i >= 0 && i < mapSize && landscape[i*mapSize + x] > 0) {
                finish = i*mapSize + x;
                return true;
            }
    }
    return false;
}

inline int distanceToWater(landscapeCell *landscape, int mapSize, int start, int &finish, int step) {
    int dist = 1, x = start%mapSize, y = start/mapSize;
    while(1) {
        for (int i = x - dist; i <= x + dist; i++)
            if (isWater(landscape, mapSize, i, y - dist, finish) || isWater(landscape, mapSize, i, y + dist, finish))
                return dist;
        for (int i = y - dist + 1; i <= y + dist - 1; i++)
            if (isWater(landscape, mapSize, x - dist, i, finish) || isWater(landscape, mapSize, x + dist, i, finish))
                return dist;
        if (x - dist < 0 && x + x + dist > mapSize && y - dist < 0 && y + dist > mapSize)
            return -1;
        dist += step;
    }
}

inline float factorByLength(int mapSize, int start, int finish) {
    int len = abs(start%mapSize - finish%mapSize) + abs(start/mapSize - finish/mapSize);
    if (len <= 64)
        return 0.5f;
    if (len <= 128)
        return 0.05f;
    return 0.02f;
}

void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish) {
    std::vector<int> river;
    node startNode(static_cast<unsigned short>(start%mapSize), static_cast<unsigned short>(start/mapSize), nullptr);
    node finishNode(static_cast<unsigned short>(finish%mapSize), static_cast<unsigned short>(finish/mapSize), nullptr);
    if (!m.Astar(startNode, finishNode, river, factorByLength(mapSize, start, finish), 0))
        return;
    size_t length = river.size() - 1;
    for (size_t i = 0; i <= length; i++)
        landscape[river[i]] = 0;
    rivers.push_back(river);
}

int neighbourWater(landscapeCell *landscape, int fullSize, int mapSize, int index) {
    int sum = 0;
    if (index + 1 < fullSize && landscape[index + 1] == 0)
        sum++;
    if (index - 1 >= 0 && landscape[index - 1] == 0)
        sum++;
    if (index + mapSize < fullSize && landscape[index + mapSize] == 0)
        sum++;
    if (index - mapSize >= 0 && landscape[index - mapSize] == 0)
        sum++;
    return sum;
}

int calcWidth(int maxwidth, int length) {
    int newWidth = length/50;
    newWidth = newWidth > maxwidth ? maxwidth : newWidth;
    return newWidth = newWidth <= 1 ? 1 : newWidth;
}

inline int getVectorIndex (const std::vector<int>&array, int value) {
    size_t len = array.size();
    for (size_t i = 0; i < len; i++)
        if (array[i] == value)
            return i;
    return -1;
}

inline int findRiverLength(const std::vector<std::vector<int>> &rivers, int index, int notThisRiver) {
    for (size_t i = 0; i < rivers.size(); i++)
        if (i != notThisRiver && getVectorIndex (rivers[i], index) != -1)
            return rivers[i].size();
    return rivers[notThisRiver].size();
}

void generateRivers(landscapeAlgorithm &alg, int number, int length, int width) {
    std::vector<int> highlands;
    landscapeCell * landscape = alg.landscape;
    int mapSize = alg.mapSize;
    int len = mapSize*mapSize;
    int *tempMap = new int [len];
    double average = 0.0, factor = 0.3;
    for (int i = 0; i < len; i++) {
        tempMap[i] = landscape[i] > 0 ? landscape[i] : blockedCell;
        if (landscape[i] > 0)
            highlands.push_back(i);
    }
    int highlandsSize = highlands.size();
    if (highlandsSize == 0) {
        delete [] tempMap;
        return;
    }
    for (int i = 0; i < highlandsSize; i++)
        average += (double)landscape[highlands[i]]/highlandsSize;
    average *= factor;
    mapField m(mapSize, tempMap);
    while (number > 0) {
        int start = -1, finish = -1, dist, count = 0;
        do {
            start = highlands[rand()%highlandsSize];
            dist = distanceToWater(landscape, mapSize, start, finish, 1);
            if (count++ > 50)
                break;
        } while (landscape[start] < average || dist < length || landscape[finish] > landscape[start]);
        generateRiverAstar(landscape, mapSize, m, start, finish);
        number--;
    }
    std::vector<int> intersections, widths;
    for (int i = 0; i < rivers.size(); i++)
        for (int j = 0; j < rivers[i].size(); j++) {
            int index = rivers[i][j];
            if (neighbourWater(landscape, len, mapSize, index) > 2) {
                intersections.push_back(index);
                widths.push_back(calcWidth(width, findRiverLength(rivers, index, i)));
            }
        }
    for (int i = 0; i < rivers.size(); i++) {
        length = rivers[i].size();
        int size = 1, halfsize = 0, totalLength = mapSize*mapSize;
        int newWidth = calcWidth(width, length);
        for (int j = 0; j < length; j++) { //TODO: Should river change landscape?
            int index = rivers[i][j], nextIndex = rivers[i][j + 1];
            if (j == length - 1)
                nextIndex = rivers[i][j - 1];
            if (size < newWidth) {
                if(j == size*length/newWidth)
                    halfsize = ++size/2;
                int intersectionIndex = getVectorIndex (intersections, index);
                if (intersectionIndex != -1) {
                    size += widths[intersectionIndex];
                    halfsize = size/2;
                    if (size > newWidth) {
                        size = newWidth;
                        halfsize = size/2;
                    }
                }
            }
            int direction = abs(index - nextIndex) == 1 ? mapSize : 1;
            for (int k = index - direction*halfsize; k <= index + (size&1 ? direction*halfsize : direction*(halfsize - 1)); k += direction)
                if (k > 0 && k < totalLength)
                    landscape[k] = 0;
        }
    }
    delete [] tempMap;
}
