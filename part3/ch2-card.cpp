#include <iostream>
#include <cstdlib>
#include <ctime>


// rand, time library

using namespace std;

enum suit {
    clubs, diamonds, hearts, spades
};

struct card {
    int pips;
    suit s;
};

void pr_card(card cd){
    switch(cd.pips){
        case 1: cout << "A"; break;
        case 11: cout << "J"; break;
        case 12: cout << "Q"; break;
        case 13: cout << "K"; break;
        default: cout << cd.pips;  
    }

    switch(cd.s){
        case clubs: cout << "C"; break;
        case diamonds: cout << "D"; break;
        case hearts: cout << "H"; break;
        case spades: cout << "S"; break;
        default: cerr << "suit error\n"; exit(1);
    }
    cout << " ";
}

void init_deck(card d[]){
    for(int i=0;i<52;i++){
        switch(i / 13){
            case 0: d[i].s = clubs; break;
            case 1: d[i].s = diamonds; break;
            case 2: d[i].s = hearts; break;
            case 3: d[i].s = spades; break;      
        }
        d[i].pips = 1 + i % 13;
    }
}

void shuffle(card d[]){
    for(int i=0;i<52;i++){
        int k = rand() % 52;
        card t = d[i];
        d[i] = d[k];
        d[k] = t;
    }
}

void pr_deck(const card d[]){
    for(int i=0;i<52;i++){
        if(i % 13 == 0)
            cout << "\n";
        pr_card(d[i]);
    }
    cout << endl;
}

int main(){
    card deck[52];
    srand(time(NULL));
    init_deck(deck);
    pr_deck(deck);
    shuffle(deck);
    pr_deck(deck);

    return 0;
}
