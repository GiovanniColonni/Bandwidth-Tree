#include "Tree.h"
#include <iostream>
#include <vector>

Tree::Tree(){};

Tree::~Tree(){};

Tree::Tree(int b, int s, int e){
    band = b;
    start = s;
    end = e;
    amb = b;
};

void Tree::setBand(int b){
    band = b;
};

void Tree::setStart(int s){
    start = s;
};

void Tree::setAMB(int a){
    amb = a;
};

void Tree::setEnd(int e){
  end = e; 
};

void Tree::setP(Tree * n){
    p = n;
};

void Tree::setC1(Tree *n){
    c1 = n;
};

void Tree::setC2(Tree *n){
    c2 = n;
};

void Tree::setC3(Tree *n){
    c3 = n;
};

void Tree::toString(){
    std::cout << "s : " << getStart() << " e : " << getEnd() << " amb : " << getAMB() << std::endl;
    std::cout << "n childs : " << getNChild() << std::endl;

};

int Tree::getNChild(){

int count = 0;

        if(c1 != nullptr){
            count++;
        }
        if(c2 != nullptr){
            count++;
        }
        if(c3 != nullptr){
            count++;
        }

        return count;
}

bool Tree::isLeaf(){
        return ((getC1() == nullptr) && (getC2() == nullptr) && (getC3() == nullptr)); // getChild().size()
}

 std::vector<Tree *> Tree::getChilds(){
        std::vector<Tree *>v;
        v.emplace_back(c1);
        v.emplace_back(c2);
        v.emplace_back(c3);
        return v;
    };