#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>

#include "Edge.h"



class Graph{
    private:
        int n_nodes;
        std::map<int,std::vector<Edge*>*> * g;
    
    public:
        Graph();
        
        Graph(int nodes){
            n_nodes = nodes;
            g = new std::map<int,std::vector<Edge*>*>();
        };
        
        ~Graph();

        std::map<int,std::vector<Edge *>*> * getGraph();

        void insertTree(int node,std::vector<Edge *>* edges){
            g->insert( std::pair<int,std::vector<Edge *>*>(node,edges) );
        }
        std::vector<int> * Feasibility(int s,int e,int b,int source,int destination);
        std::vector<int> * BFS(int source,int destination,std::map<int,std::vector<Edge*>*> * tmp_g);
        int * findRoute(std::map<int,std::vector<Edge *> *> * g_tmp,int source,int destination);
};  


#endif // _
