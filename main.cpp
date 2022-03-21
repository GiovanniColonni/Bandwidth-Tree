#include <iostream>
#include "./classes/Node.h"
#include <vector>
#include <cstdlib>
#include <limits>

//#include "./classes/BandWidthTreeMethods.cpp"

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


int amb(Node *v){ // sum of band from v to root node
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


int MinBW(Node * n,int s_i,int e_i){ // find minimum bandwidth on the interval [s_i,e_i)
    
    if(s_i > e_i){
        cout << "s_i > e_i" << endl;
        return 0;
    }

    //it++;
    
    if(!(n->getStart() <= s_i) || !(n->getEnd() >= e_i)){ // GUARDARE DELL INTERVALLO NON COMPLETO [)
        //cout << "Not contained" << endl;
        return 0;
    };


    int minBW = numeric_limits<int>::max(); // not really infinite but 2^31-1
    bool is_leaf = (n->getC1() == nullptr) && (n->getC2() == nullptr) && (n->getC3() == nullptr); // getChild().size()
    
    if(((n->getStart() == s_i) && (n->getEnd() == e_i)) || is_leaf){ //  [)
        //cout << "iteration " << it << " here leaf or complete, value : " << amb(n) <<  endl;
        return amb(n);
    };
    
    vector<Node *> v = {n->getC1(),n->getC2(),n->getC3()};
    //cout << "iteration " << it << " looking into childs : " << endl;

    for(auto & child : v)
    {
        if(child != nullptr){
        

        bool contained = (s_i <= child->getStart() && child->getStart() < e_i) ||
                         ((child->getStart() <= s_i && s_i <=  child->getEnd()) && (child->getStart() <= e_i && e_i <=  child->getEnd()) )
                         || (s_i < child->getEnd() && child->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
        
        if(contained){
            int s = max(child->getStart(),s_i);
            int e = min(child->getEnd(),e_i);

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

int main(int argc, char * argv[]){
    
    if(argc < 4){
        cout << "Insert [Node,start,end]...exit" << endl;
        exit(1);
    }

    
    char * node = argv[1];
    int s = atoi(argv[2]);
    int e = atoi(argv[3]);


    // TODO: matching char with the node


    Node A(5,0,60);
    Node B(10,0,30);
    Node C(0,30,60);
    Node D(5,0,10);
    Node E(0,10,20);
    Node F(10,20,30);
    Node G(0,30,50);
    Node H(20,50,60);

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

    
    

    for (auto i = 1; i < 60; i = i + 1){
        for (auto j = i + 5; j < 65; j =  j + 5){
            cout << "t(" << i << "," << j << ") - > " << MinBW(&D,i,j) << endl;
        }        
    }
    
    

    return 0;
};