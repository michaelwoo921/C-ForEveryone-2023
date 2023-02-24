#include<iostream>
#include <cmath>

using namespace std;


class duo {
   public:  
       duo():first(0.0), second(0.0){}
       duo(double x, double y=0.0): first(x), second(y){}
       double get_first(){return first; }
       double get_second(){return second;}
       void set_first(double d){ first =d;}
       void set_second(double d){second =d; }
   
   protected:
     double first, second;

};

class point:public duo {
  public:
    point(): duo(){}
    point(double x, double y=0.0): duo(x,y){}
    virtual double length(){
      return sqrt(first*first + second*second);
    }

};

class point3d final:public point {
  public:
    point3d(): point(), z(0.0){}
    double length(){
      return sqrt(first*first + second*second + z*z);
     }
     void set_z(double d){z=d;}
     
  private:
    double z;

};

int main(){
  point p;
  p.set_first(3.0); p.set_second(4.0);
  cout << "point p "  << p.get_first() << ", " << p.get_second()<<endl;
  cout << "length: " << p.length()<< endl;
  
  point3d t;
  t.set_first(5.0); t.set_second(6.0); t.set_z(7.0);
  cout << "point3d t " << t.length() << endl;
  
  point* ptr_p;
  ptr_p = &t;
  cout << "pointer to t: " << ptr_p -> length() << endl;
}
