#include <iostream>
#include <string>
#include <complex>
#include <vector>

using namespace std;

void display(const vector<int> &vec){
    cout << " display vec\n";
    for(int k=0;k<vec.size();k++)
        cout << vec[k] << " ";
    cout << endl;
}

void swap(int &v, int &w){
    int temp =v;
    v = w;
    w = temp;
    
}

void bubble_sort(vector<int> &vec){
    for(int k=0; k<vec.size();k++){
        for(int l=k; l< vec.size(); l++){
            if(vec[k] > vec[l]){
                swap(vec[k], vec[l]);
            }
        }
    }
}

void display(string* str){
    cout << *str;
}


int main(){
    int arr[8] = {8, 34, 3, 13, 1, 21, 5, 2};
    vector<int> vec(arr, arr + 8);
    cout<< "before sort: \n";
    display(vec);
    bubble_sort(vec);
    cout<< "after sort: \n";
    display(vec);

    string msg ="I have a good time";
    string *p = &msg;
    display(p);


}



