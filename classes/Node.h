#ifndef NODE_H
#define NODE_H
#include <vector>
class Node{

private:
    int band;
    int start;
    int end;

    Node *p = nullptr;
    Node *c1 = nullptr;
    Node *c2 = nullptr;
    Node *c3 = nullptr;
    
public:
    Node();
    Node(int band, int start, int end);

    void setBand(int band);
    void setStart(int start);
    void setEnd(int end);
    void setP(Node *n);
    void setC1(Node *n);
    void setC2(Node *n);
    void setC3(Node *n);


    int getBand() { return band; }
    int getStart() { return start; }
    int getEnd()  { return end; }

    Node * getP(){return p;}
    Node * getC1(){return c1;}
    Node * getC2(){return c2;}
    Node * getC3(){return c3;}

};

#endif