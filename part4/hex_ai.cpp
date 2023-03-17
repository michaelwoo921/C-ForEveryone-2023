#include <iostream>
#include <cassert>
#include <vector>
#include<algorithm>

using namespace std;

class hex_graph {
    public:
        enum class color {EMPTY, RED, BLUE};
        hex_graph(int h=11);
        int get_hsize(){return hsize;}
        void draw_board();
        void make_move(string str); // accept a move from key board only when it is legal 
        void move(int k, color c);
        void move_back(int k);
        void reset();
        void reset_to_original(); // copy of board before monte carlo simulation
        bool move_is_legal(int k);
        string mark(color c);
        color get_color(string str);

        bool connected(int k1, int k2);
        bool virtual_connected(int k1, int k2);
        bool connected(string str); // modified dijkstra to run monte carlo algo faster
        bool virtual_connected(string str); // connected using nearest neighbor and "next" nearest neihbor 
        vector<int> cluster(int k); // set of connected sites to site k
        vector<int> virtual_cluster(int k); // set of "virtual" connected sites to site k

        bool neighbor(int k, int l); // true if k is nearest neighbor of l
        // bridge neigbbor counts the number of common nearest neighbor of k and l which have empty colors
        int bridge_neighbor(int k, int l);    //return -1, 0, 1, 2
        bool close_to(int k, color c); // k has a nearest neighbor or bridge neighbor which has color c

    
        int dijkstra(string str); // min dist of (X or O) nearest neighbor path from one end to the other end
        int virtual_dijkstra(string str); // min dist of (X or O) "bridge" neighbor path from one end to the other end
        void randomplay(int k, color c); // randomized play of the rest of unoccupied sites after site k has color c
        int monte_carlo(); // return the best move of blue player by some strategies and using monte carlo method 

        // some of possible moves 
        vector<int> edge_moves(); // all empty nearest neighbor or bridge neighbor of res sites
        vector<int> center_moves(); // move to center or nearest neighbor of center
        vector<int> plausible_moves();  // edge moves, center moves, block red path, extend blue path

        vector<int> block();  // set of sites that blue player blocks red nearest neighbor path
        vector<int> virtual_block();   // set of sites that blue player blocks red "bridge" neighbor path
        vector<int> extend(); // set of sites that blue player extends blue nearest neighbor path
        vector<int> virtual_extend();  // set of sites that blue player extends blue "bridge" neighbor path

        vector<int> block_weak();  // set of sites that blue player blocks(weaker version) red nearest neighbor path
        vector<int> virtual_block_weak();   // set of sites that blue player blocks(weaker version) red "bridge" neighbor path
        void hex_program(); // main program which runs

        void connection_test(); // test for conneced and virtually connected method
        void dijkstra_test(); // test to debug dijkstra and virtual_dijkstra method
        void monte_carlo_test(); // test for monte_carlo method

    private: 
        int hsize, size;
        int num_moves=0;
        int num_play =5000;
        vector<color> board;
        vector<color> board_orig;
        vector<int> top_side;
        vector<int> bottom_side;
        vector<int> left_side;
        vector<int> right_side;
        vector< vector<int> > edgelist;
        vector< vector<int> > virtual_edgelist;
        vector<vector<int>> top_bridge;
        vector<vector<int>> bottom_bridge;
        vector<vector<int>> left_bridge;
        vector<vector<int>> right_bridge;

        vector<int> xmincost; // min distance of red path from top to bottom
        vector<int> omincost; // min distance of blue path from left to right
        vector<int> virtual_xmincost; // min bridge neighor red path from top to bottom
        vector<int> virtual_omincost;  // min bridge neighbor blue path from left to right

};


vector<int> hex_graph::plausible_moves(){
    vector<int> possible_moves, good_moves;
    color rr =color::RED, bb = color::BLUE, ee = color::EMPTY;

    // step 1: the first move
    if(num_moves <=2)
        return center_moves();

    // step 2 always reduce virtual distance or distance of blue cluster
    // or always block (increase) virtual distance or distance of red cluster
    int xdist, odist;
    xdist = dijkstra("X"); odist = dijkstra("O");

    if(odist<=xdist){ 
        //  extend blue virtual dijkstra distance path
        if(virtual_extend().size()>0){
            // cout << "virtual extend, ";
            return virtual_extend();
        }
        //  extend blue dijkstra distance path
        if(extend().size()>0){
            // cout << "extend, ";
            return extend();
        }
    }else{
            //  block red virtual dijkstra distance path
        if(virtual_block().size()>0){
            // cout << "virtual block, ";
            return virtual_block();
        }
        //  block red dijkstra distance path
        if(block().size()>0){
            // cout << "block, ";
            return block();
        }
    }

    //  block red virtual dijkstra distance if the distance might decrease in the next red move
    if(virtual_block_weak().size()>0){
        return virtual_block_weak();
    }
    //  block red dijkstra distance
    if(block_weak().size()>0){
        return block_weak();
    }

      
    // step 3: edge moves : contain all neighbors of red sites (incase all previous strategies fail)
    return edge_moves();

}



int main(){
    int hsize=11; // change hsize to 5 for testing. all tests are written for hsize =5.;
    int size = hsize*hsize;
    hex_graph g(hsize);
    srand(time(0));

    //  comment out hex_program to test other methods
    g.hex_program();
  
    // testing connections for hsize =5
    // g.connection_test();

    //  testing dijkstra and virtual_dijkstar for hsize = 5
    // g.dijkstra_test();

    // testing monte carlo method for hsize= 5
    // g.monte_carlo_test();
  
}



// str is either "X" or "O". get input i, j from keyboard and accept only a legal move
void hex_graph::make_move(string str){
    bool legal_move = true;
    hex_graph::color c = str == "X" ? hex_graph::color::RED : hex_graph::color::BLUE;
    
    while(legal_move){
        
        int i,j, k;
        cout << str <<"'s turn. Choose two integers i j separated by space"<< endl;
        cout << "For example 0 3 represent 0 th row and 3rd column"<< endl;
        
        cin >> i;
        cin >> j;
        k= i* hsize + j;
        assert(i>=0 && i<hsize && j>=0 && j<hsize);

        if(move_is_legal(k) ){
            move(k, c);
            legal_move = false; // make a move then break from loop
        }else{
            cout << i << ", " << j << " are illegal move" << endl;
            draw_board();
        }

    
    }

}


