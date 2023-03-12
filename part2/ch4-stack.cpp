#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;

const int max_len =1000;

class stack {
    private: 
        char s[max_len];
        int top;
        enum {EMPTY=-1, FULL=max_len -1};

    public:
    void reset(){ top = EMPTY;}

    void push(char c){s[++top]=c; }

    char pop(){return s[top--];}

    int top_of(){return s[top]; }

    bool empty(){return top == EMPTY;}

    bool full(){return top == FULL; }

};

int main(){

    char str[40]="Hello I am Bob";
    stack s;
    int i=0;

    cout << str << endl;
    s.reset();
    while(str[i]){
        if(!s.full())
            s.push(str[i]);
        i++;
    }
    while(!s.empty())
        cout << s.pop();
    cout <<"\n";
    

    return 0;
}