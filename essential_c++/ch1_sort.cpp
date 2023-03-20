#include<iostream>
#include <fstream>
#include <vector>
#include <algorithm>
// #include <string>

using namespace std;

int main(){
    vector <string> words;

    string data;
    int cnt=0;

    ifstream infile("ch1_sort.txt");
    
    while(infile >> data){
        words.push_back(data);
    }

    sort(words.begin(), words.end());

    for(auto w: words){
        cout << w << " ";
    }
}