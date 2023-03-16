#include<iostream>
#include <cassert>
#include<vector>
#include<algorithm>


using namespace std;

int main(){
    int size =10;
    vector<int> common_carriers, temp_carriers;


    // assert(common_carriers.size()>0);
    sort(common_carriers.begin(), common_carriers.end());
        
        for(auto it=common_carriers.begin(); it<common_carriers.end() -1; it++){
            // int prev = *it, curr = *(it+1);
            // if(prev != curr){
            //     temp_carriers.push_back(prev);
            // }
            
            // if(it == common_carriers.end() -2){
            //     temp_carriers.push_back(curr);
            // }
        
        }
    for(auto w: temp_carriers)
        cout << w << " ";
}

