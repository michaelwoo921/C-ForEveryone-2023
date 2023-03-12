#include <iostream>
#include <algorithm>
#include <string>


using namespace std;

void incr(int& i){
  static int n=0; i=n++;
}

void outvec(int i){
  cout << i;
}

int main(){

  string words[5] ={"my", "hop", "mop", "hope", "cope"};
 
  string* where = find(words, words +5, "hop");
  
  cout << *(++where) << endl;
  
  sort(words, words +5);
  where = find(words, words + 5, "hop");
  
  cout << *(++where) << endl;
  
  
  vector<int> v(6);
 
 for_each(v.begin(), v.end(), [](int &i){static int n=1;  i=n++;});
 for_each(v.begin(), v.end(), [](int i) -> void { cout << i << endl;});

 int i=0;
 incr(i);
 outvec(i);
  incr(i);
 outvec(i);

 for(auto w: v){
  cout << w << "\n";
 }

}
