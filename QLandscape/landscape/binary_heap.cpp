#include "binary_heap.h"

binary_heap::binary_heap() {
    size = 0;
    maxSize = 8;
    list = new node* [maxSize];
}

binary_heap::~binary_heap() {
    delete [] list;
}

void binary_heap::push_back(node *n) {
    if (size == maxSize) {//resize array
        maxSize *= 2;
        node **list2 = new node* [maxSize];
        memcpy(list2, list, static_cast<size_t>(size)*sizeof(node*));
        delete [] list;
        list = list2;
        list[size++] = n;
    } else {
        list[size++] = n;
    }
}

void binary_heap::pop_back() {////////////////////////
    list[size--] = nullptr;
}

void binary_heap::add(node *n) {
    push_back(n);
    int i = size - 1;
    int parent = (i - 1) >> 1;
    while (i > 0 && list[parent]->f > list[i]->f) {
        node *temp = list[i];
        list[i] = list[parent];
        list[parent] = temp;
        i = parent;
        parent = (i - 1) >> 1;
    }
}

void binary_heap::heapify(int i) {
    int leftChild;
    int rightChild;
    int largestChild;
    while(1) {
        leftChild = (i << 1) + 1;
        rightChild = (i << 1) + 2;
        largestChild = i;
        if (leftChild < size && list[leftChild]->f < list[largestChild]->f) {
            largestChild = leftChild;
        }
        if (rightChild < size && list[rightChild]->f < list[largestChild]->f) {
            largestChild = rightChild;
        }
        if (largestChild == i) {
            return;
        }
        node *temp = list[i];
        list[i] = list[largestChild];
        list[largestChild] = temp;
        i = largestChild;
    }
}

void binary_heap::buildHeap() {
    for (int i = size/2; i >= 0; i--)
        heapify(i);
}

node* binary_heap::getMax() {
    node *result = list[0];
    list[0] = list[size - 1];
    pop_back();
    heapify(0);
    return result;
}

bool binary_heap::empty() {
    if (size == 0)
        return 1;
    else
        return 0;
}
