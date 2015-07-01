#include <cstdlib>
#include <fstream>
#include <vector>
#include "binary_heap.h"

#ifndef ASTAR_MAP_H

#define ASTAR_MAP_H

const int emptyCell = 0, blockedCell = -1, startCell = 2, stopCell = 3;

using namespace std;

class mapField {
   unsigned short size, sizeMod, sizeLog;///size must be 2^n
   int* mapArray;
   char *info;//info about list (info = 1 if this node is in open list and = 2 if in close list)
   binary_heap openList;
   node *allNodes;
   
public:
   unsigned short getSize() {return size;};
   mapField(unsigned short);
   mapField(int, int*);
   ~mapField();
   friend ostream& operator<<(ostream&, const mapField&);
   bool saveMap(char*);
   bool loadMap(char*);
   friend void generateField(unsigned short, int, mapField&);
   bool isEmpty(unsigned short, unsigned short); 
   bool makeStart(unsigned short, unsigned short, node&, int&);
   bool makeStop(unsigned short, unsigned short, node&, int&, int&);
   int returnInfo(unsigned short, unsigned short);
   void setInfo(unsigned short, unsigned short, int);
   void generateStartStop(node&, node&, int&, int&, int&);
   node loadStart();
   node loadStop();
   void addNode(unsigned short, unsigned short, int, const node&, const node&);
   void addAvailable(const node&, const node&);
   bool Astar(const node&, const node&, vector<int>&, unsigned short);
   void makeRegions();
   void floodFill8Stack(unsigned short, unsigned short, int, int);
   bool maxRadius(unsigned short, unsigned short, unsigned short, int&);
};

struct stack {
   int *stack;
   int stackPointer, stackSize, h;
   bool pop(unsigned short&, unsigned short&);
   bool push(unsigned short, unsigned short); 
   bool pop(int&);
   bool push(int);
   void emptyStack();
};

#endif
