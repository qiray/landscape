#include <algorithm>
#include <cstring>
#include "mapfield.h"

mapField::mapField(int Size, int *array) {
   if (Size == 0) 
      Size = 1;
   size = static_cast<unsigned short>(Size);
   sizeLog = static_cast<unsigned short>(log2int(size));
   sizeMod = size - 1;
   unsigned int limit = size*size;
   mapArray = new int [limit];
   info = new char [limit];
   allNodes = new node [limit];
   for (unsigned int i = 0; i < limit; i++) {
      mapArray[i] = array[i];
      info[i] = 0;
      node n(i%size, static_cast<unsigned short>(i/size), nullptr);
      allNodes[i] = n;
   }
   binary_heap openList;
}

mapField::~mapField() {
    delete [] mapArray;
    delete [] info;
    delete [] allNodes;
}

std::ostream& operator<< (std::ostream& s, const mapField& m) {
   int* p = m.mapArray;
   for(int i = 0; i < m.size; i++) {
      for(int j = 0; j < m.size; j++)
         s << p[j + i*m.size]<< " ";
      s << std::endl;
   }
   return s;
}

bool mapField::saveMap(char* fileName) { //save map to file fileName, return true if successful
   std::fstream f(fileName, std::fstream::out);
   if (f.fail())
      return false;
   f << size << std::endl;
   f << *this << std::endl;
   f.close();
   return true;
}

bool mapField::loadMap(char* fileName) { //load map from file fileName, return true if successful
   std::fstream f(fileName, std::fstream::in);
   if (f.fail())
      return false;
   f >> size;
   sizeLog = static_cast<unsigned short>(log2int(size));
   sizeMod = size - 1;
   delete [] mapArray;
   delete [] info;
   delete [] allNodes;
   unsigned int limit = size*size;   
   mapArray = new int [limit];
   info = new char [limit];
   allNodes = new node [limit];
   for (unsigned int i = 0; i < limit; i++) {
      f >> mapArray[i];
      info[i] = 0;
      node n(i%size, static_cast<unsigned short>(i/size), nullptr);
      allNodes[i] = n;      
   }
   f.close();
   return true;
}

void mapField::addNode(int x, int y, int index, const node &current, const node &stop, float roughness) {
   if (x >= size || y >= size || x < 0 || y < 0)
      return;//don't handle nodes beyond the edge of map
   int i = x + y*size;
   if (mapArray[i] == blockedCell || info[i] == 2) //if cell is blocked or in close list
      return;
   node *p = &allNodes[i];
   int diff = (mapArray[i] - mapArray[index])*node::getWeight(); //height difference influence
   if (diff > 0)
      diff *= 5;
   if (info[i] != 1) {//if not in open list
      p->clearG();
      p->clearH();
      p->H(stop);
      p->setParent(const_cast<node*>(&current));
      if (roughness > 0.001f)
         diff += p->parentStraightLength(roughness);
      p->G(diff);
      p->setF();
      info[i] = 1;
      openList.add(&allNodes[i]);
   } else if (current.getG() + node::getWeight() + diff < p->G(diff)) {
         p->setParent(const_cast<node*>(&current));
         p->G(diff);
         p->setF();
   }
}

void mapField::addAvailable(const node &current, const node &stop, float roughness) {//add empty nodes which border with node current to openList
   int x0 = current.getX(), y0 = current.getY(), index = y0*size + x0;
   int x1 = x0 - 1, y1 = y0 - 1, x2 = x0 + 1, y2 = y0 + 1;
   addNode(x0, y1, index, current, stop, roughness);
   addNode(x0, y2, index, current, stop, roughness);
   addNode(x1, y0, index, current, stop, roughness);
   addNode(x2, y0, index, current, stop, roughness);
}

inline bool mapField::maxRadius(unsigned short stopX, unsigned short stopY, unsigned short step, int &ret) {//return true if there is at least one node which distance to stop node is <= then step
   if (step == 0)
      return false;
   for(int i = stopX - step/2; i <= stopX + step/2; i++)
      for(int j = stopY - step/2; j <= stopY + step/2; j++)
         if(0 <= i && i < size && 0 <= j && j < size && info[i + j*size] == 1) {
            ret = i + j*size;
            return true;
         }
   return false;
}

bool mapField::Astar(const node &startNode, const node &stopNode, std::vector<int> &way, float roughness, unsigned short step = 0) {//return true if path is found
   //step is max distance between stopNode and node where search stops, if zero then stop only if stop node is reached
   node *start, *stop;
   int startNum = startNode.getX() + startNode.getY()*size;
   int stopNum = stopNode.getX() + stopNode.getY()*size;
   size_t limit = size*size;
   unsigned short stopX = stopNode.getX(), stopY = stopNode.getY();
   memset(info, '\0', limit*sizeof(char));
   start = &allNodes[startNum];
   stop = &allNodes[stopNum];
   start->initF(start->H(stopNode));
   openList.clear();
   openList.add(start);
   addAvailable(*start, *stop, roughness);
   info[startNum] = 2;
   while(!openList.isEmpty()) {
      node *current(openList.getMin());
      info[current->getX() + current->getY()*size] = 2;
      addAvailable(*current, *stop, roughness);
      if (info[stopNum] == 1 || maxRadius(stopX, stopY, step, stopNum)) {//if stop node is in the open list
         node *temp(&allNodes[stopNum]);
         way.push_back(stopNum);
         while (temp != start) {//while start node isn't reached
            temp = temp->getParent();//go back to start through nodes' parents
            way.push_back(temp->getX() + temp->getY()*size);
         }
         reverse(way.begin(), way.end());
         return true;//path found
      }
   }
   return false;//can't find path
}