// constructor (build hex_graph with sites, edges, virtual edges, top, bottom, left, right sides, mincosts,)
hex_graph::hex_graph(int h): hsize(h), size(h*h){
    board.resize(size);
    board_orig.resize(size);
    edgelist.resize(size);
    virtual_edgelist.resize(size);

    top_bridge.resize(hsize -1);
    bottom_bridge.resize(hsize -1);
    left_bridge.resize(hsize -1);
    right_bridge.resize(hsize -1);
    
    xmincost.resize(size);
    omincost.resize(size);
    virtual_xmincost.resize(size);
    virtual_omincost.resize(size);

    for(int k=0;k<size;k++){
        board[k] = color::EMPTY;
    }
    for(int k=0;k<hsize;k++)
        top_side.push_back(k);
    for(int k=size -hsize; k<size;k++)
        bottom_side.push_back(k);
    for(int k=0;k<size; k += hsize)
        left_side.push_back(k);
    for(int k= hsize-1;k<size; k+=hsize)
        right_side.push_back(k);
    int i,j; // ith row and jth column
    // edge_list[k] contains nearest neighbor nodes: 6 nodes for internal, 2 or 3 nodes on the corner, 4 nodes on each side 
    for(int k=0; k<size; k++){
        i = k / hsize; j = k % hsize;  // node number k

        // for the first column
        if(j == 0 && k !=0 && k != size - hsize){
            edgelist[k].push_back(k+1); 
            edgelist[k].push_back(k - hsize);
            edgelist[k].push_back(k - hsize+ 1);    
            edgelist[k].push_back(k + hsize);
            virtual_edgelist[k].push_back(k-hsize + 2);
            virtual_edgelist[k].push_back(k + hsize + 1);
            if(k - 2*hsize + 1>0){
                virtual_edgelist[k].push_back(k - 2*hsize + 1);
            }           
        }

        if(k == 0){ // top left corner
            edgelist[k].push_back(1); 
            edgelist[k].push_back(hsize);
            virtual_edgelist[k].push_back(hsize + 1);
        }
        if(k == size - hsize){ // bottom left corner
            edgelist[k].push_back(k+1); 
            edgelist[k].push_back(k - hsize);   
            edgelist[k].push_back(k - hsize + 1);
            virtual_edgelist[k].push_back(k - hsize + 2);
            virtual_edgelist[k].push_back(k - 2*hsize + 1);
        }

        // for the last column
        if(j == hsize -1 && k != hsize -1 && k != size -1){
            edgelist[k].push_back(k-1); 
            edgelist[k].push_back(k - hsize);
            edgelist[k].push_back(k + hsize -1); 
            edgelist[k].push_back(k + hsize);
            virtual_edgelist[k].push_back(k - hsize -1);
            virtual_edgelist[k].push_back(k + hsize - 2);
            if(k+ 2*hsize -1 <size -1)
                virtual_edgelist[k].push_back(k + 2* hsize -1);
        }  
        if(k == hsize -1){  // top right corner
            edgelist[k].push_back(k-1); 
            edgelist[k].push_back(k + hsize);   
            edgelist[k].push_back(k + hsize -1);   
            virtual_edgelist[k].push_back(k + hsize - 2);
            virtual_edgelist[k].push_back(k + 2* hsize -1);
        }
        if(k == size -1){ // botom right corner
            edgelist[k].push_back(size -2); 
            edgelist[k].push_back(size -1 - hsize);
            virtual_edgelist[k].push_back(k - hsize -1);
        }
        // for the first row
        if(k>0 && k< hsize-1){
            edgelist[k].push_back(k-1); 
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + hsize); 
            edgelist[k].push_back(k + hsize -1);
            virtual_edgelist[k].push_back(k + hsize + 1);
            virtual_edgelist[k].push_back(k + 2* hsize -1);
            if(k>1)
                virtual_edgelist[k].push_back(k + hsize - 2);
        }

        // for the last row
        if(k > size - hsize && k < size-1){
            edgelist[k].push_back(k-1); 
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k - hsize);   
            edgelist[k].push_back(k - hsize + 1);
            virtual_edgelist[k].push_back(k - hsize -1);
            virtual_edgelist[k].push_back(k - 2*hsize + 1);
            if(k<size-2)
                virtual_edgelist[k].push_back(k - hsize + 2);
        }

        // 6 nearbor nodes for internal nodes
        if(i!=0 && i != hsize -1 && j!=0 && j!= hsize -1){
            edgelist[k].push_back(k-hsize); 
            edgelist[k].push_back(k-hsize +1);
            edgelist[k].push_back(k-1); 
            edgelist[k].push_back(k+1);
            edgelist[k].push_back(k + hsize);   
            edgelist[k].push_back(k + hsize -1);
            virtual_edgelist[k].push_back(k - hsize -1);
            virtual_edgelist[k].push_back(k + hsize + 1);
            if(i>1)    
                virtual_edgelist[k].push_back(k - 2*hsize + 1);
            if(j>1)
                virtual_edgelist[k].push_back(k + hsize - 2);
            if(i<hsize-2)  
                virtual_edgelist[k].push_back(k + 2*hsize -1);
            if(j<hsize -2)
                virtual_edgelist[k].push_back(k - hsize + 2);

        }
    }


    for(int ii=0;ii<hsize-1;ii++){
        int first =ii, second = first+1, third = first + hsize;
        top_bridge[ii].push_back(first);
        top_bridge[ii].push_back(second);
        top_bridge[ii].push_back(third);

        first = size -hsize + ii; second = first+1; third = second - hsize;
        bottom_bridge[ii].push_back(first);
        bottom_bridge[ii].push_back(second);
        bottom_bridge[ii].push_back(third);

        first = ii*hsize; second = first + hsize; third = first + 1;
        left_bridge[ii].push_back(first);
        left_bridge[ii].push_back(second);
        left_bridge[ii].push_back(third);

        first = ii*hsize + hsize -1; second = first + hsize; third = second - 1;
        right_bridge[ii].push_back(first);
        right_bridge[ii].push_back(second);
        right_bridge[ii].push_back(third);


    }
}


