#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

template <typename T>
bool isPalindrome(T first, T last){

  while(true){
  
   last--;
   if(first == last)
     break;
   if(*first != *last)
     return false;
   first++;
   if(first == last)
     break;
  }
  return true;
}

int main(){

  string str = "iamai";
  bool result = isPalindrome(str.begin(), str.end());

  cout << str[0] << " is palindrome: " << result << endl;

return 0;
}
