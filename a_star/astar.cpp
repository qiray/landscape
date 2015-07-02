#include "mapfield.h"

extern stack coordStack;

void mapField::floodFill8Stack(unsigned short x, unsigned short y, int newColor, int oldColor) {
   if(newColor == oldColor)
      return;
   coordStack.emptyStack();      
   if(!coordStack.push(x, y))
      return; 
   while(coordStack.pop(x, y)) {
      int i = x + y*size; 
      allNodes[i].region = newColor;
      if(x + 1 < size && mapArray[i + 1] != blockedCell && allNodes[i + 1].region == oldColor) {       
         if(!coordStack.push(i + 1))
            return;
      }    
      if(x - 1 >= 0 && mapArray[i - 1] != blockedCell && allNodes[i - 1].region == oldColor) {
         if(!coordStack.push(i - 1))
            return;
      }    
      if(y + 1 < size && mapArray[i + size] != blockedCell && allNodes[i + size].region == oldColor) {
         if(!coordStack.push(i + size))
            return;
      }
      if(y - 1 >= 0 && mapArray[i - size] != blockedCell && allNodes[i - size].region == oldColor) {
         if(!coordStack.push(i - size))
            return;
      }
   }
}


void mapField::addNode(unsigned short x, unsigned short y, int index, const node &current, const node &stop, float roughness) {
   if (x >= size || y >= size)
      return;//don't handle nodes beyond the edge of map
   int i = x + y*size;
   node *p = &allNodes[i]; 
   if (mapArray[i] == blockedCell || info[i] == 2) //if cell is blocked or in close list
      return;
   int diff = (mapArray[i] - mapArray[index])*node::weight;
   if (info[i] != 1) {//if not in open list
      p->H(stop);
      p->parentNode = const_cast<node*>(&current);
      if (roughness > 0.001)
         diff += p->parentStraightLength(roughness); //(hl + hr) / 2 + rand(-r * len, +r * len);
      p->G(diff);
      p->f = p->g + p->h;;
      info[i] = 1;
      openList.add(&allNodes[i]);
      //mapArray[i] = 7;///only for demonstartion
   } else if (current.getG() + node::weight + diff < p->G(diff)) {
         p->parentNode = const_cast<node*>(&current);
         p->G(diff);
         p->f = p->g + p->h;
   }
}

void mapField::addAvailable(const node &current, const node &stop, float roughness) {//add empty nodes which border with node current to openList
   int x0 = current.x, y0 = current.y, index = y0*size + x0;
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

bool mapField::Astar(const node &startNode, const node &stopNode, vector<int> &way, float roughness, unsigned short step = 0) {//return 1 if path is found
   //step is max distance between stopNode and node where search stops, if zero then stop only if stop node is reached
printf("start: x = %d y = %d stop: x = %d y = %d\n", startNode.x, startNode.y, stopNode.x, stopNode.y);
   node *start, *stop;
   int startNum = startNode.x + startNode.y*size;
   int stopNum = stopNode.x + stopNode.y*size;
   int limit = size*size;
   for (int i = 0; i < limit; i++)
	allNodes[i].g = allNodes[i].h = 0;
   unsigned short stopX = stopNode.x, stopY = stopNode.y;
   memset(info, '\0', limit*sizeof(char));
   start = &allNodes[startNum];
   stop = &allNodes[stopNum];
   start->f = start->H(stopNode);
   openList.clear();   
   openList.add(start);
   openList.buildHeap();
   addAvailable(*start, *stop, roughness);
   info[startNum] = 2;
   while(!openList.empty()) {
      node *current(openList.getMax());
      info[current->x + current->y*size] = 2;
      addAvailable(*current, *stop, roughness);
      way.push_back(stopNum);  
      if (info[stopNum] == 1 || maxRadius(stopX, stopY, step, stopNum)) {//if stop node is in the open list
         node *temp(&allNodes[stopNum]);
         while (temp != start) {//while start node isn't reached
            temp = temp->parentNode;//go back to start through nodes' parents
            //mapArray[temp->x + temp->y*size] = 5; ///only for demonstration 
            way.push_back(temp->x + temp->y*size);                   
         }
         return true;///path found
      }
   }
   return false;///can't find path
}

void mapField::makeRegions () {
   unsigned short x = 0, y = 0;
   int i = 0, regionCode = 6, num = 0; 
   coordStack.stackPointer = 0;
   coordStack.h = size;
   coordStack.stackSize = 8*size*size;//max size to avoid stack overflow
   coordStack.stack = new int [coordStack.stackSize];
   for(x = 0; x < size; x++) {
      for(y = 0; y < size; y++) {      
         if (allNodes[i].region == 0 && mapArray[i] != blockedCell) {
            floodFill8Stack(x, y, regionCode, 0);
            regionCode++;    
         }
         i++;
      }
   }
   delete [] coordStack.stack;
}

/*void func() { 
	
}

int main(int argc, char** argv) {
   try {
      mapField m(1);
      //generateField(512, 1, m);
      //m.saveMap(argv[1]);
      //return 0;
      m.loadMap(argv[1]);     
      node start, stop;
      m.makeRegions();//prepare regions for A*
      start = m.loadStart();
      stop = m.loadStop();
      if(m.Astar(start, stop, 0))
         cout << "founded \n";
      else
         cout << "no\n";
      m.saveMap(argv[2]);
      return 0;
   }
   catch (string s) {
      cerr << s << endl;
      return 1;
   }
}*/
