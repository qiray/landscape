#include "binary_heap.h"
#include <iostream>

auto compare = [](node* a, node *b) {return a->getF() > b->getF();};

binary_heap::binary_heap() {
    size = 0;
    make_heap(list.begin(), list.end(), compare);
}

void binary_heap::add(node *n) {
    list.push_back(n);
    size++;
    push_heap(list.begin(), list.end(), compare);
}

node* binary_heap::getMin() {
    pop_heap(list.begin(), list.end(), compare);
    node* result = list.back();
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

void binary_heap::print() {
    std::cout << "heap: ";
    for (auto i : list) std::cout << i->getF() << ' ';
    std::cout << '\n';
}
