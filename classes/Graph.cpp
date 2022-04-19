#include "Graph.h"
#include "Edge.h"


std::vector<int> Feasibility(int s,int d, int b,int t1,int t2){
    // copy the graph and remove all the edge that can't be allocated
    // because they can't satisfy the request

}

/*
We can specialize the DFS algorithm to find a path between two given vertices u and z. 
i) Call DFS(G, u) with u as the start vertex. 
ii) Use a stack S to keep track of the path between the start vertex and the current vertex. 
iii) As soon as destination vertex z is encountered, return the path as the 
contents of the stack 
INFO :
https://iq.opengenus.org/path-between-nodes-directed-graph/
*/