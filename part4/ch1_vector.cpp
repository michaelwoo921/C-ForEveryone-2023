#include <iostream>
#include <vector>
#include <iterator>

using namespace std;


template <typename T>
void square(T first, T last){
  for(; first != last; first++){
    *first = (*first) * (*first);
  }
}

int main(){

vector<int> v;
for(int i=1;i<10;i++)
  v.push_back(i);
  

for(auto i: v)
  cout << i << "\t";
cout << endl;

square(v.begin(), v.end());

cout << "after squaring it " << endl;
for(auto it = v.begin(); it!= v.end(); ++it)
  cout << *it << "\t";
cout << endl;



}
