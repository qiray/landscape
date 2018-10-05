#include "mapfield.h"

stack coordStack;//stack struct (described below)

mapField::mapField(unsigned short Size) {
   if (Size == 0) 
      Size = 1;
   size = Size;
   sizeMod = 1;
   sizeLog = 1;
   unsigned int limit = Size*Size;   
   mapArray = new int [limit];
   info = new char [limit];
   allNodes = new node [limit];
   for (int i = 0; i < limit; i++) {
      mapArray[i] = emptyCell;
      info[i] = 0;
      node n(i%size, i/size, 0);
      allNodes[i] = n;      
   }
   binary_heap openList;
   makeRegions();
}

mapField::mapField(int Size, int *array) {
   if (Size == 0) 
      Size = 1;
   size = Size;
   sizeLog = log2int(size);
   sizeMod = size - 1;
   unsigned int limit = Size*Size;   
   mapArray = new int [limit];
   info = new char [limit];
   allNodes = new node [limit];
   for (int i = 0; i < limit; i++) {
      mapArray[i] = array[i];
      info[i] = 0;
      node n(i%size, i/size, 0);
      allNodes[i] = n;
   }
   binary_heap openList;
   makeRegions();
}

mapField::~mapField() {
   delete [] mapArray;
   delete [] info;
   delete [] allNodes;
}

ostream& operator<< (ostream& s, const mapField& m) {
   int* p = m.mapArray;
   for(int i = 0; i < m.size; i++) {
      for(int j = 0; j < m.size; j++)
         s << p[j + i*m.size]<< " ";
      s << endl;
   }
   return s;
}

bool mapField::saveMap(char* fileName) { //save map to file fileName, return 1 if successful
   fstream f(fileName, fstream::out);
   if (f.fail())
      throw (string)"Can't save to file " + fileName;
   f << size << endl;
   f << *this << endl;
   f.close();
   return 1;
}

bool mapField::loadMap(char* fileName) { //load map from file fileName, return 1 if successful
   fstream f(fileName, fstream::in);
   if (f.fail())
      throw (string)"Can't load from file " + fileName;
   f >> size;
   sizeLog = log2int(size);
   sizeMod = size - 1;
   delete [] mapArray;
   delete [] info;
   delete [] allNodes;
   unsigned int limit = size*size;   
   mapArray = new int [limit];
   info = new char [limit];
   allNodes = new node [limit];
   for (int i = 0; i < limit; i++) {
      f >> mapArray[i];
      info[i] = 0;
      node n(i%size, i/size, 0);
      allNodes[i] = n;      
   }
   f.close();
   makeRegions();
   return 1;
}

/**********************************************

	Struct Stack needed to fill algorithm

***********************************************/

bool stack::pop(unsigned short &x, unsigned short &y) { 
   if(stackPointer > 0) { 
      int p = stack[stackPointer]; 
      y = p / h; 
      x = p % h; 
      stackPointer--; 
      return 1; 
   } else { 
      return 0; 
   }    
}

bool stack::push(unsigned short x, unsigned short y) {
   if(stackPointer < stackSize - 1) {
      stackPointer++;
      stack[stackPointer] = x + y*h;
      return 1;
   } else {
      return 0;
   }
}

bool stack::pop(int &x) {
   if(stackPointer > 0) {
      x = stack[stackPointer];
      stackPointer--;
      return 1;
   } else {
      return 0;
   }
}

bool stack::push(int x) { 
   if(stackPointer < stackSize - 1) {
      stackPointer++;
      stack[stackPointer] = x;
      return 1;
   } else { 
      return 0;
   }    
}     

void stack::emptyStack() {
   stackPointer = 0;
}
