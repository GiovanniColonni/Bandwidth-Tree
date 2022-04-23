#ifndef BANDWIDTHTREEMETHODS
#define  BANDWIDTHTREEMETHODS

#include <iostream>
#include "Node.h"
#include <limits>
#include <vector>
#include "Graph.h"
#include "Edge.h"
#include <string>
#include <cstdlib>
#include <fstream>

namespace methods{

static int MinBW(Node * n,int s_i,int e_i){ // find minimum bandwidth on the interval [s_i,e_i)
    
    if(s_i > e_i){
        std::cout << "s_i > e_i" << std::endl;
        return 0;
    }

    //it++;
    
    if(!(n->getStart() <= s_i) || !(n->getEnd() >= e_i)){ // GUARDARE DELL INTERVALLO NON COMPLETO [)
        //cout << "Not contained" << endl;
        return 0;
    };


    int minBW = std::numeric_limits<int>::max(); // not really infinite but 2^31-1
    bool is_leaf = (n->getC1() == nullptr) && (n->getC2() == nullptr) && (n->getC3() == nullptr); // getChild().size()
    
    if(((n->getStart() == s_i) && (n->getEnd() == e_i)) || is_leaf){ //  [)
        //cout << "iteration " << it << " here leaf or complete, value : " << amb(n) <<  endl;
        return n->getAMB();
    };
    
    std::vector<Node *> v = {n->getC1(),n->getC2(),n->getC3()};
    //cout << "iteration " << it << " looking into childs : " << endl;

    for(auto & child : v)
    {
        if(child != nullptr){
        

        bool contained = (s_i <= child->getStart() && child->getStart() < e_i) ||
                         ((child->getStart() <= s_i && s_i <=  child->getEnd()) && (child->getStart() <= e_i && e_i <=  child->getEnd()) )
                         || (s_i < child->getEnd() && child->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
        
        if(contained){
            int s = std::max(child->getStart(),s_i);
            int e = std::min(child->getEnd(),e_i);

            //cout << "iteration " << it <<" s,e = " << s << "," << e << endl;

            int value = MinBW(child,s,e);

            //cout << "iteration " << it << " value : " << value << " for " << child->getStart() << "," << child->getEnd() << endl;


            if(minBW > value){
                //cout << "iteration " << it << " new minBW " << value << " old one " << minBW << endl;
                minBW = value;
            }
        } else{

                //cout << "iteration " << it << " not contained : " << child->getStart() << "," << child->getEnd() << endl;
            }
        }
       
    }
    //cout << "iteration " << it << " final minBW " << minBW  <<  endl;
    return minBW;

};

static Graph * createGraph(){ // read the graph from file

    // ASSUMPTIONS:
    // 1. default bandwidth 10 kbit
    // 2. initially you always have all the capacity
    // 3. range time tra 0 e 60
    Graph * g = new Graph(1,1);
    std::ifstream topology("./topology.txt");
    std::string l;
    if(!topology.is_open()){
        std::cout << "can't open topology file..it exist?" << std::endl;
        return nullptr;
    }
    while(topology){

        std::vector<Edge *> * v = new std::vector<Edge *>();
        getline(topology,l);
        
        if(l != ""){
            int n = std::stoi(&l[0]);
            for(auto i = 1; i < l.size(); i++){
                if(l[i] != ' '){
                   int e_to = std::stoi(&l[i]);
                   Node * root = new Node(10,0,60); // sostituire con costanti
                   Edge * e = new Edge(e_to,root);
                   v->push_back(e);
                        
                }
            }
            // inserting node + edge list in the map
            //cout << "1" << endl;
            g->insertNode(n,v);
            //cout << "2" << endl;
            
        }
        l = "";
    }
    topology.close();
    return g;
}

static void merge(Node * n,int s_i, int e_i){
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

static void balance(Node * n){
    return;
}

static void decrement_bandwidth(Node * n,int w,int c,int d){

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

static int processLeaf(Node * v1,int w,int s_i,int e_i){
        std::cout << "   processing leaf" << std::endl;
        auto n_amb = (v1->getAMB() -w);
        auto amb = v1->getAMB();

        if(v1->getStart() == s_i && v1->getEnd() == e_i){ 
            std::cout << "complete leaf" << std::endl;
            // entire node so no extra childs needed
            // decrement bandwidth
            v1->setAMB(n_amb);
            return n_amb; 
        } 

       

        if(v1->getStart() == s_i && v1->getEnd() > e_i){
            // l
            std::cout << "right part" << std::endl;
            Node * C1 = new Node(0,s_i,e_i);
            C1->setAMB(n_amb);
            v1->setC1(C1);
            
            // r
            Node * C3 = new Node(0,e_i,v1->getEnd());
            C3->setAMB(amb);
            v1->setC3(C3);
             // same bw
            // sub root
            v1->setAMB(n_amb);
            return n_amb;
        }
        if(v1->getStart() < s_i && v1->getEnd() == e_i){
            std::cout << "left part" << std::endl;
            // l
            Node * C1 = new Node(0,v1->getStart(),s_i);
            v1->setC1(C1);
            C1->setAMB(amb);
            // r
            Node * C3 = new Node(0,s_i,v1->getEnd());
            v1->setC3(C3);
            C3->setAMB(n_amb); // same bw
            // sub root
            v1->setAMB(n_amb); 
            return n_amb;
        }
        // split 3 child 
        std::cout << "3 child" << std::endl;
        Node * C1 = new Node(0,v1->getStart(),s_i);
        Node * C2 = new Node(0,s_i,e_i);
        Node * C3 = new Node(0,e_i,v1->getEnd());


        C1->setAMB(amb);
        C2->setAMB(n_amb);
        C3->setAMB(amb);

        v1->setC1(C1);
        v1->setC2(C2);
        v1->setC3(C3);
        
       
        v1->setAMB(n_amb); 
        
        return n_amb;
}

static int split(Node * v1,Node * v2,Node * v3,int count, int w, int s_i,int e_i){ // forse devo ritornare il valore di count
    int rv1 = v1->getEnd();
    int lv1 = v1->getStart();
    
    count++;

    bool contained = (s_i <= v1->getStart() && v1->getStart() < e_i) ||
                         ((v1->getStart() <= s_i && s_i <=  v1->getEnd()) && (v1->getStart() <= e_i && e_i <=  v1->getEnd()) )
                         || (s_i < v1->getEnd() && v1->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
    if(!contained){
        std::cout << "it: " << count << " not contained" << std::endl;
        return -1;
    }
    
    bool is_leaf = v1->isLeaf();

    if(v1->getStart() <= s_i && v1->getEnd() >= e_i && !is_leaf){
        std::cout << "it: " << count << " contained egual" << std::endl;
        //v1->setAMB(v1->getAMB() - w);
        // IF THE NODE is contained by the req interval then simply decrement and return

        //v1->setAMB(v1->getAMB() - w); // ERRORE, LA BANDA MINIMA ORA é LA BANDA MINIMA TRA I FIGLI COINVOLTI
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
            std::cout << "it: " << count << " entering child cycle" << std::endl;
            if(v != nullptr){
            auto n_s = std::max(s_i,v->getStart());
            auto n_e = std::min(e_i,v->getEnd());
            std::cout << "it: " << count << " n_s " << n_s << " n_e " << n_e << std::endl;
            
            if(n_s < n_e){ // means no interception between node interval and requested interval
                b = split(v,nullptr,nullptr,count,w,n_s,n_e);
                std::cout << "it: " << count << " child" << std::endl;
                if(minBW > b){
                    minBW = b;
                }
            }
            }else{
                std::cout << "it: "<< count << " nullptr child" << std::endl;
            }
            // SET bandwidth of this node as the new minimum among the child
            // IF this.getAMB() > new minimum
        }
        std::cout << "it : " << count << " b " << b << "min BW " << minBW << std::endl;
        if(v1->getAMB() > minBW){ // new minimum among the childs
            std::cout << "it : " << count << " new minimum " << minBW << std::endl;
            v1->setAMB(minBW);
        }
        return minBW; // counter = 
    }
     
    if(is_leaf){
        // process child
        std::cout << "it: " << count << " on the leaf" << std::endl;
        return processLeaf(v1,w,s_i,e_i);
    }
    std::cout << "here" << std::endl;
    return 0;
}

static Node * AllocateBW(Node * u,int w,int s_i,int e_i){
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
    std::cout << "entering split" << std::endl;
    auto a  = split(u,nullptr,nullptr,0,w,s_i,e_i);
    // CALL MERGE 
    // BALANCE THE TREE
    return u;
};

static void printNodes(std::ofstream &file,Node * n){
    
    file << "(" << n << ";" << n->getStart() << ";" << n->getEnd() << ";"<< n->getAMB() <<")," << n->getC1() << "," << n->getC2() << "," << n->getC3() << std::endl;
    if(n->getC1() != nullptr){
        printNodes(file,n->getC1());
    }
    if(n->getC2() != nullptr){
        printNodes(file,n->getC2());
    }
    if(n->getC3() != nullptr){
        printNodes(file,n->getC3());
    }
    return;
}

static void printTree(Node * root){
 // each line is a node + childs
 // node child1 child 2 child 3 in the form of pointers (since they are unique)
 std::ofstream tree_file ("../tree.txt");
 printNodes(tree_file,root);
 
 tree_file.close();

};

}; // END namespace
#endif