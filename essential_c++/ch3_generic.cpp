#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <map>
#include <string>
#include <set>

using namespace std;

template <typename IT, typename T>
IT find(IT first, IT last, const T& value){
    for(; first != last; first++){
        if(*first == value){
            return first;
        }
    }
    return last;


}

bool less_than(int v1, int v2){
    return v1 < v2;
}

vector<int> filter (vector<int>& vec, int val, bool (*pred)(int, int)){
    vector<int> nvec;
    for(int k=0;k<vec.size();k++)
        if(pred(vec[k], val))
            nvec.push_back(vec[k]);
    return nvec;

}

int main(){
    
    int ia[7] ={8, 34, 2, 56, 7, 4, 8};
    vector<int> vec(ia, ia+7);
    vector<int> nvec;
    nvec = filter(vec, 10, less_than);
    for(auto w: nvec){
        cout << w << "  ";
    }

    set<int> iset(vec.begin(), vec.end());
    for(auto w: iset)
        cout << w<< "  ";
    cout << endl;

  
   
    map<string, int> words;
    string tword;
    words["any"]=1;
    words["more"] =2;
    words["news"] =4;
    map<string, int>::iterator it = words.begin();
    while(it != words.end()){
        cout << it ->first << ": " << it ->second <<endl;
        it++;
    }


    
}