#include <iostream>
#include "./classes/Node.h"
#include <vector>
#include <cstdlib>
#include <limits>
#include <fstream>

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
        return n->getAMB();
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

void merge(Node * n,int t){
    return;
}

void balance(Node * n){
    return;
}


int which_child(Node * v){
    auto vp = v->getP();
     if(vp->getC1() == v){ 
        return 1; // rigth child
    }
    if(vp->getC2() == v){
        return 2; // center child
    }
    if(vp->getC3() == v){
        return 3; // right child
    }
    return 0;
}
Node * imm_right(Node *  v){
    auto p = v->getP();
    if(p->getC2()!= nullptr && p->getC2() != v){
        return p->getC2();
    }
    if(p->getC3()!= nullptr && p->getC3() != v){
        return p->getC2();
    }
    return nullptr;
}

Node * imm_left(Node *  v){
    auto p = v->getP();
    if(p->getC2()!= nullptr && p->getC2() != v){
        return p->getC2();
    }
    if(p->getC1()!= nullptr && p->getC1() != v){
        return p->getC2();
    }
    return nullptr;
}
/*
int split(Node * v1,Node * v2,Node * v3,int count, int w, int s_i,int e_i){ // forse devo ritornare il valore di count
    int rv1 = v1->getEnd();
    int lv1 = v1->getStart();
    
    bool contained = (s_i <= v1->getStart() && v1->getStart() < e_i) ||
                         ((v1->getStart() <= s_i && s_i <=  v1->getEnd()) && (v1->getStart() <= e_i && e_i <=  v1->getEnd()) )
                         || (s_i < v1->getEnd() && v1->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
    if(!contained){
        return -1;
    }
    if(v1->getStart() >= s_i && v1->getEnd() >= e_i){
        v1->setAMB(v1->getAMB() - w);
        return 1; // counter = 1 
    }
    bool is_leaf = (v1->getC1() == nullptr) && (v1->getC2() == nullptr) && (v1->getC3() == nullptr); // getChild().size()
    if(is_leaf){
        // process child
        if(v1->getStart() == s_i && v1->getEnd() == e_i){ 
            // entire node so no extra childs needed
            // decrement bandwidth
            v1->setAMB(v1->getAMB() - w);
            return 0; 
        } 
        
        auto n_amb = (v1->getAMB() -w);
        //split 2 child

        if(v1->getStart() == s_i && v1->getEnd() > e_i){
            // l
            v1->setC1(&Node(0,s_i,e_i));
            v1->getC1()->setAMB(n_amb);
            // r
            v1->setC3(&Node(0,e_i,v1->getEnd()));
            v1->getC3()->setAMB(v1->getAMB()); // same bw
            // sub root
            v1->setAMB(n_amb);
        }
        if(v1->getStart() < s_i && v1->getEnd() == e_i){
            // l
            v1->setC1(&Node(0,v1->getStart(),s_i));
            v1->getC1()->setAMB(v1->getAMB());
            // r
            v1->setC3(&Node(0,s_i,v1->getEnd()));
            v1->getC3()->setAMB(n_amb); // same bw
            // sub root
            v1->setAMB(n_amb); 
        }

        // split 3 child 
        v1->setC1(&Node(0,v1->getStart(),s_i));
        v1->getC1()->setAMB(v1->getAMB());
        v1->setC2(&Node(0,s_i,e_i));
        v1->getC2()->setAMB(n_amb);
        v1->setC3(&Node(0,e_i,v1->getEnd()));
        v1->getC3()->setAMB(v1->getAMB());

        v1->setAMB(n_amb); 

        return 0;
    }
    return 0;
}

Node * AllocateBW(Node * u,int w,int s_i,int e_i){
    int minBW;
    minBW = MinBW(u,s_i,e_i);

    if(w > 0 && s_i > e_i){
        cout << "w <= 0 or s_i > e_i" << endl;
        return nullptr;
    };

    if(minBW < w){
        cout << "Not enough bandwidth available in " << s_i << "," << e_i << endl;
        return nullptr;
    };
    split(u,nullptr,nullptr,0,w,s_i,e_i);

    return u;
};
*/
void printNodes(ofstream &file,Node * n){
    // ADD [c,d] and AMB! 
    file << n << "," << n->getC1() << "," << n->getC2() << "," << n->getC3() << endl;
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
void printTree(Node * root){
 // each line is a node + childs
 // node child1 child 2 child 3 in the form of pointers (since they are unique)
 ofstream tree_file ("tree.txt");
 printNodes(tree_file,root);
 
 tree_file.close();

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

    // set aggreagate minimum bandwidth on the tree nodes
    A.setAMB(amb(&A));
    B.setAMB(amb(&B));
    C.setAMB(amb(&C));
    D.setAMB(amb(&D));
    E.setAMB(amb(&E));
    F.setAMB(amb(&F));
    G.setAMB(amb(&G));
    H.setAMB(amb(&H));
    
    
    /*
    for (auto i = 1; i < 60; i = i + 1){
        for (auto j = i + 5; j < 65; j =  j + 5){
            cout << "t(" << i << "," << j << ") - > " << MinBW(&D,i,j) << endl;
        }        
    }
    */
    printTree(&A);
    
    return 0;
};