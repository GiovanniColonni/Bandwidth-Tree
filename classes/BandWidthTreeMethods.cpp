#ifndef BTM
#define BTM

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
//----------------
#include "Tree.h"
#include "Graph.h"
#include "Edge.h"

using namespace std;

namespace methods{

inline int amb(Tree *v){ // sum of band from v to root node
    
    int amb_;
    int amb_i;

    
    if(v->getP() == nullptr){
        return v->getBand();    
    };
    
    amb_i = methods::amb(v->getP());

    amb_ = v->getBand() + amb_i;
    return amb_;
};


inline int MinBW(Tree * n,int s_i,int e_i){ // find minimum bandwidth on the interval [s_i,e_i)
    
    if(s_i > e_i){
        std::cout << "s_i > e_i" << std::endl;
        return 0;
    }
    
    if(!(n->getStart() <= s_i) || !(n->getEnd() >= e_i)){ 
        return 0;
    };


    int minBW = std::numeric_limits<int>::max(); 
    bool is_leaf = (n->getC1() == nullptr) && (n->getC2() == nullptr) && (n->getC3() == nullptr);
    
    if(((n->getStart() == s_i) && (n->getEnd() == e_i)) || is_leaf){ 
        return n->getAMB();
    };
    
    std::vector<Tree *> v = {n->getC1(),n->getC2(),n->getC3()};

    for(auto & child : v)
    {
        if(child != nullptr){
        

        bool contained = (s_i <= child->getStart() && child->getStart() < e_i) ||
                         ((child->getStart() <= s_i && s_i <=  child->getEnd()) && (child->getStart() <= e_i && e_i <=  child->getEnd()) )
                         || (s_i < child->getEnd() && child->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
        
        if(contained){
            int s = std::max(child->getStart(),s_i);
            int e = std::min(child->getEnd(),e_i);

            int value = MinBW(child,s,e);

            if(minBW > value){
                minBW = value;
            }
        }
    }
       
    }
    return minBW;

};

inline Graph * createGraph(){ // read the graph from file

    // ASSUMPTIONS:
    // 1. default bandwidth 10 kbit
    // 2. initially you always have all the capacity
    // 3. range time tra 0 e 60
    std::ifstream topology("./topology.txt");
    std::string l;
    if(!topology.is_open()){
        std::cout << "can't open topology file..it exist?" << std::endl;
        return nullptr;
    }
    
    // number of nodes in the graph is in the first line of the file
    getline(topology,l);
    int n_nodes = std::stoi(&l[0]);
    Graph * g = new Graph(n_nodes,1);
    l = "";
    std::cout << n_nodes << std::endl;

    while(topology){

        std::vector<Edge *> * v = new std::vector<Edge *>();
        getline(topology,l);
        
        if(l != ""){
            int n = std::stoi(&l[0]);
            for(auto i = 1; i < l.size(); i++){
                if(l[i] != ' '){
                   int e_to = std::stoi(&l[i]);
                   Tree * root = new Tree(10,0,60); // sostituire con costanti
                   Edge * e = new Edge(e_to,root);
                   v->push_back(e);
                        
                }
            }
            // inserting node + edge list in the map
            //cout << "1" << endl;
            g->insertTree(n,v);
            //cout << "2" << endl;
            
        }
        l = "";
    }
    topology.close();
    return g;
}

inline void merge(Tree * n,int s_i, int e_i){
    // if the node has some leaf then try to merge it 
    // otherwise 
    bool contained = (s_i <= n->getStart() && n->getStart() < e_i) ||
                         ((n->getStart() <= s_i && s_i <=  n->getEnd()) && (n->getStart() <= e_i && e_i <=  n->getEnd()) )
                         || (s_i < n->getEnd() && n->getEnd() <= e_i); // [c,d) int [l(u),r(u))
    
    if (!contained){
        return;
    }
    auto childs = n->getChilds();
    for (auto c: childs){
        if(c != nullptr){
            
        }
    }
    
    return;
}

inline void balance(Tree * n){
    return;
}

inline void decrement_bandwidth(Tree * n,int w,int c,int d){

    if(n->getStart() == c && n->getEnd() == d){ // entire interval
        n->setAMB(n->getAMB() - w);
        // decrement also the childs
        if(n->isLeaf()){
            return;
        }
        auto childs = n->getChilds();
        for(auto v:childs){
            decrement_bandwidth(v,w,v->getStart(),v->getEnd());
        };
        return;
}

}

inline int processLeaf(Tree * v1,int w,int s_i,int e_i){
        //std::cout << "   processing leaf" << std::endl;
        auto n_amb = (v1->getAMB() -w);
        auto amb = v1->getAMB();

        if(v1->getStart() == s_i && v1->getEnd() == e_i){ 
            //std::cout << "complete leaf" << std::endl;
            // entire node so no extra childs needed
            // decrement bandwidth
            v1->setAMB(n_amb);
            return n_amb; 
        } 

       

        if(v1->getStart() == s_i && v1->getEnd() > e_i){
            // l
            //std::cout << "right part" << std::endl;
            Tree * C1 = new Tree(0,s_i,e_i);
            C1->setAMB(n_amb);
            v1->setC1(C1);
            
            // r
            Tree * C3 = new Tree(0,e_i,v1->getEnd());
            C3->setAMB(amb);
            v1->setC3(C3);
             // same bw
            // sub root
            v1->setAMB(n_amb);
            return n_amb;
        }
        if(v1->getStart() < s_i && v1->getEnd() == e_i){
            //std::cout << "left part" << std::endl;
            // l
            Tree * C1 = new Tree(0,v1->getStart(),s_i);
            v1->setC1(C1);
            C1->setAMB(amb);
            // r
            Tree * C3 = new Tree(0,s_i,v1->getEnd());
            v1->setC3(C3);
            C3->setAMB(n_amb); // same bw
            // sub root
            v1->setAMB(n_amb); 
            return n_amb;
        }
        // split 3 child 
        //std::cout << "3 child" << std::endl;
        Tree * C1 = new Tree(0,v1->getStart(),s_i);
        Tree * C2 = new Tree(0,s_i,e_i);
        Tree * C3 = new Tree(0,e_i,v1->getEnd());


        C1->setAMB(amb);
        C2->setAMB(n_amb);
        C3->setAMB(amb);

        v1->setC1(C1);
        v1->setC2(C2);
        v1->setC3(C3);
        
       
        v1->setAMB(n_amb); 
        
        return n_amb;
}

inline int split(Tree * v1,Tree * v2,Tree * v3,int count, int w, int s_i,int e_i){ // forse devo ritornare il valore di count
    int rv1 = v1->getEnd();
    int lv1 = v1->getStart();
    
    count++;

    bool contained = (s_i <= v1->getStart() && v1->getStart() < e_i) ||
                         ((v1->getStart() <= s_i && s_i <=  v1->getEnd()) && (v1->getStart() <= e_i && e_i <=  v1->getEnd()) )
                         || (s_i < v1->getEnd() && v1->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
    if(!contained){
        //std::cout << "it: " << count << " not contained" << std::endl;
        return -1;
    }
    
    bool is_leaf = v1->isLeaf();

    if(v1->getStart() <= s_i && v1->getEnd() >= e_i && !is_leaf){
        
        // IF THE NODE is contained by the req interval then simply decrement and return

        if(v1->getStart() == s_i && v1->getEnd() == e_i){
            std::cout << "it: " << count << " full node" << std::endl;
            v1->setAMB(v1->getAMB() - w);
            return (v1->getAMB() - w);
        }
        // ELSE call split into the childs of that node for the interested interval by the child
        // FOR EACH OF THE CHILD 
        // then return
        auto childs = v1->getChilds();
        int minBW = std::numeric_limits<int>::max();
        int b; 
        for(auto v:childs){
            //std::cout << "it: " << count << " entering child cycle" << std::endl;
            if(v != nullptr){
            auto n_s = std::max(s_i,v->getStart());
            auto n_e = std::min(e_i,v->getEnd());
            //std::cout << "it: " << count << " n_s " << n_s << " n_e " << n_e << std::endl;
            
            if(n_s < n_e){ // means no interception between node interval and requested interval
                b = split(v,nullptr,nullptr,count,w,n_s,n_e);
                //std::cout << "it: " << count << " child" << std::endl;
                if(minBW > b){
                    minBW = b;
                }
            }
            }else{
                //std::cout << "it: "<< count << " nullptr child" << std::endl;
            }
            // SET bandwidth of this node as the new minimum among the child
            // IF this.getAMB() > new minimum
        }
        //std::cout << "it : " << count << " b " << b << "min BW " << minBW << std::endl;
        if(v1->getAMB() > minBW){ // new minimum among the childs
            //std::cout << "it : " << count << " new minimum " << minBW << std::endl;
            v1->setAMB(minBW);
        }
        return minBW; // counter = 
    }
     
    if(is_leaf){
        // process child
        //std::cout << "it: " << count << " on the leaf" << std::endl;
        return processLeaf(v1,w,s_i,e_i);
    }
    //std::cout << "here" << std::endl;
    return 0;
}

inline Tree * AllocateBW(Tree * u,int w,int s_i,int e_i){
    int minBW;
    minBW = methods::MinBW(u,s_i,e_i);
    
    if(w > 0 && s_i > e_i){
        std::cout << "w <= 0 or s_i > e_i" << std::endl;
        return nullptr;
    };

    if(minBW < w){
        std::cout << "Not enough bandwidth available in " << s_i << "," << e_i << std::endl;
        return nullptr;
    };
    //std::cout << "entering split" << std::endl;
    auto a  = split(u,nullptr,nullptr,0,w,s_i,e_i);
    
    return u;
};

inline void printTrees(std::ofstream &file,Tree * n){
    
    file << "(" << n << ";" << n->getStart() << ";" << n->getEnd() << ";"<< n->getAMB() <<")," << n->getC1() << "," << n->getC2() << "," << n->getC3() << std::endl;
    
    if(n->getC1() != nullptr){
        printTrees(file,n->getC1());
    }
    if(n->getC2() != nullptr){
        printTrees(file,n->getC2());
    }
    if(n->getC3() != nullptr){
        printTrees(file,n->getC3());
    }
    return;
}

inline void printTree(Tree * root){
 // each line is a node + childs
 // node child1 child 2 child 3 in the form of pointers (since they are unique)
 std::ofstream tree_file ("tree.txt");
 if (tree_file.is_open()){
     printTrees(tree_file,root);
 }else{
     std::cout << "file not opened" << std::endl;
 }
 
 tree_file.close();

};

inline int MaxLinkDuration(Tree * root,int ts,int w){
    // trovare intervallo consecutivo più lungo
    return 0;
}


}; // END namespace
#endif