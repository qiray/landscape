#include <iostream>
#include <algorithm>
#include <vector>
#include "node.h"

#ifndef BINARY_HEAP_H

#define BINARY_HEAP_H

class binary_heap {
    std::vector<node*> list;
    int size;
public:
    binary_heap();
    void add(node*);
    node* getMin();
    bool isEmpty();
    void clear();
    void print();
};

#endif
