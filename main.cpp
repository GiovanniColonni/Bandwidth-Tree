#include <iostream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <map>
#include <string>


#include "./classes/Edge.h"
#include "./classes/Graph.h"
#include "./classes/Node.h"

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

void merge(Node * n,int s_i, int e_i){
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

void balance(Node * n){
    return;
}

void decrement_bandwidth(Node * n,int w,int c,int d){

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

int processLeaf(Node * v1,int w,int s_i,int e_i){
        cout << "   processing leaf" << endl;
        auto n_amb = (v1->getAMB() -w);
        auto amb = v1->getAMB();

        if(v1->getStart() == s_i && v1->getEnd() == e_i){ 
            cout << "complete leaf" << endl;
            // entire node so no extra childs needed
            // decrement bandwidth
            v1->setAMB(n_amb);
            return n_amb; 
        } 

       

        if(v1->getStart() == s_i && v1->getEnd() > e_i){
            // l
            cout << "right part" << endl;
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
            cout << "left part" << endl;
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
        cout << "3 child" << endl;
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

int split(Node * v1,Node * v2,Node * v3,int count, int w, int s_i,int e_i){ // forse devo ritornare il valore di count
    int rv1 = v1->getEnd();
    int lv1 = v1->getStart();
    
    count++;

    bool contained = (s_i <= v1->getStart() && v1->getStart() < e_i) ||
                         ((v1->getStart() <= s_i && s_i <=  v1->getEnd()) && (v1->getStart() <= e_i && e_i <=  v1->getEnd()) )
                         || (s_i < v1->getEnd() && v1->getEnd() <= e_i); // [c,d) int [l(u),r(u))
        
    if(!contained){
        cout << "it: " << count << " not contained" << endl;
        return -1;
    }
    
    bool is_leaf = v1->isLeaf();

    if(v1->getStart() <= s_i && v1->getEnd() >= e_i && !is_leaf){
        cout << "it: " << count << " contained egual" << endl;
        //v1->setAMB(v1->getAMB() - w);
        // IF THE NODE is contained by the req interval then simply decrement and return

        //v1->setAMB(v1->getAMB() - w); // ERRORE, LA BANDA MINIMA ORA é LA BANDA MINIMA TRA I FIGLI COINVOLTI
        if(v1->getStart() == s_i && v1->getEnd() == e_i){
            cout << "it: " << count << " full node" << endl;
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
            cout << "it: " << count << " entering child cycle" << endl;
            if(v != nullptr){
            auto n_s = max(s_i,v->getStart());
            auto n_e = min(e_i,v->getEnd());
            cout << "it: " << count << " n_s " << n_s << " n_e " << n_e << endl;
            
            if(n_s < n_e){ // means no interception between node interval and requested interval
                b = split(v,nullptr,nullptr,count,w,n_s,n_e);
                cout << "it: " << count << " child" << endl;
                if(minBW > b){
                    minBW = b;
                }
            }
            }else{
                cout << "it: "<< count << " nullptr child" << endl;
            }
            // SET bandwidth of this node as the new minimum among the child
            // IF this.getAMB() > new minimum
        }
        cout << "it : " << count << " b " << b << "min BW " << minBW << endl;
        if(v1->getAMB() > minBW){ // new minimum among the childs
            cout << "it : " << count << " new minimum " << minBW << endl;
            v1->setAMB(minBW);
        }
        return minBW; // counter = 
    }
     
    if(is_leaf){
        // process child
        cout << "it: " << count << " on the leaf" << endl;
        return processLeaf(v1,w,s_i,e_i);
    }
    cout << "here" << endl;
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
    cout << "entering split" << endl;
    auto a  = split(u,nullptr,nullptr,0,w,s_i,e_i);
    // CALL MERGE 
    // BALANCE THE TREE
    return u;
};

void printNodes(ofstream &file,Node * n){
    
    file << "(" << n << ";" << n->getStart() << ";" << n->getEnd() << ";"<< n->getAMB() <<")," << n->getC1() << "," << n->getC2() << "," << n->getC3() << endl;
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

Graph * createGraph(){ // read the graph from file

    // ASSUMPTIONS:
    // 1. default bandwidth 10 kbit
    // 2. initially you always have all the capacity
    // 3. range time tra 0 e 60
    Graph * g = new Graph(1,1);
    ifstream topology("topology.txt");
    string l;
    while(topology){

        vector<Edge *> * v = new vector<Edge *>();
        getline(topology,l);
        if(l != ""){
            int * n = new int(stoi(&l[0]));
            
            for(auto i = 1; i < l.size(); i++){
                if(l[i] != ' '){
                   int * e_to = new int(stoi(&l[i]));
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

int main(int argc, char * argv[]){
    
    if(argc < 4){
        cout << "Insert [w,start,end]...exit" << endl;
        exit(1);
    }

    
    int w = atoi(argv[1]);
    int c = atoi(argv[2]);
    int d = atoi(argv[3]);


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
    
    //Node * a = AllocateBW(&A,w,c,d);
    
    //printTree(&A);
    Graph * g = createGraph(); // PROVARE A PRINTARE MAPPA !!!!s
    std::map<int *,std::vector<Edge*>*> * g_map = g->getGraph();
    for( auto & [v,e] : *g_map){
        cout << e->size() << endl;
    };
    exit(0);
};