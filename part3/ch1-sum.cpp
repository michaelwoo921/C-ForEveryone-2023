

// C++ io
// one line comments
// harp define macro
// inline function
// change array to vector<>

#include <iostream>
#include <vector>

using namespace std;

const int N = 40;

template <class T>
void sum(int& result, int n, vector<T> d);

int main()
{
   int accum = 0;
   vector<int> data(N);

   for(int i = 0; i < N; ++i)
      data[i] = i*i;

    sum(accum, N, data);

    cout << "sum id " << accum << endl;
    return 0;

}


template <class T>
void sum(int& result, int n, vector<T> d){
  result  = 0;
  for(int i = 0; i < n; ++i)
    result += d[i];
}