void hex_graph::hex_program(){
   
    cout << "Running hex program. It uses 11 by 11 board by default\n";
    cout<< " You are a Red player X and you need to create path from top to bottom to win the game\n";
 

    bool game_playing = true;
    while(game_playing){
        draw_board();  
        make_move("X");

        // game over when X is connected
        if(connected("X")){
            draw_board();
            cout << "\nX won \n";
            game_playing = false;
            break;
        }
        int best_pos;
        best_pos= monte_carlo();
        cout << "best O position: row and col (" << best_pos /hsize << ", " << best_pos % hsize <<") " << endl;

        // best move chosen by monte carlo method
        move(best_pos, hex_graph::color::BLUE);
        
        // if O is connected game over
        if(connected("O")){
            draw_board();
            cout << "\nO won \n";
            game_playing = false;
            break;
        }
    }

}

void hex_graph::draw_board(){
    string hedge=" _ ";
    string ledge = " /";
    string redge =" \\";
    string hshift ="";
    string labelshift = "  ";
    // labeling jth column
    cout <<endl;
    cout << "  ";
    for(int j=0;j<hsize;j++)
        cout << j % 10 << "   ";
    cout << endl;

    for(int i=0;i<hsize;i++){
        // labeling ith row
        cout << i % 10<< " ";
        for(int j=0;j< hsize;j++){
            int k = i*hsize + j;
            if( j < hsize -1){
                cout << mark(board[k]) << hedge;
            }else{
                cout << mark(board[k]);
            }       
        }
        cout << endl << hshift;
        cout << labelshift;
        if( i< hsize -1){
            for(int j=0;j< hsize;j++){  
                if(j!=hsize -1)
                    cout << redge << ledge;   
                else
                    cout << redge;
            }
            hshift += "  ";
            cout << endl << hshift;
        }
    }

    // labeling jth column
    for(int j=0;j<hsize;j++)
        cout << j % 10 << "   ";
    cout << endl;
}

// return corresponding color for X or O or "."
string hex_graph::mark(color c){
    switch(c){
        case color::EMPTY: return ".";
        case color::RED: return "X";
        case color::BLUE: return "O";
        default: return ".";
    }
}

hex_graph::color hex_graph::get_color(string str){
    if(str =="X"){
        return color::RED;
    }else if(str =="O"){
        return color::BLUE;
    }
    else{
        return color::EMPTY;
    }
}


void hex_graph::move(int k, color c){ 
    board[k] = c; board_orig[k] =c;  num_moves++;}
bool hex_graph::move_is_legal(int k){ 
    if(k<0 || k>=size)
        return false;
    return board[k] == color::EMPTY;}
void hex_graph::move_back(int k){board[k] = color::EMPTY; board_orig[k] = color::EMPTY; num_moves--;}
void hex_graph::reset(){
    for(int k=0;k<size;k++){
        board[k] = color::EMPTY;
        board_orig[k] = color::EMPTY;
    }
    num_moves =0;
}
void hex_graph::reset_to_original(){
    for(int k=0;k<size;k++)
        board[k] =board_orig[k];
}
int hex_graph::bridge_neighbor(int k, int l){ 
    // return the number of empty colors of common neighbors of k and l
    // return -1, 0, 1, or 2 (-1 means l is not virtual_edgelist of k)
    int carrier_cnt= 0, blue_cnt =0, red_cnt =0, empty_cnt =0;
    if(neighbor(k,l))
        return -1;
    for(auto w1:edgelist[k])
        for(auto w2: edgelist[l]){
            if(w1 == w2){
                carrier_cnt++;
                if(board[w1] == color::RED)
                    red_cnt++;
                if(board[w1] == color::BLUE)
                    blue_cnt++;
                if(board[w1] == color::EMPTY)
                    empty_cnt++;
            }
        }
    if(carrier_cnt != 2)
        return -1;
    return empty_cnt;
}

 bool hex_graph::neighbor(int k, int l){ // check whether k and l are nearest neighbor
    for(auto w: edgelist[k]){
        if(w == l)
            return true;
    }
    return false;
}
                   

bool hex_graph::close_to(int k, color c){
    //return true if k is close to color c cluster
    for(auto w: edgelist[k]){
        if(board[w] == c)
            return true;
    }
    for(auto w: virtual_edgelist[k]){
        if(board[w] == c)
            return true;
    }
    return false;
}


bool hex_graph::connected(string str){
    // str is either X or O
    // fast algorithm only to check if there is a path from one side to the opposite side by the same color

    // setup cSet initially set false,  c_cnt: 0
    bool cSet[size];
    int c_cnt =0;

    // setup one side and opposite side
    vector<int> one_side; 
    vector<int> opposite_side;

    if(str == "X"){
        for(auto w: top_side)
            one_side.push_back(w);
        for(auto w: bottom_side)
            opposite_side.push_back(w);
    }
    if(str == "O"){
        for(auto w: left_side)
            one_side.push_back(w);
        for(auto w: right_side)
            opposite_side.push_back(w);
    }

    for(int k=0;k<size;k++)
        cSet[k]= false;

    // initial update of cSet at oneside
    color c = get_color(str);
    color c_opp=c;  // color of opponent
    if(c == color::RED)
        c_opp = color::BLUE;
    if(c == color::BLUE)
        c_opp = color::RED;

    for(auto k: one_side){
        if(board[k] == c){
            cSet[k]=true; c_cnt++;
        }
    }
    if(c_cnt ==0)
        return false;
    int temp = c_cnt; // when c_cnt does not update, while loop exits

    while(c_cnt< size && temp>0){
        temp = c_cnt;
        // update cSet which is one unit away from previous cSet
        for(int k=0;k<size; k++){
            if(cSet[k]){
                for(auto w: edgelist[k]){
                    if(!cSet[w] && board[w] == c){
                        cSet[w] = true; c_cnt++;
                    }
                }
            }
            
        }
        if(temp == c_cnt){ // meaning cSet cannot expand and break from while loop
            break;
        }   
    }
    
    for(auto k: opposite_side){
        if(cSet[k])
            return true;
    }

    return false;
}

