#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <map>

#include "Edge.h"


class Graph{
    private:
        int n_nodes;
        int n_edges;
        std::map<int *,std::vector<Edge*>*> * g;
    
    public:
        Graph();
        
        Graph(int nodes,int edges){
            n_nodes = nodes;
            n_edges = edges;
            g = new std::map<int *,std::vector<Edge*>*>();
        };
        
        ~Graph();

        std::map<int *,std::vector<Edge *>*> * getGraph(){
            return g;
        }
        void insertNode(int * node,std::vector<Edge *>* edges){
            g->insert( std::pair<int *,std::vector<Edge *>*>(node,edges) );
        }
        std::vector<int> Feasibility(int b,int t1,int t2);
};


#endif // _
