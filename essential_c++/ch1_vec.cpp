#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(){
    string user_name;
   
    cout << "what is your first name?: ";
    cin >> user_name;
    vector<int> fibonacci, lucas, pell, triangular, square, pentagonal;
   

    const int seq_cnt =6;
    vector<int> *seq_addrs [ seq_cnt] = {&fibonacci, &lucas, &pell, &triangular, &square, &pentagonal};
    vector <int> *current_vec =0;
    srand(seq_cnt);
    int seq_index = rand() % seq_cnt;
    current_vec = seq_addrs[seq_index];
    if(current_vec && ! current_vec -> empty() && (*current_vec)[1] ==1){

    }

    ifstream infile("ch1_vec.txt");

    int num_tries = 0, num_cor =0;

    if(! infile){
        // open failed
    }
    else{
        string name;
        int nt, nc;
        while(cin >> name){
            infile >> nt >> nc;
            if(name == user_name){
                // match

                num_tries = nt;
                num_cor = nc;
            }
        }
    }

    ofstream outfile("ch1_vec.txt", ios_base::app);
    if(! outfile){
        cerr << " unable to open data\n";
    }else{
        outfile << user_name << ' ' << num_tries << ' ' << num_cor << endl;
    }


    


}