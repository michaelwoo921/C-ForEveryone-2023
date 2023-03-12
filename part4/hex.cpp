#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class hex_graph {
    
    public:
        enum class color {EMPTY, RED, BLUE};
        hex_graph(int hsize);  // n by n hex graph
        void reset(); // reset board (initialize  the board) at the begiing of the game
        int get_hsize(){return hsize;} // get number of rows
        void random_play(); // randomly assign color RED, color BLUE for all nodes on the graph
        void draw_board(); 
        void move(int k, color c){board[k] = c; } // assign color on node k 
        bool move_is_legal(int i, int j){// move is legal only when node color is blank and inside the board
            return (i>=0 && i<hsize && j>=0 && j< hsize && board[i*hsize + j] == color::EMPTY);
        }
        bool dijkstra(string str);
        // str is either "X" (red) or "O" (blue)
        // returns true if there is red path from top to bottom
        // returns true if there is a blue path from left to right
        

        // "X" for red node, "O" for blue node, "." for empty node 
        string mark(color c){
            if(c == color::EMPTY)
                return ".";
            else if(c== color::BLUE)
                return "O";
            else 
                return "X";
        }
        bool get_game_over(){ return game_over;}
        string get_who_won(){ return who_won;}



    private:
        int hsize; // number of rows
        int size; //  number of nodes
        vector< vector<int> > edgelist; // list of nearest neighbor nodes for each node
        vector<color> board; // vector of colors with size 
        vector<int> path; // path of connected nodes path[n]=k means path from k to n
        bool game_over; // updated after dijskstra(string str) method
        string who_won; // updated after dijskstra(string str) method
     
};

// str is either "X" or "O". get input i, j from keyboard and accept only a legal move
void makeMove(hex_graph &g, string str){
    bool move_is_legal = true;
    hex_graph::color c = str == "X" ? hex_graph::color::RED : hex_graph::color::BLUE;

    while(move_is_legal){
        g.draw_board();
        int i,j, k;
        cout << str <<"'s turn. Choose two integers i j separated by space"<< endl;
        cout << "For example 0 3 represent 0 th row and 3rd column"<< endl;
        
        cin >> i;
        cin >> j;
        k= i* g.get_hsize() + j;

        // if move is legal then assign a move and break from while loop
        if(g.move_is_legal(i,j)){
            g.move(k, c);
            move_is_legal = false;
        }else{
            cout << i << ", " << j << " are illegal move" << endl;
        }
    
    }

}

int main(){
    int hsize;
    cout << "starting the hex game\n";
    cout << " Choose the numbe of rows for the game: ";
    cin >> hsize;

    int size = hsize*hsize;
    hex_graph g(hsize);
    g.reset();
    // g.random_play();
    g.draw_board();
    string str; // str  is "X" or "O"

    for(int turn=0;turn<size; turn++){
        if(turn % 2==0) // X player's turn 
            str = "X";
        else
            str = "O";

        makeMove(g, str);
        g.dijkstra(str);
        if(g.get_game_over()){
            g.draw_board();
            cout << str << " won the game\n";
            break; // break from for loop
        }
    }


    return 0;
}





hex_graph::hex_graph(int hsize): hsize(hsize), size(hsize*hsize){
    edgelist.resize(size);
    board.resize(size);
    path.resize(size);
    game_over = false;
    who_won ="";

    int i,j;
    // edge_list[k] contains nearest neighbor nodes: 6 nodes for internal, 2 or 3 nodes on the corner, 4 nodes on each side 
    for(int k=0; k<size; k++){
        i = k / hsize; j = k % hsize;  // row i, column j, node number k

        // for the first column
        if(j == 0 && k !=0 && k != size - hsize){
            edgelist[k].push_back(k+1); edgelist[k].push_back(k - hsize);
            edgelist[k].push_back(k - hsize+ 1);    edgelist[k].push_back(k + hsize);
        }

        if(k == 0){ // top left corner
            edgelist[k].push_back(1); edgelist[k].push_back(hsize);
        }
        if(k == size - hsize){ // bottom left corner
            edgelist[k].push_back(k+1); 
            edgelist[k].push_back(k - hsize);   edgelist[k].push_back(k - hsize + 1);
        }

        // for the last column
        if(j == hsize -1 && k != hsize -1 && k != size -1){
            edgelist[k].push_back(k-1); edgelist[k].push_back(k - hsize);
            edgelist[k].push_back(k + hsize -1); edgelist[k].push_back(k + hsize);
        }  
        if(k == hsize -1){  // top right corner
            edgelist[k].push_back(k-1); edgelist[k].push_back(k + hsize);   
            edgelist[k].push_back(k + hsize -1);
        }
        if(k == size -1){ // botom right corner
            edgelist[k].push_back(size -2); edgelist[k].push_back(size -1 - hsize);
        }
        // for the first row
        if(k>0 && k< hsize-1){
            edgelist[k].push_back(k-1); edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + hsize); edgelist[k].push_back(k + hsize -1);
        }

        // for the last row
        if(k > size - hsize && k < size-1){
            edgelist[k].push_back(k-1); edgelist[k].push_back(k+1);
            edgelist[k].push_back(k - hsize);   edgelist[k].push_back(k - hsize + 1);
        }

        // 6 nearbor nodes for internal nodes
        if(i!=0 && i != hsize -1 && j!=0 && j!= hsize -1){
            edgelist[k].push_back(k-hsize); edgelist[k].push_back(k-hsize +1);
            edgelist[k].push_back(k-1); edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + hsize);   edgelist[k].push_back(k + hsize -1);
        }
    }


} 

