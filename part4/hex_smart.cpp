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
        void reset_to_orig(); // reset board to the original state just right before the simulation
        int get_hsize(){return hsize;} // get number of rows
        color get_color(int k){ return board[k]; }
        int get_num_move(){return num_move;}
        void random_play(int k); // randomly assign color RED, color BLUE for all nodes on the graph
        void draw_board(); 
        void move(int k, color c){board[k] = c; board_orig[k] =c;  num_move++;} // assign color on node k 
        void move_back(int k){
            if(board_orig[k] != color::EMPTY){
                board_orig[k] = color::EMPTY; num_move--;
            }
        }
        bool move_is_legal(int k){// move is legal only when node color is blank and inside the board
            int i = k/hsize, j = k % hsize;
            return (k>=0 && k< size  && board[k] == color::EMPTY);
        }
        bool dijkstra(string str);
        // str is either "X" (red) or "O" (blue)
        // returns true if there is red path from top to bottom
        // returns true if there is a blue path from left to right
        int minpath(string str);
        // path  which minizes the cost from one side to the other side
        // red path is from top side to bottom and blue path from left to right

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
        vector<color> board_orig; // original copy of board before monte carlo simulation
        int num_move; // number of moves during game;
        vector<int> path; // path of connected nodes path[n]=k means path from k to n
        bool game_over; // updated after dijskstra(string str) method
        string who_won; // updated after dijskstra(string str) method
     
};

// str is either "X" or "O". get input i, j from keyboard and accept only a legal move
void makeMove(hex_graph &g, string str){
    bool legal_move = true;
    hex_graph::color c = str == "X" ? hex_graph::color::RED : hex_graph::color::BLUE;

    while(legal_move){
        g.draw_board();
        int i,j, k;
        cout << str <<"'s turn. Choose two integers i j separated by space"<< endl;
        cout << "For example 0 3 represent 0 th row and 3rd column"<< endl;
        
        cin >> i;
        cin >> j;
        k= i* g.get_hsize() + j;

        // if move is legal then assign a move and break from while loop
        if(g.move_is_legal(k)){
            g.move(k, c);
            legal_move = false;
        }else{
            cout << i << ", " << j << " are illegal move" << endl;
        }
    
    }

}



vector<int> block_bridge(hex_graph &g, int k,  int hsize){
    // "X": block bridge, "O": create bridge
    int size = hsize*hsize;
    vector<int> bridge_vec;
    
    // block opponent's bridge
    int br1, br2, car11, car12, car21, car22;
    br1 = k - 2*hsize +1;
    br2 = k + 2*hsize -1;
    car11= k -hsize;
    car12 = car11 + 1;
    car21 = k + hsize -1;
    car22 = car21 +1;
    hex_graph::color red = hex_graph::color::RED,  blue = hex_graph::color::BLUE, empty =hex_graph::color::EMPTY;
    if(br1>0 && g.get_color(k)==red && g.get_color(br1) == empty && g.get_color(car11) == empty && g.get_color(car12) == empty)
        bridge_vec.push_back(br1);
    if(br1>0 && g.get_color(k)==red && g.get_color(br1) == red && g.get_color(car11) == blue && g.get_color(car12) == empty)
        bridge_vec.push_back(car12);
    if(br1>0 && g.get_color(k)==red && g.get_color(br1) == red && g.get_color(car11) == empty && g.get_color(car12) == blue)
        bridge_vec.push_back(car11);
    else if(car11>0 && g.get_color(k)==red && g.get_color(car11) == empty)
        bridge_vec.push_back(car11);     // include certain neighbor
    
    if(br2< size && g.get_color(k)==red && g.get_color(br2) == empty && g.get_color(car21) == empty && g.get_color(car22) == empty)
        bridge_vec.push_back(br2);
    if(br2< size && g.get_color(k)==red && g.get_color(br2) == red && g.get_color(car21) == blue && g.get_color(car22) == empty)
        bridge_vec.push_back(car22);
    else if(car22< size && g.get_color(k)==red && g.get_color(car22) == empty)
        bridge_vec.push_back(car22);     // include certain neighbor
    if(br2 < size && g.get_color(k)==red && g.get_color(br2) == red && g.get_color(car21) == empty && g.get_color(car22) == blue)
        bridge_vec.push_back(car11);
        
    


    return bridge_vec;
    // create bridge to make longer path from east to west
 




}

