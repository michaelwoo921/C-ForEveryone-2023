#include <iostream>


using namespace std;

class point {
   
   public: point(double x=0.0, double y=0.0): x(x), y(y) {};
       friend point operator+(const point& p, const point& q){
         point sum = {p.x + q.x, p.y + q.y};
         return sum;
        };
        
        friend ostream& operator<< (ostream& out, const point& p){
          out << "( " << p.x << ", " << p.y << ") ";
          return out;
        };
        
   


   private: double x, y;
};

template <class T>
void swap_two(T& p, T& q){
	T temp =p;
	p =q;
	q = temp;
}


int main(){
  point p(3.0), q(1.0, 2.5);
  cout << p << q << "the sum: " << p+q << endl;
  swap_two(p,q);
  cout << "after swap --- \n";
  cout << p << q;
  
}
