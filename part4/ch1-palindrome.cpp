#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

template <typename Bidirectional>
bool isPalindrome(Bidirectional first, Bidirectional last){

  while(true){
  
   last--;
   if(first == last)
     break;
   if(first != last)
     return false;
   first++;
   if(first == last)
     break;
  }
  return true;
}

int main(){

vector<char> str = {'i', 'a', 'm', 'i'};
bool result = isPalindrome(str.begin(), str.end());

cout << str << " is palindrome: " << result << endl;
}
