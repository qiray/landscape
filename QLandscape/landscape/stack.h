#ifndef STACK_H
#define STACK_H

/**********************************************

        Stack needed to fill algorithm

***********************************************/

class Stack {
    int *stack;
    int stackPointer, stackSize, h;
public:
    Stack(int);
    ~Stack();
    bool pop(unsigned short&, unsigned short&);
    bool push(unsigned short, unsigned short);
    bool pop(int&);
    bool push(int);
    void emptyStack();
};

#endif // STACK_H
