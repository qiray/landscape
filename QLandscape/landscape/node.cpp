#include "node.h"

node::node() {
    x = 0;
    y = 0;
    parentNode = nullptr;
    region = 0;
}

node::node (unsigned short X, unsigned short Y, node* parent) {
    x = X;
    y = Y;
    parentNode = parent;
    region = 0;
    if (parent)
        g = parent->g + weight;
    else
        g = 0;
    h = 0;//h will be calculated in function H(node)
}

node* node::getParent() const {
    return parentNode;
}

unsigned short node::getX() const {
    return x;
}

unsigned short node::getY() const {
    return y;
}

void node::setParent(node* n) {
    parentNode = n;
}

int node::G() {
    if (g == 0)
        return g = parentNode->g + weight;
    else
        return g;
}

int node::G(int value) {
    if (g == 0)
        return g = parentNode->g + weight + value;
    else
        return g;
}

int node::H(node stop) {
    h = (abs(stop.x - x) + abs(stop.y - y))*weight;
    return h;
}

int node::F(node stop) {
    f = G() + H(stop);
    return f;
}

int node::getF() const {
    return f;
}

int node::getG() const {
    return g;
}

int node::parentStraightLength(float roughness) const {
    node *current = const_cast<node*>(this), *parent = current->parentNode;
    int len = 0, prevDiff = 0;
    if (parent)
        prevDiff = current->x - parent->x + (current->y - parent->y)*10;
    while (parent) {
        int diff = current->x - parent->x + (current->y - parent->y)*10;
        if (diff != prevDiff)
            break;
        current = parent;
        parent = parent->parentNode;
        prevDiff = diff;
        len++;
    }
    return static_cast<int>(len*weight*roughness);
}

int log2int(int n) {
    int result = 0;
    n = abs(n);
    while(n >>= 1)
        result++;
    return result;
}
