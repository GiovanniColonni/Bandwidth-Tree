#ifndef Edge_H
#define Edge_H
#include "Node.h" // bandwidth tree
class Edge{
    private:
        Node * root;
        int * node;
    
    public:
        Edge();
         Edge(int * n, Node * r){
            node = n;
            root = r;
        };
        ~Edge();
       

        void setRoot(Node * r){
            root = r;
        }
        Node * getRoot(){
            return root;
        }

        int * getNode(){
            return node;
        }
    
};
#endif 