bool hex_graph::connected(int k1, int k2){
    // str is either X or O
    // fast algorithm only to check if there is a path from one side to the opposite side by the same color

    // setup cSet initially set false,  c_cnt: 0
    bool cSet[size];
    int c_cnt =0;

    color c1 = board[k1], c2 = board[k2], ee = color::EMPTY;
    if(c1!=c2 || c1==ee || c2 ==ee)
        return false;

    for(int k=0;k<size;k++)
        cSet[k]= false;

    // initial update of cSet
    cSet[k1] = true; c_cnt =1;

    int temp = c_cnt; 

    while(true){
        temp = c_cnt;
        // update cSet which is one unit away from previous cSet
        for(int k=0;k<size; k++){
            if(cSet[k]){
                for(auto w: edgelist[k]){
                    if(!cSet[w] && board[w] == c1){
                        cSet[w] = true; c_cnt++;
                    }
                }
            }
            
        }
        if(temp == c_cnt){ 
            break;
        }   
    }
    
    return cSet[k2];
}


bool hex_graph::virtual_connected(string str){
    // str is either X or O
    // fast algorithm only to check if there is a virtual path from one side to the opposite side by the same color

    // setup cSet initially set false,  c_cnt: 0
    bool cSet[size];
    int c_cnt =0;
    color c = get_color(str);
    color c_opp=c;  // color of opponent
    if(c == color::RED)
        c_opp = color::BLUE;
    if(c == color::BLUE)
        c_opp = color::RED;
    color c_empty = color::EMPTY;

    // setuo one side and opposite side
    vector<int> one_side, opposite_side;
    
    if(str == "X"){
        for(auto w: top_side)
            one_side.push_back(w);
        for(auto w: bottom_side)
            opposite_side.push_back(w);
    }
    if(str == "O"){
        for(auto w: left_side)
            one_side.push_back(w);
        for(auto w: right_side)
            opposite_side.push_back(w);
    }

    for(int k=0;k<size;k++)
        cSet[k]= false;
    
    // initial update of cSet at oneside
    for(auto k: one_side){
        if(board[k] == c){
            cSet[k]=true; c_cnt++;
        }
    }
    if(str == "X"){
        for(int k=0;k<hsize-1;k++){
            if(board[k]==c_empty && board[k+1] == c_empty
              && board[k+hsize] == c){
                cSet[k+hsize] =true; c_cnt++;
            }
        }
    }
    if(str == "O"){
        for(int k=0;k<size-hsize;k += hsize){
            if(board[k]==c_empty && board[k+hsize] == c_empty
              && board[k+1] == c){
                cSet[k+1] =true; c_cnt++;
            }
        }
    }


    int temp = -1; // when c_cnt does not update, while loop exits
    if(c_cnt ==0)
        return false;


    while(temp != c_cnt){
        temp = c_cnt;
        // update cSet which is at most one bridge unit away from previous cSet
        // nearest neighbor
        for(int k=0;k<size; k++){
            for(auto w: edgelist[k]){
                if(cSet[k] && !cSet[w] && board[w] == c){
                    cSet[w] = true; c_cnt++;
                }
            }
        }
        // cSet with one bridge unit away
        for(int k=0;k<size; k++){
            for(auto w: virtual_edgelist[k]){
                if(cSet[k] && !cSet[w] && board[w] == c && bridge_neighbor(k,w) == 2){
                    // two carriers have empty colors above
                            cSet[w] = true; c_cnt++;
                }
            }
        }

        // check if connected to opposite side
        for(auto w: opposite_side){
            if(cSet[w])
                return true;
        }
        if(str == "X"){
            for(int k=size - hsize;k<size-1;k++){
                if(board[k]==c_empty && board[k+1] == c_empty
                && board[k- hsize+1] == c){
                    if(cSet[k -hsize +1])
                        return true;
                }
            }
        }
        if(str == "O"){
            for(int k=hsize-1;k<size-hsize;k += hsize){
                if(board[k]==c_empty && board[k+hsize] == c_empty
                && board[k+ hsize -1] == c){
                    if(cSet[k + hsize -1])
                        return true;
                }
            }
        }

        if(temp ==c_cnt){
            cout << str <<" break from while loop\n";
            break;
        }
    }




    cout <<str <<  " ***c_cnt " <<  c_cnt<< "\n";
        return false;

}


bool hex_graph::virtual_connected(int k1, int k2){
    // setup cSet initially set false,  c_cnt: 0
    bool cSet[size];
    int c_cnt =0;

    color c1 = board[k1], c2 = board[k2], ee = color::EMPTY;
    if(c1!=c2 || c1==ee || c2 ==ee)
        return false;

    for(int k=0;k<size;k++)
        cSet[k]= false;

    // initial update of cSet
    cSet[k1] = true; c_cnt =1;

    int temp = c_cnt; 

    while(true){
        temp = c_cnt;
        // update cSet which is one unit away from previous cSet
        for(int k=0;k<size; k++){
            if(cSet[k]){
                for(auto w: edgelist[k]){
                    if(!cSet[w] && board[w] == c1){
                        cSet[w] = true; c_cnt++;
                    }
                }
            } 
        }

        // cSet with one bridge unit away
        for(int k=0;k<size; k++){
            if(cSet[k]){
                for(auto w: virtual_edgelist[k]){
                    if(!cSet[w] && board[w] == c1 && bridge_neighbor(k,w) == 2){
                        // two carriers have empty colors above
                        cSet[w] = true; c_cnt++;
                    }
                }
            }

        }
        if(temp == c_cnt){ 
            break;
        }   
    }
    
    return cSet[k2];
}


vector<int> hex_graph::cluster(int k){
    vector<int> shape;
    if(board[k] == color::EMPTY){
        return shape;
    }

    color c = board[k]; // either RED or BLUE
    bool cSet[size]; int c_cnt =0;
    // initial setup for cSet
    for(int i=0;i<size;i++){
        cSet[i] = false;
    }
    cSet[k] = true; c_cnt++;
    shape.push_back(k);
    int temp =c_cnt;
    while(c_cnt < size && temp>0){
        temp = c_cnt;
        for(int l=0; l<size; l++){
            // update cSet to include neighbor with the same color
            for(auto w: edgelist[l]){
                if(cSet[l] && !cSet[w] && board[w] ==c){
                    cSet[w] = true; c_cnt++;
                    shape.push_back(w);
                }
            }
        }
        if(temp == c_cnt){  // no more cSet update and break from loop
            break;
        }

    }
    
    return shape;


}