vector<int> create_bridge(hex_graph &g, int k,  int hsize){
    // "X": block bridge, "O": create bridge
    // create bridge to make longer path from east to west
    int size = hsize*hsize;
    vector<int> bridge_vec;
    
  
    int br1, br2, car11, car12, car21, car22;
 
    hex_graph::color red = hex_graph::color::RED,  blue = hex_graph::color::BLUE, empty =hex_graph::color::EMPTY;

    // create bridge to make longer path from east to west
    br1 = k + 2 - hsize;
    br2 = k -2 + hsize;
    car11 = k+1;  car12 = k+1  - hsize;
    car21 = k -1; car22 = k -1 + hsize;
    if(br1>0 && g.get_color(k)==blue&& g.get_color(br1) == empty && g.get_color(car11) == empty && g.get_color(car12) == empty)
        bridge_vec.push_back(br1);
    if(br1>0 && g.get_color(k)==blue && g.get_color(br1) == blue && g.get_color(car11) == red && g.get_color(car12) == empty)
        bridge_vec.push_back(car12);
    if(br1>0 && g.get_color(k)==blue && g.get_color(br1) == blue && g.get_color(car11) == empty && g.get_color(car12) == red)
        bridge_vec.push_back(car11);
    else if(car11>0 && g.get_color(k)==blue && g.get_color(car11) == empty)
        bridge_vec.push_back(car11); 
    if(br2< size && g.get_color(k)==blue && g.get_color(br2) == empty && g.get_color(car21) == empty && g.get_color(car22) == empty)
        bridge_vec.push_back(br2);
    if(br2< size && g.get_color(k)==blue && g.get_color(br2) == blue && g.get_color(car21) == red && g.get_color(car22) == empty)
        bridge_vec.push_back(car22);
    if(br2 < size && g.get_color(k)==blue && g.get_color(br2) == blue && g.get_color(car21) == empty && g.get_color(car22) == red)
        bridge_vec.push_back(car21);
    else if(car21 < size && g.get_color(k)==blue && g.get_color(car21) == empty)
        bridge_vec.push_back(car21);

    return bridge_vec;

}




int monte_carlo(hex_graph& g, int num_play){

        string str ="X";

        int good_pos=-1; // best position for O player obtained from mon carlo simulation 
        int hsize = g.get_hsize();
        int size = hsize*hsize;

        vector <int> reasonable_moves; // possibly good positions for BLUE player

        // reasonable moves
        // for(int k=0;k<size;k++){
        //     for(auto w: block_bridge(g,k, hsize)){
        //         // w is legal move
        //         g.move(w, hex_graph::color::BLUE);    
        //         reasonable_moves.push_back(w);
        //         g.move_back(w);
        //         g.reset_to_orig();
        //     }
        //     for(auto w: create_bridge(g,k, hsize)){
        //         // w is legal move
        //         g.move(w, hex_graph::color::BLUE);
        //         reasonable_moves.push_back(w);
        //         g.move_back(w);
        //         g.reset_to_orig(); // reset to original board before simulation
        //     }
        // }
        for(int k=0;k<size;k++)
            reasonable_moves.push_back(k);
  
        int xmincnt=-1; // want to minimize the chance for red player to win
        for(auto k: reasonable_moves){
                int xcnt=0; 
                if(g.move_is_legal(k)){
                    for(int i=0; i<num_play;i++){
                        g.random_play(k);
                
                        if(g.dijkstra(str))
                            xcnt++;
                        // move back
                        g.move_back(k);
                        g.reset_to_orig();
                    }
                    if(xmincnt ==-1 || xcnt< xmincnt){
                        xmincnt = xcnt; // want to minimize the chance for red player to win
                        good_pos = k;  // update a good position for blue player
                        
                    }

                }

        }
     
        cout << "\nrecent position for O: (" << good_pos /hsize << ", " << good_pos % hsize << ")\n";
        return good_pos;
}
int main(){
    int hsize = 7;
    cout << "starting the hex game\n";
    cout << " Choose the number of rows for the game: (by default 11 by 11 hex board)";
    // cin >> hsize;

    int size = hsize*hsize;
    hex_graph g(hsize);
    g.reset();
    // g.random_play(12);
    // g.dijkstra("X");
    // g.dijkstra("O");
    g.draw_board();
    string str; // str  is "X" or "O"

    while(true){
        str ="X"; // "X" player's turn
        makeMove(g, str);

        int num_play =5000;
        int min_row, min_col, k;
        int xcnt =0, xmincnt =-1;

     
        k = monte_carlo(g, num_play);
        if(k!= -1){
            g.move(k, hex_graph::color::BLUE);
            if(g.dijkstra("X")){
                cout << "\nX won the game\n";
                g.draw_board();
                break;
            }
            if(g.dijkstra("O")){
                cout << "\nO won the game\n";
                g.draw_board();
                break;
            }

        }
        

    }

    return 0;
}




