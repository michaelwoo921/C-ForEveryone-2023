#include <iostream>

using namespace std;

class list_element {
   public:
      list_element(int n=0, list_element* ptr = 0): d(n), next(ptr) {}
      int d;
      list_element* next;
};

class list {
   public:
     list(): head(0), cursor(0){};
     list(const int* arr, int n);
     list(const list& lst);
     ~list();
     void prepend(int n);
     void advance(){ cursor = cursor -> next; }
     int get_element(){ return cursor -> d; }
     void print();
     
     

   private:
    list_element* head;
    list_element* cursor;
};

int main(){

list lst;
lst.prepend(9);
lst.prepend(7);
lst.print();
list a;
for(int i=0;i<10;i++){
  a.prepend(i*i);
}
a.print();

int data[10] = {3,4,6,7,-3,-5};
list d(data, 10);
list e(data,6);

d.print();
e.print();

return 0;
}

list::list(int* arr, int n){}

list::list(const list& lst){
if(lst.head ==0){
  head = 0; cursor =0;
} else {
 // setup
 cursor = lst.head;
 list_element* h = new list_element();
 list_element* previous;
 head =h; 
 h -> d = lst.head -> d;
 previous = h;
 
 for(cursor =lst.head; cursor != 0;){
  //chain and create
  h = new list_element();
  h -> d = cursor -> d;
  previous -> next = h;
  cursor = cursor -> next;
  previous = h;
 }
 cursor = head;

}
}

list::~list(){
  for(cursor =head; cursor !=0;){
    cursor = head -> next;
    delete head;
    head = cursor;
  }
}

void list::prepend(int n){
  if(head ==0){
    head = cursor = new list_element(n, head);
  } else {
    head = new list_element(n, head);
  }
};
void list::print(){
  list_element* h = head;
  while(h!= 0) {
  cout << h -> d << " -->";
  h = h -> next;
  
  }
  cout << endl;
};