vector<int> hex_graph::virtual_cluster(int k){
    vector<int> shape;
    if(board[k] == color::EMPTY){
        return shape;
    }

    color c = board[k]; // either RED or BLUE
    bool cSet[size]; int c_cnt =0;
    // initial setup for cSet
    for(int i=0;i<size;i++){
        cSet[i] = false;
    }
    cSet[k] = true; c_cnt++;
    shape.push_back(k);
    int temp =c_cnt;
    while(c_cnt < size && temp>0){
        temp = c_cnt;
        for(int l=0; l<size; l++){
            // update cSet to include neighbor with the same color
            for(auto w: edgelist[l]){
                if(cSet[l] && !cSet[w] && board[w] ==c){
                    cSet[w] = true; c_cnt++;
                    shape.push_back(w);
                }
            }
            // update cSet to include bridge neighbor with the same color
            for(auto w: virtual_edgelist[l]){
                if(cSet[l] && !cSet[w] && board[w] == c && bridge_neighbor(w,l) ==2){
                    // six virtual neighbor cases with two carriers empty color
                    cSet[w] = true; c_cnt++; shape.push_back(w);
                }
            }
        }
        if(temp == c_cnt){  // no more cSet update and break from loop
            break;
        }

    }
    
    return shape;


}


int hex_graph::dijkstra(string str){
    // str is either X or O
    // return time which takes from oneside to the other side   
    // X: red path from top to botom, O: blue path from east to west

    // initial setup
    int dist = 0, c_cnt = 0, o_cnt = 0; // dist (time), c_cnt: number of elements reachable by time t =dist
    bool cSet[size], oSet[size]; // o_cnt: size of oSet
    vector<int> mincost(size); // initially set to be -1 mincost is same as first passage time
    for(int k=0;k<size;k++){
        cSet[k]=false; oSet[k] = false;  mincost[k]=-1;  
    }

    // setup one side and opposite side
    vector<int> one_side, opposite_side;
    if(str == "X"){
        for(auto w: top_side)
            one_side.push_back(w); 
        for(auto w: bottom_side)
            opposite_side.push_back(w); 
    }
    if(str == "O"){
        for(auto w: left_side)
            one_side.push_back(w); 
        for(auto w: right_side)
            opposite_side.push_back(w); 
    }

    color c = str == "X" ? color::RED : color::BLUE;
    color c_opp =c;  // color of opponent
    if(c == color::RED)
        c_opp = color::BLUE;
    if(c == color::BLUE)
        c_opp = color::RED;

    // initial update of cSet and oSet
    // step 1 include all elements whcih can be reached at time 0 to cSet
    dist =0;
    for(auto k: one_side){
        if(board[k] == c){
            cSet[k] = true; c_cnt++; mincost[k] = 0;
        }
    }
    for(int k=0;k<size;k++)
        if(cSet[k])
            for(auto w: cluster(k))
                if(!cSet[w]){
                    cSet[w] = true; c_cnt++; mincost[w]=0;
                }


    // step 2 include all empty color neighbors of cSet and one side to oSet 
   for(auto k: one_side){
        if(board[k]== color::EMPTY){
            oSet[k] = true; o_cnt++; mincost[k]=1;
        }
    }
    for(int k=0;k<size;k++)
        if(cSet[k])
            for(auto w: edgelist[k])
                if(!cSet[w] && !oSet[w] && board[w]==color::EMPTY){
                    oSet[w] = true; o_cnt++; mincost[w]=1;     
                }
    // step 3 include color c clusters neighbors of oSet to oSet        
    for(int k=0;k<size;k++)
        if(oSet[k] && board[k]==color::EMPTY)
            for(auto w: edgelist[k]){
                if(!cSet[w] && !oSet[w] && board[w] ==c){
                    oSet[w] = true; o_cnt++; mincost[w]=1;
                }
            }
    for(int k=0;k<size;k++)
        if(oSet[k] && board[k]==c)
            for(auto w: cluster(k)){
                if(!cSet[w] && !oSet[w]){
                    oSet[w] = true; o_cnt++; mincost[w]=1;
                }

            }
        
 
    // increment time by 1 then update cSet and update oSet
    while(o_cnt >0){
        dist++;
        // update cSet which can be reached by time (dist)
        for(int k=0;k<size;k++){  
            if(oSet[k]){ 
                cSet[k] = true; c_cnt++; oSet[k] = false; o_cnt--;
            }
        }

        // update oSet which can be reached by time = dist +1
        // step 1: all empty color neighbor of cSets to oSet
        for(int k=0;k<size;k++){
            if(cSet[k] && mincost[k] ==dist){
                for(auto w: edgelist[k])
                    if(!cSet[w] && !oSet[w] && board[w] == color::EMPTY){
                        oSet[w] = true; o_cnt++;
                        if(mincost[w]==-1 || mincost[w] > mincost[k]){
                            mincost[w] = mincost[k] + 1;
                        }                                
                    }           
            }
        }
        
        // step 2: include color c clusters neighbors of oSet to oSet       
        for(int k=0;k<size;k++)
            if(oSet[k] && board[k] == color::EMPTY)
                for(auto w: edgelist[k]){
                    if(!cSet[w] && !oSet[w] && board[w] ==c){
                        oSet[w] = true; o_cnt++;
                        mincost[w] =mincost[k];             
                    }
                }
        for(int k=0;k<size;k++)
            if(oSet[k] && board[k] == c){
                for(auto w: cluster(k))
                    if(!cSet[w] && !oSet[w]){
                        oSet[w] = true; o_cnt++;
                        mincost[w] =mincost[k];     
                    }
            }  
                  
    }
    // save mincost to private class variables
    for(int k=0;k<size; k++)
        if(str=="X")
            xmincost[k] =mincost[k];
        else
            omincost[k] =mincost[k];



   
    // either cSet reaches opposite side in min_dist time or cannot be reached (return -1)
    // or oppoaite side not reachable
    int min_dist =-1;
    for(auto k: opposite_side){
        if(mincost[k]>=0 && min_dist<0)
            min_dist = mincost[k]; 
        if(min_dist > mincost[k] && mincost[k] >=0){
            min_dist = mincost[k];
        }
    }
    return min_dist;

}


