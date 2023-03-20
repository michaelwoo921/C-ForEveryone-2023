

// C++ io
// one line comments
// harp define macro
// inline function
// change array to vector<>

#include <iostream>
#include<vector>

using namespace std;

template <typename T>
void sum(T& result , int n, vector<T> d);



int main(){
  const int N = 40;

  int accum =0;
  vector<int> data(N);

  for(int i=0;i<N;i++){
    data[i]= i;
  }

  sum(accum, N, data);

 
  cout << "sum is " << accum << endl;

  return 0;

}

template <typename T>
void sum(T& result, int n, vector<T> d){
    for(int i=0;i<n; i++)
      result += d[i];
}
