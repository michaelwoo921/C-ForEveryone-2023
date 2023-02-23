#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iterator>

using namespace std;

int main(){
  ifstream word_file("ch6-word.txt");
  istream_iterator<string> start(word_file), end;
  vector<string> words(start, end);
  for(auto str: words){
    cout << str << "\t";
  }
  cout<< "after sorting: " <<endl;
  sort(words.begin(), words.end());
  
   for(auto str: words){
    cout << str << "\t";
  }

}