void hex_graph::reset(){
    for(int k=0; k<size;k++)
        board[k] = color::EMPTY;
}

void hex_graph::random_play(){
    for(int k=0; k<size;k++)
        if(k < (size + 1)/2)
            board[k] = color::BLUE;
        else
            board[k] = color::RED;
    random_shuffle(board.begin(), board.end());
}

void hex_graph::draw_board(){
    string hedge=" _ ";
    string hblank = "";
    string vblank = "   ";
    string ledge = "/ ";
    string redge ="\\ ";


    cout << hsize << " by " << hsize << " hex board"<< endl;
    cout << "O represents the move by BLUE player and X represents the move by RED player" 
        << endl;

    // label of column node
    cout << "  ";
    for(int j=0;j<hsize;j++) // j column number row =col
        cout << (j % 10) << "   ";
    cout << endl;

    for(int i=0;i<hsize;i++){

        cout << hblank;
        // label of each row
        cout << (i% 10) << " ";
        // horizontal edges row: node followed by edge except the last column

        for(int j=0;j< hsize;j++){
            int k = i*hsize + j;
            if(j==hsize-1)
                cout << mark(board[k]);
            else
                cout << mark(board[k]) + hedge;

        }

        cout << endl;
        // vetical edges : \ followede by / except the last column
        cout << vblank;

        if(i!=hsize-1){
            for(int j=0;j<hsize;j++){
                if(j==hsize -1)
                    cout << redge;
                else
                    cout << redge + ledge;
                } 
                cout << endl;
        }

        // for next row move nodes and edges to the right
        hblank +="  ";
        vblank +="  ";    
    }

    cout << endl;
}




bool hex_graph::dijkstra(const string str){ // str : "O" or "X"
    if(str == "X")
        std::cout << "running dijkstra algorithm for red path from top to bottom" << endl;
    else 
        std::cout << "running dijkstra algorithm for blue path from left to right" << endl;
    // shortest red X path from top to bottom algorithm

    // reset
    bool cSet[size];
    int c_cnt =0;
    for(int k=0;k<size;k++){
        cSet[k] =false; 
        path[k] =k;
    }
    color c;
    if(str == "X")
        c = color::RED; 
    else
        c = color::BLUE; 

    int temp = -1;
    vector<int> oneSide;
    vector<int> oppositeSide;

    // set up top and bottom side if str == "X"
    if(str == "X")
        for(int k=0;k<hsize;k++){
            oneSide.push_back(k);  // top row 
            oppositeSide.push_back(size-1-k); // bottom row
        }
    else
        for(int k=0;k<size;k += hsize){
            oneSide.push_back(k);  // left column
            oppositeSide.push_back(size-1-k); // right column
        }  
    

    // set cSet for the top side
    for(auto k : oneSide)
        if(board[k] == c){
            cSet[k] =true; c_cnt++; 
        } 

    temp = c_cnt;

    while(c_cnt < size && temp >0){

        // update cSet to include nearby blue neighbors
        for(int k=0; k<size;k++){
            for(auto n: edgelist[k]){
           
                if(cSet[k] && !cSet[n] && board[n] == c){
                    cSet[n] = true; c_cnt++;
                    path[n] = k; // connected path from k to n
                }
            }
        }
        if(temp == c_cnt){ // no more neighoring node with the same color and break from while loop
            break;
        }else{
            temp = c_cnt;
        }
        
    }

    // check if there is any cSet on the oppsite side. if true then there is a connected path
    // and game over and str won the game
    for(auto k: oppositeSide){
        if(cSet[k]){ 
            game_over = true;
            who_won = str; 
            return true;
        }
    }
    return false;

}



