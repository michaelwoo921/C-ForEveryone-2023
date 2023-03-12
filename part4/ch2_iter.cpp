#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>


using namespace std;

template <typename T>
void square(T first, T last){
    for(;first!=last;first++){
        *first = (*first)* (*first);
    }
}

int main(){
    vector<int> v = {1,3,5};
    square(v.begin(), v.end());
    vector<string> w ={"h", "e", "l", "l", "o"};
    replace(w.begin(), w.end(), "l", "o");

    for(auto w: v)
        cout << w << " ";

    cout <<"\n";
    for(auto str: w)
        cout << str;
}