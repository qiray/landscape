#include <iostream>
#include <cstring>//for memset and memcpy
#include "node.h"

#ifndef BINARY_HEAP_H

#define BINARY_HEAP_H

class binary_heap {
    node** list;
    int size, maxSize;
    void push_back(node*);
    void pop_back();
public:
    int getSize() {return size;}
    binary_heap();
    ~binary_heap();
    int heapSize() {return size;}
    void add(node*);
    void heapify(int);
    void buildHeap();
    node* getMax();
    bool empty();
    void clear() {size = 0;}
};

#endif
