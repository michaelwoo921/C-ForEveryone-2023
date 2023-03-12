#include<iostream>
#include<vector>


using namespace std;

int main(){
    int size =10;
    vector<int> vec ={1,3,5,7,9,11};
    for(auto it=vec.begin(); it<vec.end()-1;it++){
        cout << *(it +1)<< " ";
    }
}

