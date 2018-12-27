#include <cstdlib>
#include <fstream>
#include <vector>
#include "binary_heap.h"

#ifndef ASTAR_MAP_H

#define ASTAR_MAP_H

const int blockedCell = -1;

using namespace std;

class mapField {
    unsigned short size, sizeMod, sizeLog;//size must be 2^n
    int* mapArray;
    char *info;//info about list (info = 1 if this node is in open list and = 2 if in close list)
    node *allNodes;
    binary_heap openList;
public:
    unsigned short getSize() {return size;}
    mapField(int, int*);
    ~mapField();
    friend ostream& operator<<(ostream&, const mapField&);
    bool saveMap(char*);
    bool loadMap(char*);
    void addNode(int, int, int, const node&, const node&, float);
    void addAvailable(const node&, const node&, float);
    bool Astar(const node&, const node&, vector<int>&, float, unsigned short);
    inline bool maxRadius(unsigned short, unsigned short, unsigned short, int&);
};

#endif
