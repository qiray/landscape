#include "binary_heap.h"
#include <iostream>

binary_heap::binary_heap() {
    size = 0;
}

void binary_heap::add(node *n) {
    list.push_back(n);
    size++;
    push_heap(list.begin(), list.end(), [](node* a, node *b) {return a->getF() > b->getF();});
}

node* binary_heap::getMax() {
    node* result = list.front();
    pop_heap(list.begin(), list.end());
    list.pop_back();
    size--;
    return result;
}

bool binary_heap::isEmpty() {
    return size == 0;
}

void binary_heap::clear() {
    size = 0;
    list.clear();
}
