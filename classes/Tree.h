#ifndef TREE_H
#define TREE_H
#include <vector>

class Tree{

private:
    int band;
    int start;
    int end;
    int amb;

    Tree *p = nullptr;
    Tree *c1 = nullptr;
    Tree *c2 = nullptr;
    Tree *c3 = nullptr;
    
public:
    Tree();
    Tree(int band, int start, int end);
    ~Tree();

    void setBand(int band);
    void setStart(int start);
    void setAMB(int amb);
    void setEnd(int end);
    void setP(Tree *n);
    void setC1(Tree *n);
    void setC2(Tree *n);
    void setC3(Tree *n);


    int getBand(){ return band; }
    int getStart(){ return start; }
    int getEnd(){ return end; }
    int getAMB(){return amb;}
    int getNChild();

    void toString();
    
    bool isLeaf();

    std::vector<Tree *> getChilds();

    Tree * getP(){return p;}
    Tree * getC1(){return c1;}
    Tree * getC2(){return c2;}
    Tree * getC3(){return c3;}


};

#endif