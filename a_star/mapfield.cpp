#include "mapfield.h"

stack coordStack;//stack struct (described below)

string intToStr(unsigned short n) { //function converts unsigned short to string
   string tmp;
   if (n == 0)
      return "0";
   while (n > 0) {
      char t[] = {n%10 + '0'};
      tmp.insert(0, t);
      n /= 10;
   }
   return tmp;
}

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

void generateBarriers(int* p, unsigned int limit, int num) {
   srand (time(NULL));
   for (int i = 0; i < limit; i++)
      if (rand()%10000 < num)
         p[i] = blockedCell;   //barrier
      else
         p[i] = emptyCell;   //empty land   
}

void generateField(unsigned short Size, int info, mapField &m) {//generates map which size is Size*Size
   m.size = Size;
   m.sizeLog = log2int(m.size);
   m.sizeMod = m.size - 1;
   delete [] m.mapArray;
   unsigned int limit = Size*Size;
   m.mapArray = new int [limit];
   int* p = m.mapArray;
   switch (info) {
      case 0: //empty land
         for (int i = 0; i < limit; i++) 
            p[i] = emptyCell;
         break;
      case 1: //few barriers
         generateBarriers(p, limit, 2000);
         break;  
      case 2: //medium barriers
         generateBarriers(p, limit, 3500);
         break;   
      case 3: //many barriers
         generateBarriers(p, limit, 7000);
         break;               
   }
}

bool mapField::isEmpty(unsigned short x, unsigned short y) { //return 1 if point (x, y) is empty at this map
   if (x >= size)
      throw (string)"" + intToStr(x) + " is too big. Max size is only " + intToStr(size);
   if (y >= size)
      throw (string)"" + intToStr(y) + " is too big. Max size is only " + intToStr(size);
   if (mapArray[x + y*size] != blockedCell)
      return true;
   else
      return false;
}

bool mapField::makeStart(unsigned short x, unsigned short y, node &Start, int &prevShift) {//make start point at x, y and return 1 if successful
   if (isEmpty(x, y)) {
      unsigned int limit = size*size, shift = x + y*size;
      mapArray[prevShift] = 0;//previous start point 
      mapArray[shift] = startCell;//start point here
      Start.parentNode = 0;
      Start.x = x;
      Start.y = y;
      prevShift = shift;
      return 1;
   } else {
      throw (string) "Impossible to make start point at " + intToStr(x) + ", " + intToStr(y);
      //return 0;//of course it is strange but it's only temporary or not :-)
   }
}

bool mapField::makeStop(unsigned short x, unsigned short y, node &Stop, int &prevStatus, int &prevShift) {//make stop point at x, y and return 1 if successful
   unsigned int limit = size*size, shift = x + y*size;
   if (x >= size)
      throw (string)"" + intToStr(x) + " is too big. Max size is only " + intToStr(size);
   if (y >= size)
      throw (string)"" + intToStr(y) + " is too big. Max size is only " + intToStr(size);
   mapArray[prevShift] = prevStatus; //change back info about previous stop
   prevShift = x + y*size;
   prevStatus = mapArray[prevShift];
   mapArray[prevShift] = stopCell;//stop point here
   Stop.parentNode = 0;
   Stop.x = x;
   Stop.y = y;
   return 1;
}

int mapField::returnInfo(unsigned short x, unsigned short y) {//return information about point (x,y)
   if (x >= size)
      throw intToStr(x) + " is too big. Max size is only " + intToStr(size);
   if (y >= size)
      throw intToStr(y) + " is too big. Max size is only " + intToStr(size);
   return mapArray[x + y*size];
}

void mapField::setInfo(unsigned short x, unsigned short y, int Z) {//set information about point (x,y)
   if (x >= size)
      throw intToStr(x) + " is too big. Max size is only " + intToStr(size);
   if (y >= size)
      throw intToStr(y) + " is too big. Max size is only " + intToStr(size);
   mapArray[x + y*size] = Z;
}

void mapField::generateStartStop(node &start, node &stop, int &prevStatus, int &prevStart, int &prevStop) {
   srand (time(NULL));
   unsigned int lim = size*size - 1, k = 0;
   do
      k = rand() & lim;
   while (mapArray[k] == blockedCell);
   makeStart(k & sizeMod, k >> sizeLog, start, prevStart);
   srand (k + time(NULL));
   do
      k = rand() & lim;
   while (mapArray[k] == blockedCell);
   prevStatus = mapArray[k];
   makeStop(k & sizeMod, k >> sizeLog, stop, prevStatus, prevStop);
}

node mapField::loadStart() {
   unsigned int limit = size*size;
   for(unsigned int i = 0; i < limit; i++)
      if(mapArray[i] == startCell)
         return node(i & sizeMod, i >> sizeLog, 0);
   throw (string) "No start point found";
}

node mapField::loadStop() {
   unsigned int limit = size*size;
   for(unsigned int i = 0; i < limit; i++)
      if(mapArray[i] == stopCell)
         return node(i & sizeMod, i >> sizeLog, 0);
   throw (string) "No stop point found";
}

/**********************************************

	Struct Stack needed to fill algorithm

***********************************************/

int push1 = 0, pop1 = 0;

bool stack::pop(unsigned short &x, unsigned short &y) { 
//pop1++;
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
//push1++;
   if(stackPointer < stackSize - 1) {
      stackPointer++;
      stack[stackPointer] = x + y*h;
      return 1;
   } else {
      return 0;
   }
}

bool stack::pop(int &x) {
//pop1++;
   if(stackPointer > 0) {
      x = stack[stackPointer];
      stackPointer--;
      return 1;
   } else {
      return 0;
   }
}

bool stack::push(int x) { 
//push1++;
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
