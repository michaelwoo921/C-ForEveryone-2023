#include <iostream>
#include <cstring>

using namespace std;


const int max_len =255;

class string_new {
    public:
        void assign(const char* st){strcpy(s, st); len = strlen(st);}
        int length(){return len;}
        void print(){ cout << s <<"\nLength: " << len <<"\n";}
        friend string_new operator+ (const string_new &a, const string_new &b){
            string_new temp;
            temp.assign(a.s);
            temp.len = a.len + b.len;
            if(temp.len < max_len){
                strcat(temp.s, b.s);
            } else{
                cerr << "Max Length exceeded in concatenation\n";
            }
            
            return temp;
        }

    private:
        int len;
        char s[max_len];
};

void print(char* c){cout << c << "\nLength: " << strlen(c) << "\n";}

int main(){
    string_new one, two, both;
    char three[40] ={"My name is Michael."};

    one.assign("I am C");
    two.assign(three);
    cout << three << "\nLength: " << strlen(three)<< endl;
    one.print();
    two.print();
    print(three);
    both = one + two;
    both.print();

}