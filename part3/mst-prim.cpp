#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
using namespace std;

class graph {

  public:
    graph(int n=7): N(n) {
      adj = new int*[N];
      for(int i=0;i<N;i++)
        adj[i] = new int[N];
    }
  
   // test case with N=7 vertices;
   void test(){
      // N=7
      for(int i=0;i<7;i++)
        for(int j=0;j<7;j++)
          adj[i][j]=-1;
      adj[0][1] = adj[1][0] =2; adj[0][2] =adj[2][0] = 3;
      adj[1][2] =adj[2][1] = 2; adj[1][3] =adj[3][1] =4; adj[1][4] =adj[4][1] = 4;
      adj[2][3] =adj[3][2] =5; adj[3][5]=adj[5][3] = 4; 
      adj[4][5] = adj[5][4] =5; adj[4][6]=adj[6][4] =3;
      adj[5][6]=adj[6][5] = 5;
   }
   
   void prim();
   void print();
   void setEdge(int i, int j, int val){
     adj[i][j] = val;
   }
 
 
   private:
     int N; // size of graph
     int** adj;
     vector<int> mst;
     int mincost;
   
};

int main(){

cout << "test case with N=7" <<endl;
  graph gtest;
  gtest.test();
  gtest.prim();
  gtest.print();
  
  cout << "\n read data from text file\n";
  int num_vertices;
  ifstream data_file("mst-prim-input.txt");
  istream_iterator<int> start(data_file), end;
  vector<int> data(start, end);
  num_vertices =data[0];
  cout << num_vertices << " vertices --------------------"<<endl;
 cout << data.size() /3 << " edges " << endl;
  graph gr(num_vertices);
  int edge_cnt =0;
  
  for(int i=0;i<num_vertices; i++)
    for(int j=0;j< num_vertices;j++)
      gr.setEdge(i,j, -1);
  for(auto it=data.begin() +1; it != data.end();){
    int s =*it; it++; int d =*it; it++; int val = *it; it++;
    gr.setEdge(s,d, val);
  }
  
  gr.prim();
  gr.print();
  
    return 0;
}

void graph::prim(){
 // initialize cSet 0 
 bool cSet[N], oSet[N];

 int c_cnt,ix, jx,temp, total_cost =0;
 for(int i=0;i<N;i++){
   cSet[i] = oSet[i] = false;
 }
 cSet[0] = true; c_cnt=1; ix=0; 
 
 
 while(c_cnt <N){
  // update oSet and mincost edge (ix, jx) total cost
  temp =-1;
  for(int i=0;i<N;i++){
    if(cSet[i]){
      for(int j=0;j<N;j++){
        if(adj[i][j] >0 && !cSet[j] ){
          if(temp<0){
           temp = adj[i][j]; ix =i; jx=j;
          }
          if(adj[i][j] < temp){
            temp = adj[i][j]; ix =i; jx=j;
          }
           
        }
      }
    }
  }
  // (ix, jx) mincost
  mst.push_back(ix);
  mst.push_back(jx);
  mst.push_back(temp);
  total_cost += temp;
  cSet[jx] =true; c_cnt++; mincost = total_cost;
  
 }


}

void graph::print(){
cout<< "edges with minimum costs: (i,j, cost) "<< endl;
 for(int i=0; i< mst.size();i=i+3){
   if(i%15 ==0) cout<< endl;
   cout << "(" << mst[i] << ", " << mst[i+1] << ", " << mst[i+2] << ")\t";
 }
 
 cout << "\ntotal cost: " << mincost;
 
}


