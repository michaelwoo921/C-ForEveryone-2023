#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

double prob(){
  return static_cast<double> (rand())/RAND_MAX;
}

ofstream outp("dijkstra-out.txt"); 

class graph{
  public:
     graph(int size): N(size) {
  
       adj = new double*[N];
       for(int i=0;i<N;i++)
          adj[i] = new double[N]; 
       path = new int[N];
       mincost = new double[N];
     }
     void print_edges();
     void print_path();
     void dijkstra();
     void test();
     void setRandomEdges(double density=0.1, double min=1.0, double max =10.0);


   
   private:
     double** adj;
     int N;
     double *mincost;
     int *path;
     double d;
     double _min, _max;

 };
 
 
 
 int main(){
 
 outp << "test case with N=9"<<endl;
 graph g1(9);
 g1.test();
 g1.print_edges();
 g1.dijkstra();
 g1.print_path();
 
 
 cout << "This program calculates shortest path from origin to any destination\n\n\n";
  
  graph g(20);
  g.setRandomEdges(0.18, 1.0, 10.0);
  g.print_edges();
  g.dijkstra();
  g.print_path();


  return 0;
  
 }
 
 void graph::dijkstra(){
  outp << "running dijkstra algorithm" << endl;
  
  // initialize cSet and oSet;
  bool cSet[N], oSet[N];
  double cost[N];
  int c_cnt =0, o_cnt =0;
  int ix;
  double temp;
  
  int cnt=0, ocnt=0;
  for(int i=0;i<N;i++){
    cSet[i] =oSet[i] = false; cost[i] =-1.0;
  }
  cSet[0]=true; c_cnt=1; mincost[0]= 0; path[0] =0 ; 
  for(int i=0; i<N;i++){
    if(adj[0][i]>0){
       oSet[i] = true;
       o_cnt++;
       cost[i] = adj[0][i];
       path[i]=0;
    }	  
  }
  
  // while loop until c_cnt ==N or o_cnt == 0
  while(c_cnt <N){
    // pick element in Oset with smallest cost;
    temp =-1;
    for(int i=0;i<N;i++){
      if(temp<0 && oSet[i]){
        temp = cost[i]; ix=i;     
      }
      if(oSet[i] && cost[i] < temp){
        temp = cost[i]; ix =i;
      }  
    }
    // temp>0 means min cost element ix picked temp<0 means oSet is empty

    if(o_cnt>0){
        // update ix to cSet remove from oSet
         // cout << "min cost element is " << ix << " o_cnt is: " << o_cnt << endl;
        cSet[ix]= true; c_cnt++; mincost[ix] =cost[ix]; 
        oSet[ix] = false; o_cnt--; 
        // update oSet to include sites reachable from ix
        for(int j=0;j<N;j++){
          if(!cSet[j] && !oSet[j] && adj[ix][j]>0){
            oSet[j] = true; o_cnt++; cost[j] = mincost[ix] + adj[ix][j]; path[j] = ix;
          }
        }
        
    
    }
    
   // c_cnt <N and o_cnt ==0 means disconnected and exit 
   if(c_cnt <N && o_cnt ==0){
     outp << "some elements are not reachable. exiting the program\n";
     break;
   }
   
   
  
  }
  
  
    
  
 }
 
 
 void graph::test(){
   // test case N =9 case
   for(int i=0; i<9;i++)
     for(int j=0; j<0;j++)
       adj[i][j]= -1.0;
       
   adj[0][1] =adj[1][0] =4;  adj[0][2] =adj[2][0] = 3; adj[0][4] =adj[4][0] =7;
   adj[1][3]=adj[3][1] = 1;  adj[2][4] =adj[4][2] = 4;
   adj[3][4]= adj[4][3] = 3; adj[3][5]= adj[5][3] = 1;
   adj[4][5] =adj[5][4] = 1; adj[4][6] = adj[6][4] = 5; adj[4][8]=adj[8][4]= 3;
   adj[5][7]= adj[7][5] = 2; adj[5][8] = adj[8][5] = 4;
   adj[6][8]= adj[8][6] = 5;  adj[7][8] =adj[8][7] =3;
   
 }
 
 void graph::print_path(){
  for(int i=0;i<N;i++){
   int p=i, q = path[i];
   while(q !=p){
     outp << p << " <-- ";
     p=path[p]; q = path[q];
   }
   outp << p << "\tmincost: " << mincost[i] << endl;
   
  }
 }
 
 void graph::print_edges(){
   int k=N;
   // cout << "printing at most 10 by 10 adj matrices\n";
   if(N>10)
     //k =10;
   for(int i=0;i<k;i++){
     for(int j=i;j<k;j++){
     if(adj[i][j]>0)
       outp << "( " << i << ", " << j << ", " << adj[i][j] << ",)  ";
     }
     outp << endl;
   }
 }
 
 
 void graph::setRandomEdges(double density, double min, double max){
 d= density; _min = min; _max = max;
   srand(time(0));
   for(int i=0;i<N;i++)
     for(int j=i;j<N; j++){
       if(i==j){
         adj[i][j] = -1.0;
       } else {
       double p =prob();
       if(p<d){
       	adj[i][j] = adj[j][i] = _min + (_max - _min) * p ;
       } else{
         adj[i][j] =adj[j][i] = -1.0;
       }
         
       }
     }
 }