int hex_graph::virtual_dijkstra(string str){
    // str is either X or O
    // return time which takes from oneside to the other side   
    // X: red path from top to botom, O: blue path from east to west

    // initial setup
    color c = str == "X" ? color::RED : color::BLUE; // color of player ("X" or "O")
    color c_opp =c;  // color of opponent (second player)
    if(c == color::RED)
        c_opp = color::BLUE;
    if(c == color::BLUE)
        c_opp = color::RED;

    int dist = 0, c_cnt = 0, o_cnt = 0; // dist (time), c_cnt: number of elements reachable by time t =dist
    bool cSet[size], oSet[size]; // o_cnt: size of oSet
    vector<int> mincost(size); // initially set to be -1 mincost is same as first passage time
    for(int k=0;k<size;k++){
        cSet[k]=false; oSet[k] = false;  mincost[k]=-1;  
    }

    // setup one side and opposite side
    vector<int> one_side, opposite_side;
    vector<vector<int>> one_side_bridge, opposite_side_bridge;
    one_side_bridge.resize(hsize-1);
    opposite_side_bridge.resize(hsize-1);

    if(str == "X"){
        for(auto w: top_side)
            one_side.push_back(w); 
        for(auto w: bottom_side)
            opposite_side.push_back(w); 
        for(int k=0;k<hsize-1; k++){
            for(auto w: top_bridge[k])
                one_side_bridge[k].push_back(w);
        }
        for(int k=0;k<hsize-1; k++){
            for(auto w: bottom_bridge[k])
                opposite_side_bridge[k].push_back(w);
        }
      
    }
    if(str == "O"){
        for(auto w: left_side)
            one_side.push_back(w); 
        for(auto w: right_side)
            opposite_side.push_back(w); 
        for(int k=0;k<hsize-1; k++){
            for(auto w: left_bridge[k])
                one_side_bridge[k].push_back(w);
        }
        for(int k=0;k<hsize-1; k++){
            for(auto w: right_bridge[k])
                opposite_side_bridge[k].push_back(w);
        }
    }


    // initial aetup of cSet and oSet at time 0
    // step 1 include all elements which can be reached by time=0 to cSet
    for(auto k: one_side){
        if(board[k] == c){
            cSet[k] = true; c_cnt++; mincost[k] = 0;
        }
    }
    for(auto br: one_side_bridge){
        int first=br[0], second=br[1], third=br[2];
        if(board[third] == c && board[first]!=c_opp && board[second]!= c_opp){
            cSet[third]=true; c_cnt++; mincost[third] = 0;
        }
    }
        
    for(int k=0;k<size;k++)
        if(cSet[k])
            for(auto w: virtual_cluster(k))
                if(!cSet[w]){
                    cSet[w] = true; c_cnt++; mincost[w]=0;
                }
        
    // step 2 include empty colors on one side and  empty colors neighbors(neighbor, bridge neighbor) of cSet to oSet
   for(auto k: one_side){
        if(board[k]== color::EMPTY){
            oSet[k] = true; o_cnt++; mincost[k]=1;
        }
    }
    for(auto br: one_side_bridge){
        int first=br[0], second=br[1], third=br[2];
        if(board[third] == color::EMPTY && board[first]==color::EMPTY 
           && board[second] == color::EMPTY){
            oSet[third]=true; o_cnt++; mincost[third] =1;
        }
    }
    for(int k=0;k<size;k++)
        if(cSet[k])
            for(auto w: edgelist[k])
                if(!cSet[w] && !oSet[w] && board[w] == color::EMPTY){
                    oSet[w] = true; o_cnt++; mincost[w]=1;     
                }
    for(int k=0;k<size;k++)
        if(cSet[k])
            for(auto w: virtual_edgelist[k])
                if(!cSet[w] && !oSet[w] && board[w] == color::EMPTY
                    && bridge_neighbor(k, w) == 2){
                    oSet[w] = true; o_cnt++; mincost[w]=1;     
                }


    // step 3 include all color c virtual clusters neighboring oSet to oSet 
    // (elements which can be reached by time 1)          

    for(int k=0;k<size;k++)
        if(oSet[k] && board[k]==color::EMPTY){
            for(auto w: edgelist[k])
                if(!cSet[w] && !oSet[w] && board[w]==c){
                    oSet[w] = true; o_cnt++; mincost[w] = 1;
                }
        }
    for(int k=0;k<size;k++)
        if(oSet[k] && board[k]==color::EMPTY){
            for(auto w: virtual_edgelist[k])
                if(!cSet[w] && !oSet[w] && board[w]==c && bridge_neighbor(k,w)==2){
                    oSet[w] = true; o_cnt++; mincost[w] = 1;
                }
        }
    for(int k=0;k<size;k++)
        if(oSet[k] && board[k]==c){
            for(auto w: virtual_cluster(k))
                if(!oSet[w]){
                    oSet[w] = true; o_cnt++; mincost[w] = 1;
                }
        }

    // increment time by one then update cSet and update oSet in a while loop
    while(o_cnt >0){ 
        dist++;
        // update cSet.
        // step 1:  include all elements which can be reached by time (dist) to cSet
        for(int k=0;k<size;k++)
            if(oSet[k] && mincost[k] == dist){
                cSet[k] =true; c_cnt++; oSet[k] = false; o_cnt--;
            }
            
    

        // update oSet
        // step 2 include all neighboring empty colors of cSet to oSet 
        for(int k=0;k<size;k++){
            if(cSet[k] && mincost[k]== dist){
                for(auto w: edgelist[k])
                    if(!cSet[w] && !oSet[w] && board[w] == color::EMPTY){
                        oSet[w] = true; o_cnt++;
                        if(mincost[w]==-1 || mincost[w] > mincost[k]){
                            mincost[w] = mincost[k]+1;                           
                        }                                
                    }           
            }
        }
        for(int k=0;k<size;k++){
            if(cSet[k] && mincost[k] == dist){
                for(auto w: virtual_edgelist[k])
                    if(!cSet[w] && !oSet[w] && bridge_neighbor(k,w) ==2 && board[w]== color::EMPTY){
                        oSet[w] = true; o_cnt++;
                        if(mincost[w]==-1 || mincost[w] > mincost[k]){
                            mincost[w] = mincost[k]+1;                           
                        }                                
                    }           
            }
        }

        // step 3 include color c virtual clusters neighboring oSet to oSet          
        for(int k=0;k<size;k++)
            if(oSet[k]){
                for(auto w: edgelist[k]){
                    if(!cSet[w] && !oSet[w] && board[w] ==c){
                        oSet[w] = true; o_cnt++;
                        mincost[w]=mincost[k];
                    }
                }
                for(auto w: virtual_edgelist[k]){
                    if(!cSet[w] && !oSet[w] && board[w] ==c && bridge_neighbor(k, w)==2){
                        oSet[w] = true; o_cnt++;
                        mincost[w]=mincost[k];
                    }
                }
            }
        for(int k=0;k<size;k++)
            if(oSet[k] && board[k]==c)
                for(auto w: virtual_cluster(k)){
                    if(!cSet[w] && !oSet[w]){
                        oSet[w] = true; o_cnt++;
                        mincost[w]= mincost[k];
                    }
                }
    }

    // save mincost to class variable
    for(int k=0;k<size; k++)
        if(str=="X")
            virtual_xmincost[k] =mincost[k];
        else
            virtual_omincost[k] =mincost[k];


    // either cSet reaches opposite side in time = min_dist or cannot be reached (-1)
    int min_dist =-1;
    for(auto k: opposite_side){
        if(cSet[k] && min_dist<0)
            min_dist = mincost[k];
        if(cSet[k] && min_dist> mincost[k])
            min_dist = mincost[k];
    }
    for(auto br: opposite_side_bridge){
        int first=br[0], second=br[1], third=br[2];
        if(cSet[third] && board[first] == color::EMPTY && board[second] == color::EMPTY
            && board[third] != c_opp)
            if(min_dist <0 || mincost[third] < min_dist)
                min_dist = mincost[third]; 
    }

    return min_dist;

}


