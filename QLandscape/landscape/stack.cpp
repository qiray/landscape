#include "stack.h"

Stack::Stack(int size)
{
    stackPointer = 0;
    h = size;
    stackSize = 8*size*size;//max size to avoid stack overflow
    stack = new int [stackSize];
}

Stack::~Stack()
{
    delete [] stack;
}

bool Stack::pop(unsigned short &x, unsigned short &y) {
   if(stackPointer > 0) {
      int p = stack[stackPointer];
      y = static_cast<unsigned short>(p / h);
      x = static_cast<unsigned short>(p % h);
      stackPointer--;
      return 1;
   } else {
      return 0;
   }
}

bool Stack::push(unsigned short x, unsigned short y) {
   if(stackPointer < stackSize - 1) {
      stackPointer++;
      stack[stackPointer] = x + y*h;
      return 1;
   } else {
      return 0;
   }
}

bool Stack::pop(int &x) {
   if(stackPointer > 0) {
      x = stack[stackPointer];
      stackPointer--;
      return 1;
   } else {
      return 0;
   }
}

bool Stack::push(int x) {
   if(stackPointer < stackSize - 1) {
      stackPointer++;
      stack[stackPointer] = x;
      return 1;
   } else {
      return 0;
   }
}

void Stack::emptyStack() {
   stackPointer = 0;
}
