#include <cstdlib>

#ifndef NODE_H

#define NODE_H

int log2int(int);

class node {
    static const int weight = 10;
    unsigned short x, y; // coordinates
    int h, g, f;
    node* parentNode;
public:
//    friend class mapField;
    node();
    node(unsigned short, unsigned short, node*);
    node* getParent() const;
    unsigned short getX() const;
    unsigned short getY() const;
    void setParent(node*);
    int H(node);
    int G(int);
    int F(node);
    void setF();
    void initF(int);
    void clearG();
    void clearH();
    int getF() const;
    int getG() const;
    int parentStraightLength(float) const;
    static int getWeight();
};

#endif
