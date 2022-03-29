#ifndef NODE_H
#define NODE_H

class Node{

private:
    int band;
    int start;
    int end;
    int amb;

    Node *p = nullptr;
    Node *c1 = nullptr;
    Node *c2 = nullptr;
    Node *c3 = nullptr;
    
public:
    Node();
    Node(int band, int start, int end);
    ~Node();

    void setBand(int band);
    void setStart(int start);
    void setAMB(int amb);
    void setEnd(int end);
    void setP(Node *n);
    void setC1(Node *n);
    void setC2(Node *n);
    void setC3(Node *n);

    int getBand(){ return band; }
    int getStart(){ return start; }
    int getEnd(){ return end; }
    int getAMB(){return amb;}

    int getNChild(){
        int count = 0;

        if(c1 != nullptr){
            count++;
        }
        if(c2 != nullptr){
            count++;
        }
        if(c3 != nullptr){
            count++;
        }

        return count;
    }
    void toString();

    Node * getP(){return p;}
    Node * getC1(){return c1;}
    Node * getC2(){return c2;}
    Node * getC3(){return c3;}


};

#endif