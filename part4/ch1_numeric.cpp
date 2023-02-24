#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>


using namespace std;

class gen {
   public:
     gen(double x_zero, double increment): x(x_zero), incr(increment){}
     double operator()(){ x += incr; return x*x; }
    private: 
      double x, incr;
};

double integrate(gen g, int n){
  vector<double> fx(n);
  generate(fx.begin(), fx.end(), g);
  return (accumulate(fx.begin(), fx.end(), 0.0)/n );

}

template <class ForwIter>
void print(ForwIter first, ForwIter last, const char* title){
  cout << title << endl;
  while(first!= last)
    cout << *first++ << "\t";
  cout << endl;
}

int main(){
  double v1[3] ={1.2, 1.5, 1.8};
  double v2[3] ={2.1, 2.5, 2.8};
  
  double sum = accumulate(v1, v1 +3, 0.0, minus<int>());
  double inner_pr = inner_product(v1, v1+3, v2, 0.0);
  
  cout << sum << endl;
  cout << inner_pr << endl;
  
  const int n =10000;
  gen g(0.0, 1.0/n);
  
cout << "integrate x*x "<< integrate(g,n) <<endl;


int data[3]= {9,10,11};
print(data, data+3, "original values");
transform(data, data+3, data, bind2nd(multiplies<int>(), 2));
print(data, data+3, "new values");



}
