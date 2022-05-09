#include <iostream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <map>
#include <string>


#include "./classes/Edge.h"
#include "./classes/Graph.h"
#include "./classes/Tree.h"

#include "./classes/BandWidthTreeMethods.cpp"

using namespace std;
    // example from the paper 
    // A = root (5K,[0,60])
  // B = (10K,[0,30])
// D = (5K,[0,10])
 // E = (0K,[10,20])
   // F = (10K,[20,30])
        // C = (0K,[30,60]) 
    // G = (0K,[30,50])
        // H = (20K,[50,60])


int amb(Tree *v){ // sum of band from v to root node
    // SBAGLIATO NON DEVO TORNARE AL ROOT MA AL NODO IN CUI INIZIALMENTE è STATA CHIAMATA LA FUNZIONE
    int amb_;
    int amb_i;

    
    if(v->getP() == nullptr){
        return v->getBand();    
    };
    
    amb_i = amb(v->getP());

    amb_ = v->getBand() + amb_i;
    return amb_;
};


/*
ToDo:
1. togliere metodi non utilizzati
2. mettere costanti
3. riordinare dipendenze
4. togliere le implementazioni da .h su tutti
5. cambiare nome da node.h a tree_node.h o simile
6. usare make/cmake
7. togliere static dalle funzioni sennò è spreco
8. davvero anche int come puntatore ?
*/


void printTrees(std::ofstream &file,Tree * n){
    
    file << "(" << n << ";" << n->getStart() << ";" << n->getEnd() << ";"<< n->getAMB() <<")," << n->getC1() << "," << n->getC2() << "," << n->getC3() << std::endl;
    std::cout << "here ??" << std::endl;
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

void printTree(Tree * root){
 // each line is a node + childs
 // node child1 child 2 child 3 in the form of pointers (since they are unique)
 std::ofstream tree_file ("tree.txt");
 printTrees(tree_file,root);
 
 tree_file.close();

};

int main(int argc, char * argv[]){
    
    if(argc < 4){
        cout << "Insert [w,start,end]...exit" << endl;
        exit(1);
    }

    
    int w = atoi(argv[1]);
    int c = atoi(argv[2]);
    int d = atoi(argv[3]);

    
    Tree A(5,0,60);
    Tree B(10,0,30);
    Tree C(0,30,60);
    Tree D(5,0,10);
    Tree E(0,10,20);
    Tree F(10,20,30);
    Tree G(0,30,50);
    Tree H(20,50,60);

    A.setC1(&B);
    A.setC3(&C);

    B.setP(&A);
    C.setP(&A);

    B.setC1(&D);
    B.setC2(&E);
    B.setC3(&F);

    D.setP(&B);
    E.setP(&B);
    F.setP(&B);

    C.setC1(&G);
    C.setC3(&H);

    G.setP(&C);
    H.setP(&C);

    // set aggreagate minimum bandwidth on the tree nodes
    A.setAMB(amb(&A));
    B.setAMB(amb(&B));
    C.setAMB(amb(&C));
    D.setAMB(amb(&D));
    E.setAMB(amb(&E));
    F.setAMB(amb(&F));
    G.setAMB(amb(&G));
    H.setAMB(amb(&H));
    
    //Tree * a = AllocateBW(&A,w,c,d);

    printTree(&A);
    
    
    //Graph * e = methods::createGraph();

    //e->Feasibility(0,10,9,1,5);

    exit(0);

    
    
    /*
    for(auto &e:*a){
        cout << e << endl;
    }
    */

    //printTree(&A);
    /*
    Graph * g = createGraph(); // PROVARE A PRINTARE MAPPA !!!!s
    map<int *,vector<Edge*>*> * g_map = g->getGraph();
    for( auto & [v,e] : *g_map){
        cout << e->size() << endl;
    };
    exit(0);
    */
};