#include <iostream>
#include "Node.h"

using namespace std;

int MinBW(Node * n,int s_i,int e_i){ // find minimum bandwidth on the interval [s_i,e_i)
    
    if(!(n->getStart() <= s_i) || !(n->getEnd() >= e_i)){
        return 0;
    }
    
    return 0;

};