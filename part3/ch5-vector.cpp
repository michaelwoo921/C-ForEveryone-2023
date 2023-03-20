
#include<iostream>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;

int main(){

  ifstream data_file("ch5-data.txt");
  istream_iterator<int> start(data_file), end;
  vector<int> data(start, end);
  
  for(vector<int>::iterator p=data.begin(); p!= data.end(); p++)
    cout << *p << "\t";
  cout << endl;

  
  int sum =0;
  for(auto it = start; it != end;++it){
  cout << *it<< endl;
    sum += *it;
  }
  cout << endl;
  cout << "sum" << sum << ", \t average" << (double) sum / data.size() <<endl;
  
  sum =0;
  for(auto value: data){
   sum += value;
  }
  
  cout <<"for range average" << sum / data.size() ;
}