void hex_graph::randomplay(int k, color c){

    color c_opp =c;
    c_opp = (c ==color::RED) ? color::BLUE : color::RED;

    move(k, c);

    int remaining_size = size  - num_moves;
    vector<color> board_remain(remaining_size);
    for(int k1=0;k1<remaining_size;k1++){
        if(k1 < (remaining_size+ 1)/2)
            board_remain[k1] = c_opp;
        else
            board_remain[k1] = c;
    }

    random_shuffle(board_remain.begin(), board_remain.begin() + remaining_size);
    int i=0, j=0;
    for(int i=0;i<size;i++){
        if(board[i] == color::EMPTY){
            board[i] = board_remain[j]; j++;
        }      
    }

};

int hex_graph::monte_carlo(){

    // prinit out size of plausible moves
    int n =  plausible_moves().size();
    cout << "num of plausible moves: "<< n<< endl;

    int best_pos=-1, temp =-1;;
    for(auto pos: plausible_moves()){
        int xwon=0, owon=0;
        assert(move_is_legal(pos));
        if(move_is_legal(pos)){
            xwon=0; owon=0;
            for(int i=0;i<num_play;i++){

                randomplay(pos, hex_graph::color::BLUE);
                if(connected("X")){
                    xwon++;
                }
                if(connected("O")){
                    owon++;
                }
                // restore state to before random play
                move_back(pos);
                reset_to_original();
            }
            // after all trials for given position
            if(temp<0){
                temp = xwon; best_pos = pos;
            }

            if(xwon <temp){
                temp = xwon; best_pos = pos;
            }
         
        }
    }

    // cout << "best o position: (" << best_pos /hsize << ", " << best_pos % hsize<<")" << endl;
    // cout <<"\n";
    // cout << temp << "  " << best_pos;
    return best_pos;
    
};

vector<int> hex_graph::extend(){ // extend blue player 
    vector<int> possible_moves;
    int now = dijkstra("O");
    for(int k=0;k<size;k++){
        if(board[k] == color::EMPTY){
            move(k, color::BLUE);
            if(dijkstra("O") < now){
                possible_moves.push_back(k);
            };
            move_back(k);

        }
    }
    return possible_moves;
}

vector<int> hex_graph::virtual_extend(){ // extend blue player 
    vector<int> possible_moves;
    int now = virtual_dijkstra("O");
    for(int k=0;k<size;k++){
        if(board[k] == color::EMPTY){
            move(k, color::BLUE);
            if(virtual_dijkstra("O") < now && close_to(k, color::BLUE)){
                possible_moves.push_back(k);
            };
            move_back(k);
        }
    }
    return possible_moves;
}

vector<int> hex_graph::block(){ // block red player
    vector<int> possible_moves;
    int now = dijkstra("X");
    for(int k=0;k<size;k++){
        if(board[k] == color::EMPTY){
            move(k, color::BLUE);
            if(dijkstra("X") > now){
                possible_moves.push_back(k);
            };
            move_back(k);
        }
    }
    return possible_moves;
};

vector<int> hex_graph::virtual_block(){
    vector<int> possible_moves;
    int now = virtual_dijkstra("X");
    for(int k=0;k<size;k++){
        if(board[k] == color::EMPTY){
            move(k, color::BLUE);
            if(virtual_dijkstra("X") > now && close_to(k, color::RED)){
                possible_moves.push_back(k);
            };
            move_back(k);

        }
    }
    return possible_moves;
};  

vector<int> hex_graph::block_weak(){
    vector<int> possible_moves;
    int xnow, onow;
    for(int k=0;k<size;k++){
        if(board[k] == color::EMPTY){
            move(k, color::RED);
            xnow = dijkstra("X");
            move_back(k);
            move(k, color::BLUE);
            onow = dijkstra("X");
            if(onow > xnow){
                possible_moves.push_back(k);
            };
            move_back(k);

        }
    }
    return possible_moves;
};  

vector<int> hex_graph::virtual_block_weak(){
    vector<int> possible_moves;
    int xnow, onow;
    for(int k=0;k<size;k++){
        if(board[k] == color::EMPTY){
            move(k, color::RED);
            xnow = virtual_dijkstra("X");
            move_back(k);
            move(k, color::BLUE);
            onow = virtual_dijkstra("X");
            if(onow > xnow){
                possible_moves.push_back(k);
            };
            move_back(k);

        }
    }
    return possible_moves;
};  

