#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <ctime>


using namespace std;

double prob() {
  return  (double) rand() / RAND_MAX;

}


int main(){
 int size;
 double density;
 bool** graph;
 int** color;
 double** cost;
 
 cout << "size of graph? \n";
 cin >> size;
 cout << "choose density (0,1) \n";
 cin >> density;
 
 srand(time(0));
 
  
 graph = new bool*[size];
 cost = new double*[size];
 color = new int*[size];
 for(int i=0; i<size;i++){
   graph[i] = new bool[size];
   cost[i] = new double[size];
   color[i] = new int[size];
 
 }

for(int i=0;i<size;i++)
  for(int j=i;j<size;j++){
    if(i==j)
       graph[i][j] = false;
    else{
       graph[i][j] =graph[j][i] = (prob() < density);
    }
  }



for(int i=0;i<size;i++)
  for(int j=i;j<size;j++){
    if(graph[i][j]){
      cost[i][j] = cost[j][i] = prob()*30;
      color[i][j] = color[j][i] = rand() % 3;
    }
  }


ofstream outp("ch7-out.txt");
outp << size << "\n";

for(int i=0;i<size;i++)
  for(int j=0;j<size; j++)
    {
     if(graph[i][j]) {
       outp << i << "\t" << j << "\t"<<
          cost[i][j] << "\t" << color[i][j] << "\t" << endl;
     }
    }
    
    
}
