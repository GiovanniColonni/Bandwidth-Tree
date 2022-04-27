#include <ranges>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <list>

//----------------
#include "Graph.h"
#include "Edge.h"
#include "Node.h"
#include "BandWidthTreeMethods.cpp"


using namespace std;

map<int,vector<Edge *>*> * Graph::getGraph(){
            return Graph::g;
}

vector<int> *  Graph::BFS(int source,int destination,map<int,vector<Edge*>*> * tmp_g){

    if(source == destination){ // gestire questo caso
        cout << "source == destination" << endl; 
    }
    vector<int> * path = new vector<int>();
    // create vector of the same size of the number of nodes
    vector<int> * visited = new vector<int>();
    
    // create the queue for the BFS
    queue<int> traversal;
    traversal.push(source);
    visited->push_back(source);
    
    while(!traversal.empty()){
        source = traversal.front();
        traversal.pop();
        vector<Edge *>::iterator it;
        map<int,vector<Edge *> *>::iterator map_it;

        map_it = tmp_g->find(source);
        for(Edge * e: *map_it->second){

            if(e->getNode() == destination){
                // founded !
                visited->push_back(e->getNode());
                return visited;
            }

            vector<int>::iterator founded = find(visited->begin(),visited->end(),e->getNode());
            if(founded == visited->end()){ // never visited
                visited->push_back(e->getNode()); // insert in visited
                traversal.push(e->getNode()); // next to check
            }

        } 
        
    }

    return nullptr; // path not found
}        


int * Graph::findRoute(map<int,vector<Edge *> *> * g_tmp,int source,int destination){
    

    int n = Graph::n_nodes;  // metterlo con construttore
    int * pred = new int[n];
    int dist[n];

    list<int> queue;
    bool visited[n];

    for (int i = 0; i < n;i++){
        visited[i] = 0;
        dist[i] = numeric_limits<int>::max();
        pred[i] = -1;
    }

    visited[source] = true;
    dist[source] = 0;
    queue.push_back(source);

    while (!queue.empty())
    {
        int u =  queue.front();
        cout << "visiting " << u << endl;
        queue.pop_front();
        map<int,vector<Edge *> *>::iterator map_it;
        map_it = g->find(u);
        for (Edge * e: *map_it->second){
            cout << "in " << u << " " << e->getNode() << endl; 
            cout << "visited ? " << visited[e->getNode()] << endl;    

            if(visited[e->getNode()] == false || visited[e->getNode()] == 127 ){
                visited[e->getNode()] = true;
                dist[e->getNode()] = dist[u] +1;
                pred[e->getNode()] = u;
                queue.push_back(e->getNode());

                if(e->getNode() == destination){ // found
                    return pred;
                }
            }
        }
    }
    
    
    return nullptr;
}

vector<int> * Graph::Feasibility(int s,int e, int b,int source,int destination){
    // copy the graph and remove all the edge that can't be allocated
    // because they can't satisfy the request
    map<int,vector<Edge*>*> * tmp_g = new map<int,vector<Edge *>*>(); 
    
    // remove from tmp_g the edges that can't be allocated
    vector<Edge *> * v = new vector<Edge *>();
    
    for(auto & [k,edges] : *g){
        vector<Edge *>::iterator it;
        
        for(it=edges->begin();it != edges->end(); it++){

            Node * t = (*it)->getRoot();

            auto minbw = methods::MinBW(t,s,e);
            if(minbw >= b){ 
                v->push_back(*it);
            }
        }
        // add the edges to the map
        vector<Edge *> * new_edges = new vector<Edge *>();
        for(it=v->begin();it != v->end(); it++){
            new_edges->push_back(*it);
        }
        
        tmp_g->insert(pair<int,vector<Edge *>*>(k,new_edges));
        v->clear();
    }
    
    cout << "starting DFS" << endl;
    // DFS ! 
    if(tmp_g->size() == 0){ // no path
        cout << "no path can be found in the graph, not enough resources" << endl;
        return nullptr;
    }
    int * paths = Graph::findRoute(tmp_g,source,destination);
    //vector<int> * path = Graph::BFS(source,destination,tmp_g);
    int c = destination;
    if(paths != nullptr){
        cout << "path for " << source << " to " << destination << endl;
        while(paths[c] != -1){
        cout << paths[c] << endl;
        c = paths[c];
        }
    }else{
        cout << "no path" << endl;
    }
       

    return nullptr;
}


/*
We can specialize the DFS algorithm to find a path between two given vertices u and z. 
i) Call DFS(G, u) with u as the start vertex. 
ii) Use a stack S to keep track of the path between the start vertex and the current vertex. 
iii) As soon as destination vertex z is encountered, return the path as the 
contents of the stack 
INFO :
https://iq.opengenus.org/path-between-nodes-directed-graph/ -> c'è anche la complessità !!!!!!!!
*/