vector<int> hex_graph::edge_moves(){ // include empty color neighbors of all red sites
    color  rr =color::RED, bb =color::BLUE, ee= color::EMPTY;
    vector<int> possible_moves, unique_moves;
    cout <<"**** edge moves***\n";
    for(int k=0;k<size;k++)
        if(board[k] == rr){
            for(auto v: edgelist[k])
                if(board[v] == ee)
                    possible_moves.push_back(v);
            for(auto v: virtual_edgelist[k])
                if(bridge_neighbor(k,v)==2 && board[v] ==ee)
                    possible_moves.push_back(v);
        }
    if(possible_moves.size()<2)
        return possible_moves;

    assert(possible_moves.size()>=2);
    // remove duplicates
    sort(possible_moves.begin(), possible_moves.end());
    for(auto it=possible_moves.begin(); it< possible_moves.end() -1; it++){
        int prev = *it, curr = *(it+1);
        if(prev!=curr)
            unique_moves.push_back(prev);
        if(it == possible_moves.end() -2)
            unique_moves.push_back(curr);
    }

    return unique_moves;

}

 vector<int> hex_graph::center_moves(){
    vector<int> possible_moves;
    int center = size/2;
    // red move: center
    // cout << "**center moves**\n";
    if(board[center] == color::RED){
        possible_moves.push_back(center - hsize);
        possible_moves.push_back(center + hsize);
        return possible_moves;
    }
    // red move is  not in a neighbor of center
    if(board[center] == color::EMPTY){
        bool put_in_center =true;
        for(auto w: edgelist[center]){
            if(board[w] != color::EMPTY){
                put_in_center = false;
            }
        }
        if(put_in_center){
            possible_moves.push_back(center);
            return possible_moves;
        }
    }

    // red move: neighbor of center (6 of them)
    if(board[center - hsize] == color::RED || board[center + hsize] == color::RED){
        possible_moves.push_back(center);
        return possible_moves;
    }
    for(auto w: edgelist[center]){
        if(board[w] == color::RED){
            if(w == center -1){
                possible_moves.push_back(center + hsize);
            }
            if(w == center +1){
                possible_moves.push_back(center - hsize);
            }
            if(w == center - hsize+1){
                possible_moves.push_back(center + hsize);
            }
            if(w == center + hsize - 1){
                possible_moves.push_back(center - hsize);
            }

        }

    }
    return possible_moves;
    

    
    
}

void hex_graph::connection_test(){
    cout <<"***testing connection of sites****\n";
    cout << " choose 5 positions k for X: 1, 7, 13, 19, 23 ";
    int w;
    draw_board();
    vector<int> vec_red(5);
    for(int k=0;k<5; k++){
        cin >> w;
        vec_red.push_back(w);
        move(w, color::RED);
    }
    cout << " choose : " << hsize << " positions k for O: 2,3,6, 9, 14,  ";
    draw_board();

    vector<int> vec_blue(5);
    for(int k=0;k<5; k++){
        cin >> w;
        vec_blue.push_back(w);
        move(w, color::BLUE);
    }

    string str; color cc;
    int pos;

    cc = (str == "x" || str =="X")? color::RED : color::BLUE;
    str = (str == "x" || str =="X") ? "X": "O";


    
    draw_board();
    
    cout << "check whether a pair of sites k1 k2 are connected or enter -1 -1 for exit  \n";
    while(true){
        draw_board();

        int k1, k2;
        cin >> k1 >> k2;
        if(k1==-1 || k2 == -1)
            break;
        if(connected(k1,k2))
            cout << "\n connected - true\n";
        else
            cout << "\nconnected - false\n";
        if(virtual_connected(k1,k2))
            cout << "\n virtual connected - true\n";
        else
            cout << "\n virtual connected - false\n";   
    }



    while(true){
        draw_board();
        cout<<" find connected clusters. Enter position k to find k cluster or k virtual cluster (-1 for exit) \n";
        int k;
        cin >> k;
        if(k ==-1)
            break;
        cout << " cluster: ";
        // sort(cluster(k).begin(), cluster(k).end());
        for(auto w: cluster(k))
            cout << w << " ";
        cout << endl;
        cout << " virtual cluster: ";
        // sort(virtual_cluster(k).begin(), virtual_cluster(k).end());
        for(auto w: virtual_cluster(k))
            cout << w << " ";
        cout << endl;
    }
    
    while(true){
        cout <<"*** testing whether str is connected from one side to the other side***\n";
        reset();
        draw_board();
        cout << " choose str X or O: ";
        cin >> str;

        cout << "enter 5 positions k1, k2, ... for str: ";
        vector<int> vec_pos(5); // empty now
        int w;
        for(int k=0;k<5;k++){
            cin >> w; 
            vec_pos[k] = w;
            move(w, cc);
            draw_board();
            if(virtual_connected(str))
                cout << " virtually connected\n";
            if(connected(str))
                cout << " connected\n";        
        }

        for(int k=0;k<5;k++)
            move_back(vec_pos[k]);

        cout << "enter -1 to exit the program\n";
        cin >> pos;
        if(pos ==-1)
            break;
         
    }

   
}


void hex_graph::dijkstra_test(){
    int i,j, k;
    string c, str;
   
    color rr= color::RED, bb = color::BLUE, ee = color::EMPTY;
    while(true){
        
        cout << "enter i and j and str (r b any) \n";
        cin >> i >> j;
        k = i*hsize +j;
        cin >> c;
        if(c=="r"){
            move(k, rr);
            str = "X";
        }else {
            move(k, bb);
            str = "O";
        }
        draw_board();
        // cout  << " x dijkstra dist " << dijkstra("X")<< endl;
        cout  << "virtual  x dijkstra dist " << virtual_dijkstra("X")<< endl;
        cout  << " O dijkstra dist " << dijkstra("O")<< endl;
        // cout  << "virtual o dijkstra dist " << virtual_dijkstra("O")<< endl;
        
    }
   
   
}

void hex_graph::monte_carlo_test(){
    vector<int> vec_red = {12, 15, 18};
    vector<int> vec_blue = {2, 4};
    int best_pos;

    color rr= color::RED, bb = color::BLUE, ee = color::EMPTY;

    for(auto w: vec_red){
        move(w, rr);
    }
    for(auto w: vec_blue){
        move(w, bb);
    }

    draw_board();
    best_pos = monte_carlo();
    if(best_pos== -1){
        cout << "something wrong with monte carlo method";
        return exit(1);
    }
        
    move(best_pos,  bb);
    cout << "best position for O is " << best_pos;
    move_back(best_pos);
};
