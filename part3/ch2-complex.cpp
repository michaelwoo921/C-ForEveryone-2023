#include <iostream>
#include <complex>


using namespace std;

template <class T>
inline void swap(T& p, T& q){
	T temp =p;
	p =q;
	q = temp;
}


int main(){
	complex<double> r(3.2, 2.5), s(1.2, 2);
	
	
	cout << r << s;
	cout << "after swap: "<<endl;
	swap(r,s);
	cout << r << s;

}