hex_graph::hex_graph(int hsize): hsize(hsize), size(hsize*hsize){
    edgelist.resize(size);
    board.resize(size);
    board_orig.resize(size);
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
    num_move = 0;
    for(int k=0; k<size;k++)
        board[k] = color::EMPTY;
}

void hex_graph::reset_to_orig(){
   
    for(int k=0; k<size;k++)
        board[k] = board_orig[k];
}

void hex_graph::random_play(int pos){
    move(pos, color::BLUE);
    int num_move_remain = size -num_move;
    int i=0, k=0;
    vector<color> board_remain(num_move_remain);
    for(int k=0; k<num_move_remain;k++)
        if(k < (size + 1)/2)
            board_remain[k] = color::RED;
        else
            board_remain[k] = color::BLUE;
    random_shuffle(board_remain.begin(), board_remain.begin() + num_move_remain);
    for(;k<size;k++){
        if(board[k] == color::EMPTY){
            board[k]= board_remain[i];
            i++;
        }
    }
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
    // if(str == "X")
    //     std::cout << "running dijkstra algorithm for red path from top to bottom" << endl;
    // else 
    //     std::cout << "running dijkstra algorithm for blue path from left to right" << endl;
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


int hex_graph::minpath(const string str){
    string opp_str = str == "X" ? "O": "X";
    if(dijkstra(str))
        return 0;
    if(dijkstra(opp_str))
        return -1;  // opponent won and no possible path for the player to win

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

    // cSet will become set of sites connecting oneSide to the oppositeSide
    // oSet are one unit away from cSet

     // initial setup
    bool cSet[size], oSet[size];
    int mincost[size];
    int c_cnt =0, minlength = -1; // mincost of path from one side to the opposite side
    for(int k=0;k<size;k++){
        cSet[k] =false; 
        oSet[k] = false;
        mincost[k] = -1;
    }
    color c;
    if(str == "X")
        c = color::RED; 
    else
        c = color::BLUE; 


    int temp = -1;

    // set cSet for one side side
    for(auto k : oneSide){
        if(board[k] == c){
            cSet[k] =true; c_cnt++;  mincost[k]=0;
           
        } 
        if(board[k] == hex_graph::color::EMPTY){
            cSet[k] =true; c_cnt++;  mincost[k]=1;
        } 
    }
    temp =c_cnt; // cSet is not empty if temp>0


    while(c_cnt < size && temp >0){
        temp = c_cnt;
        // update oSet and mincost for oSet to include nearby neighbors
        for(int k=0; k<size;k++){
            for(auto n: edgelist[k]){
           
                if(cSet[k] && !cSet[n] && (board[n] == c || board[n] == hex_graph::color::EMPTY) ){
                    oSet[n] = true;
                    if(mincost[n] == -1){
                        if(board[n] == c)
                            mincost[n] = mincost[k];
                        else
                            mincost[n] = mincost[k] + 1;
                    }
                          
                }
                if(cSet[k] && oSet[n])
                    if(mincost[n] > mincost[k])
                        if(board[n] == c)
                            mincost[n] = mincost[k];
                        else          
                            mincost[n] = mincost[k] + 1; // case of color::EMPTY
            }


        }
        // now include all oSet to cSet and reset oset to empty
        for(int k=0;k<size;k++){
            if(oSet[k]){
                oSet[k] =false; cSet[k] = true; c_cnt++; 
            }
        }

        if(temp == c_cnt){ // cSet reached the opposite side so exit the while loop
            break;
        }      
    }

    // check if there is any cSet on the oppsite side. if true then there is a min path
    for(auto k: oppositeSide){
        if(cSet[k]){ 
           minlength  = mincost[k];

        }
        if(cSet[k] && mincost[k] < minlength)
            minlength = mincost[k];
    }
    return minlength;
}
