
#include "Node.h"
#include <vector>

Node::Node(){};

Node::Node(int b, int s, int e){
    band = b;
    start = s;
    end = e;
};

void Node::setBand(int b){
    band = b;
};

void Node::setStart(int s){
    start = s;
};

void Node::setEnd(int e){
  end = e; 
};

void Node::setP(Node * n){
    p = n;
};

void Node::setC1(Node *n){
    c1 = n;
};

void Node::setC2(Node *n){
    c2 = n;
};

void Node::setC3(Node *n){
    c3 = n;
};

  




