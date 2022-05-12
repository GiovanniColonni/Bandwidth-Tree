#ifndef TEST
#define  TEST
#include <iostream>
#include <chrono>
#include "BandWidthTreeMethods.cpp"
#include "Tree.h"
#include <math.h>

using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

namespace test{

    inline double getTime(){
       return duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    };
    
    inline void allocateNewBandiwdthTreeRandom(int n_iteration){
        Tree root(100,0,100);
        root.setAMB(100);

        int rand_b = 0;
        int rand_s = 0;
        int rand_e = 0;
        
        Tree * new_root = &root;

        for (auto i = 0; i < n_iteration ; i++){
            rand_b = (rand() % 30) + 1;

            rand_s = (rand() % 100) + 1;

            rand_e = (rand() % (100 - rand_s + 1)) + rand_s;
            cout << rand_b << " , " << rand_s << " , " << rand_e << endl;
            new_root = methods::AllocateBW(new_root,rand_b,rand_s,rand_e);
            
        }

        methods::printTree(new_root);
        
    }

    inline void testMinBW(int n_iteration){

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
        A.setAMB(methods::amb(&A));
        B.setAMB(methods::amb(&B));
        C.setAMB(methods::amb(&C));
        D.setAMB(methods::amb(&D));
        E.setAMB(methods::amb(&E));
        F.setAMB(methods::amb(&F));
        G.setAMB(methods::amb(&G));
        H.setAMB(methods::amb(&H));


    int rand_s = 0;
    int rand_e = 0;
    const int max = 60;
    for (auto i = 0; i < n_iteration ; i++){
        
        rand_s = (rand() % max) + 1;
        rand_e = (rand() % (max - rand_s + 1)) + rand_s;
            

        auto res = methods::MinBW(&A,rand_s,rand_e);
        cout << "MinBW : " << res << " in [" << rand_s << "," << rand_e << "]" << endl;


    }

    }
};
#endif