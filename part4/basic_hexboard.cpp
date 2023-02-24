#include <iostream>
#include <string>

using namespace std;

int main(){


string n ="O";
string hedge="_";
string hblank = "";
string vblank = "";
string ledge = "/";
string redge ="\\";

for(int i=0;i<7;i++){
 
 cout << hblank;
  for(int j=0;j<7;j++){
  if(j==6)
    cout << n;
  else
   cout << n + hedge;
   
  }
    cout << endl;
  cout << vblank;
  if(i !=6){
      for(int j=0;j<7;j++){
	   if(j==6)
	     cout << redge;
	   else
	     cout << redge + ledge;
	  }
	  cout << endl;
	  vblank +=" ";
	  hblank += " ";
  }
  
  }





return 0;

}
