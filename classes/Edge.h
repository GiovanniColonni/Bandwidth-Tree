#ifndef Edge_H
#define Edge_H
#include "Tree.h" // bandwidth tree


class Edge{
    private:
        Tree * root;
        int node;
    
    public:
        Edge();
        ~Edge();
        Edge(int  n, Tree * r){
            node = n;
            root = r;
        };
        

        void setRoot(Tree * r){
            root = r;
        }
        Tree * getRoot(){
            return root;
        }

        int getTree(){
            return node;
        }
    
};

#endif 