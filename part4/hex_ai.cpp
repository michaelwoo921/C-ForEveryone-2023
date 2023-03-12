#include <iostream>
#include <cassert>
// #include<string>
#include <vector>
#include<algorithm>

using namespace std;

class hex_graph {
    public:
        enum class color {EMPTY, RED, BLUE};
        hex_graph(int h=11);
        void draw_board();
        void move(int k, color c);
        void move_back(int k);
        void reset_to_original();
        bool move_is_legal(int k);
        string mark(color c);
        color get_color(string str);

        int bridge_neighbor(int k, int l);    //return -1, 0, 1, 2
        bool neighbor(int k, int l);
        bool empty_neighbor(int k, int l); // empty color on integer l
        bool empty_bridge(int k, int l); // empty color on integer l
        bool empty_line_neighbor(int k, int l); // empty color on integer l
        bool connected(string str); // modified dijkstra to run monte carlo algo faster
        bool virtual_connected(string str); // connected using nearest neighbor and "next" nearest neihbor
        bool connected(int k1, int k2);
        bool virtual_connected(int k1, int k2); 
        vector<int> cluster(int k); // set of connected sites to site k
        vector<int> virtual_cluster(int k); // set of "virtual" connected sites to site k
        vector<int> max_cluster(string str); // largest cluster on graph
        vector<int> max_virtual_cluster(string str);       // largest "virtual" cluster on graph
        int dijkstra(string str);
        int virtual_dijkstra(string str);
        void randomplay(int k, color c);
        int monte_carlo();

        vector<int>extend_max_virtual_cluster();
        vector<int> initial_strat();
        vector<int> plausible_moves();  
        void hex_program();

        void print_cost(string str);
        void print_path(string str);

        void edge_test();
        void connection_test();
        void color_move_test();
        void neighbor_test();
        void dijkstra_test();
        void monte_carlo_test();
        void strat_test();

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

    
        vector<int> xmincost;
        vector<int> omincost;
        vector<int> xpath; // 0 for "X" 1 for "O";
        vector<int> opath; 

        
};

  
vector<int> hex_graph::initial_strat(){
    vector<int> neighbor_moves;
    vector<int> center_moves;
    vector<int> good_moves;
    cout <<"Using initial strategy" <<endl;

    if(board[size/2] == color::EMPTY)
        center_moves.push_back(size/2);
    for(auto w: edgelist[size/2]){
        if(board[w] == color::EMPTY)
            center_moves.push_back(w);
    }
    for(auto w: virtual_edgelist[size/2]){
        if(board[w] == color::EMPTY){
            center_moves.push_back(w);
        }
    }


    for(int k=0; k<size;k++){
        if(board[k] == color::RED){
            for(auto w: edgelist[k]){
                if(board[w] == color::EMPTY){
                    neighbor_moves.push_back(w);
                }
            }
            for(auto w: virtual_edgelist[k]){
                if(board[w] == color::EMPTY){
                    // need more fine tuning?
                    neighbor_moves.push_back(w);
                }
            }
        }
    }
    for(int k=0;k<size;k++){
        if(board[k]==color::BLUE){
            for(auto w: edgelist[k]){
                if(board[w] == color::EMPTY){
                    neighbor_moves.push_back(w);
                }
            }
            for(auto w: virtual_edgelist[k]){
                if(board[w] == color::EMPTY){
                    // need more fine tuning?
                    neighbor_moves.push_back(w);
                }
            }
        }
    }
    vector<int> possible_moves;

    // remove duplicates 
    sort(possible_moves.begin(), possible_moves.end());
    int temp;
    for(int i=0; i<possible_moves.size();i++){
        if(i==0){
            temp = possible_moves[0];
            good_moves.push_back(temp);
        }
        else{
            if(possible_moves[i] != temp){
                temp = possible_moves[i];
                good_moves.push_back(temp);
            }
        }
    }

    return good_moves;
        
}

vector<int> hex_graph::plausible_moves(){
    vector<int> good_moves;

    return initial_strat();


    // extend or block move which can lead to the end of game
    if(num_moves<6 && initial_strat().size() < 25){
        return initial_strat(); 
    }

    cout << "no more initial strategy "<< endl;


    if(dijkstra("O") ==1){
        for(auto w: max_cluster("O")){
            for(auto k: edgelist[w]){
                if(board[k] == color::EMPTY){
                    move(k, color::BLUE);
                    if(connected("O")){
                        good_moves.push_back(k);
                        return good_moves;
                    }      
                }
            }
        }
    }

    if(dijkstra("X") ==1){
        for(auto w: max_cluster("X")){
            for(auto k: edgelist[w]){
                if(board[k] == color::EMPTY){
                    move(k, color::RED);
                    if(connected("X")){
                        move_back(k);
                        good_moves.push_back(k);
                        return good_moves;
                    }      
                }
            }
        }
    }



    if(extend_max_virtual_cluster().size()< 15){
        // print out good moves
        cout <<" good moves: ";
        for(auto w: extend_max_virtual_cluster()){
            good_moves.push_back(w);
            cout << w << " ";
        }
        cout <<"\n";
        // remove duplicates
        
        return good_moves;       
    }


    for(int k=0;k<size; k++){
        if(board[k]==color::EMPTY){
            good_moves.push_back(k);
            
        }
    }
    return good_moves;

    // number of empty neighbors and next nearest bridge neighbors are less than 20
    // then use this initial strategy 
    if(initial_strat().size() < 25){
        // print out good moves
        cout <<" good moves: ";
        for(auto w: initial_strat()){
            cout << w << " ";
        }
        cout <<"\n";
        return initial_strat();        
    }


    return extend_max_virtual_cluster();     

   
        // what is a new strategy???  extend virtual path or path
            
        // find the largest x cluster or O cluster
        // num_moves is small then check moves near previous moves and center
        // block the move or extend move
        // if "X" are almost winning then block the move
        // if "O" is almost winning connect the bridge

    
        // 
    
    
}



int main(){

    int hsize=5; // all tests written only for hsize =5;
    hex_graph g(hsize);
    srand(time(0));

    // g.edge_test();
    // g.color_move_test();
    // g.neighbor_test();

    // g.connection_test();
    g.dijkstra_test();

    // g.strat_test();

    // g.monte_carlo_test();
    // g.hex_program();
    
}


void hex_graph::hex_program(){
   
    cout << "Running hex program. It uses 11 by 11 board\n";
    cout<< " You are a Red player X and you need to create path from top to bottom to win the game\n";
 

    bool game_playing = true;
    while(game_playing){
        draw_board();
        if(virtual_connected("X")){
            cout << "x virtually connected !!!!\n";
        }
            if(virtual_connected("O")){
            cout << "o virtually connected !!!!\n";
        }
 
        int i,j;
        cout << "Type row and col position for X: \n";
        cin >> i >> j;
        if(i==-1 || j == -1)
            break;
        int k =i*hsize +j, best_pos;
        
        move(k, color::RED);
        best_pos= monte_carlo();
        if(best_pos == -1){
            cout <<"\n something wrong with monte carlo method\n";
            break;
        }

        move(best_pos, hex_graph::color::BLUE);
        
        if(virtual_connected("X"))
            cout << "\n x virtually connected\n";
 
        if(virtual_connected("O"))
            cout << "\n o virtually connected\n";;
            
        
        if(connected("X")){
            cout << "\nX won \n";
            game_playing = false;
        }
        if(connected("O")){
            cout << "\nO won \n";
            game_playing = false;
        }
    }

 
}




vector<int> hex_graph::extend_max_virtual_cluster(){
    vector<int> extend_possible_moves;
    vector<int> good_moves;
    int xdist =dijkstra("X");
    int odist =dijkstra("O");
    // extend o cluster
    for(auto w: max_cluster("O")){
        for(int k=0;k<size;k++){
            if(bridge_neighbor(w,k) && board[k] == color::EMPTY){
                move(k, color::BLUE);
                 if(odist > dijkstra("O")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                if(xdist < dijkstra("X")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                move_back(k);      
            }
            if(neighbor(w,k) && board[k] == color::EMPTY){
                move(k, color::BLUE);
                 if(odist > dijkstra("O")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                if(xdist < dijkstra("X")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                move_back(k);      
            }
        }
    }
    // block x cluster
    for(auto w: max_cluster("X")){
        for(int k=0;k<size;k++){
            if(bridge_neighbor(w,k) && board[k] == color::EMPTY){
                move(k, color::BLUE);
                 if(odist > dijkstra("O")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                if(xdist < dijkstra("X")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                move_back(k);      
            }
            if(neighbor(w,k) && board[k] == color::EMPTY){
                move(k, color::BLUE);
                 if(odist > dijkstra("O")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                if(xdist < dijkstra("X")){
                    extend_possible_moves.push_back(k); // still need to check dist decrease 
                }
                move_back(k);      
            }
        }
    }
   
    // extend obvious local bridge
    for(int k=0;k<size;k++){
        if(board[k] == color::BLUE){
            for(auto w: virtual_edgelist[k]){
                if(board[w]== color::EMPTY && bridge_neighbor(w,k)){
                    extend_possible_moves.push_back(w);
                }
            }
        }
    }
    // block obvious opponents local bridge
    for(int k=0;k<size;k++){
        if(board[k] == color::RED){
            for(auto w: virtual_edgelist[k]){
                if(board[w]== color::RED&& k!=w &&!bridge_neighbor(w,k)){
                    if(board[(w+k)/2] !=color::RED && board[(w+k+1)/2] != color::RED){
                        if(board[(w+k)/2] == color::EMPTY){
                            extend_possible_moves.push_back((w+k)/2);
                        }
                        if(board[(w+k+ 1)/2] == color::EMPTY){
                            extend_possible_moves.push_back((w+k+1)/2);
                        }
                    }
                   
                    
                }
            }
        }
    }

   
    // remove duplicates
    int temp;
    sort(extend_possible_moves.begin(), extend_possible_moves.end());
    for(int i=0;i<extend_possible_moves.size(); i++){
        if(i==0){
            temp = extend_possible_moves[0];
            good_moves.push_back(temp);
        }else{
            if(temp!= extend_possible_moves[i]){
                temp = extend_possible_moves[i];
                good_moves.push_back(temp);
            }
        }
    }

    return good_moves;

}



void hex_graph::print_cost(string str){
            // print out mincost
            if(str=="X"){
                for(int k=0;k<size;k++){
                    int i = k/hsize, j = k % hsize;
                    cout << "xmincost: " << xmincost[k] << "for ( " << i << ", "<< j <<" )\n";     
                }
            }
            else{
                for(int k=0;k<size;k++){
                    int i = k/hsize, j = k % hsize;
                    cout << "omincost: " << omincost[k] << "for ( " << i << ", "<< j <<" )\n";     
                }
            }
         
}

void hex_graph::print_path(string str){
      // print out path for X
      if(str =="X"){
        for(int k=size - hsize;k<size; k++){
            int i = k/hsize, j = k % hsize;
            cout << k << ": " << xpath[k] << "##";
            cout << "mincost: " << xmincost[k]; 
            cout <<"\n";
            
        }
      }
      else{
        for(int k=hsize -1;k<size; k += hsize){
            int i = k/hsize, j = k % hsize;
            cout << k << ": " << opath[k] << "##";
            cout << "omincost: " << omincost[k]; 
            cout <<"\n";
            
        }
      }
    

}


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
    xpath.resize(size);
    omincost.resize(size);
    opath.resize(size);

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
    cout <<"\n\n";
}

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
void hex_graph::reset_to_original(){
    for(int k=0;k<size;k++)
        board[k] =board_orig[k];
}
int hex_graph::bridge_neighbor(int k, int l){ 
    // return the number of empty colors in a carrier of bridge
    // return -1, 0, 1, or 2
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

 bool hex_graph::neighbor(int k, int l){
    for(auto w: edgelist[k]){
        if(w == l)
            return true;
    }
    return false;
}

bool hex_graph::empty_neighbor(int k, int l){
    return neighbor(k,l) && board[l] == color::EMPTY;
}
bool hex_graph::empty_bridge(int k, int l){
    return bridge_neighbor(k,l)==2 && board[l] == color::EMPTY;
}
bool hex_graph::empty_line_neighbor(int k, int l){
    int cnt =0, temp=-1;
    color c;
    for(auto w1: edgelist[k])
        for(auto w2: edgelist[l]){
            if(w1==w2){
                cnt++; c = board[w1];
                temp = w1;
            }
        }
    return cnt == 1 && c == color::EMPTY && temp==(k+l)/2;
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


vector<int> hex_graph::max_cluster(string str){
    int cluster_size =-1, cluster_index =-1;
    color c = str == "X" ? color::RED : color::BLUE;
    int temp =-1, index = -1;
    vector<int> emtycluster;
    for(int k=0;k<size;k++){
        if(board[k] == c && temp ==-1){
            temp =cluster(k).size();  index=k;       
        }
        if(cluster(k).size() > temp)
            index = k;
    }
    if(index == -1)
        return emtycluster;
    else
        return cluster(index);
}


vector<int> hex_graph::max_virtual_cluster(string str){
    color c = str == "X" ? color::RED : color::BLUE;
    vector<int> emtycluster;
    int temp=-1, index=-1;
    for(int k=0;k<size;k++){
        if(board[k] == c && temp ==-1){
            temp = virtual_cluster(k).size(); index =k;
        }
        if(temp <virtual_cluster(k).size()){
            temp = virtual_cluster(k).size(); index =k;
        }
    }
    if(index == -1)
        return emtycluster;

    return virtual_cluster(index);
}



int hex_graph::dijkstra(string str){
    // str is either X or O
    // return time which takes from oneside to the other side   
    // X: red path from top to botom, O: blue path from east to west

    // initial setup
    int dist = -1, c_cnt = 0, o_cnt = 0; // dist (time), c_cnt: number of elements reachable by time t =dist
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

    // initial update of cSet (at time  =0) starting from oneside (cSet has all color c now)
    for(auto k: one_side){
        if(board[k] == c){
            if(dist<0)
                dist =0;
            cSet[k] = true; c_cnt++; mincost[k] = 0;
        }
    }
    for(int k=0;k<size;k++)
        if(cSet[k]){
            for(auto w: cluster(k)){
                if(!cSet[w]){
                    cSet[w] = true; c_cnt++; mincost[w]=0;
                }
            }
        }

    // initial update of oSet (at time  =0) starting from oneside (oSet has all color empty now)
   for(auto k: one_side){
        if(board[k]== color::EMPTY){
            oSet[k] = true; o_cnt++; mincost[k]=1;
        }
    }
    for(int k=0;k<size;k++){
        if(cSet[k]){
            for(auto w: edgelist[k]){
                if(!cSet[w] && !oSet[w] && board[w] == color::EMPTY){
                    oSet[w] = true; o_cnt++; 
                    if(mincost[w] == -1)
                        mincost[w] = mincost[k] + 1;
                    if(mincost[w] > mincost[k] + 1)
                        mincost[w] = mincost[k] +1;
                }
            }
        }
    }

    // if cSet reaches opposite side then return
    for(auto w: opposite_side){
        if(cSet[w] && board[w] == c)
            return dist;
    }
    for(auto w: opposite_side){
        if(cSet[w] && board[w] == color::EMPTY)
            return dist;
    }

    while(o_cnt >0){
        // update cSet and increment time by 1
        for(int k=0;k<size;k++){ 
            if(oSet[k]){ 
                cSet[k] = true; c_cnt++; oSet[k] = false; o_cnt--;
            }
        }
       
        for(int k=0;k<size;k++){ // cSet includes cluster of color c
            if(cSet[k] && board[k] ==c){
                for(auto w: cluster(k)){
                    if(!cSet[w]){
                        cSet[w] =true; c_cnt++;
                        mincost[w] = mincost[k];
                    }
                }
            }
        }
        // oSet is empty now and update oSet
        for(int k=0;k<size;k++){
            if(cSet[k]){
                for(auto w: edgelist[k]){
                    if(!cSet[w] && !oSet[w] && board[w] == c){
                        oSet[w] = true; o_cnt++;             
                        if(mincost[w]==-1)
                            mincost[w] = mincost[k];
                        if(mincost[w] > mincost[k]){
                            mincost[w] = mincost[k];
                        }                          
                    }
                }
            }
        }

        for(int k=0;k<size;k++){
            if(cSet[k]){
                for(auto w: edgelist[k]){
                    if(!cSet[w] && !oSet[w] && board[w] == color::EMPTY){
                        oSet[w] = true; o_cnt++;             
                        if(mincost[w]==-1)
                            mincost[w] = mincost[k]+1;
                        if(mincost[w] > mincost[k]+1){
                            mincost[w] = mincost[k]+1;
                        }                          
                    }
                }
            }
        }


        cout << " dist "<< dist << "\n";
        cout<<"mincost*********\n";
        draw_board();
        for(int k=0;k<size;k++){
            if(k% 5 ==0)
                cout << endl;
            cout << mincost[k] << " ";
        }
        cout << endl;
        // cSet reaches opposite side then return
     
        for(auto w: opposite_side){
            if(cSet[w] && board[w]== c)
                return dist;
        }
        for(auto w: opposite_side){
            if(cSet[w] && board[w]== color::EMPTY)
                return dist+1;
        }
        dist++;

    }


    
    return -1;




}


int hex_graph::virtual_dijkstra(string str){
      // str is either X or O
    // return dist from onside to arr of positions plus arr of positions to the other side   
    // X: red path from top to botom, O: blue path from east to west

    int dist = -1;  
    // dist is a distance from one end to arr plus distance from arr to the other 
    // setup cSet and oSet initially set false,  c_cnt: 0
    bool cSet[size], oSet[size];
    vector<int> mincost(size);
    int c_cnt = 0; // size of cSet.  
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

    // initial update of cSet and oSet at oneside
    for(auto k: one_side){
        if(board[k] == c){
            if(dist<0)
                dist =0;
            cSet[k] = true; c_cnt++; mincost[k] = 0;
        }
    }
    if(str =="X"){
        for(int k=0; k<hsize -1; k++){
            if(board[k] != c_opp && board[k+1] != c_opp){
                if(board[k+ hsize] == c){
                    if(dist<0)
                        dist =0;
                    cSet[k+hsize] = true; c_cnt++; mincost[k+hsize] = 0;
                }

            }
        }
    }
    if(str =="O"){
        for(int k=0; k<size -hsize; k += hsize){
            if(board[k] != c_opp && board[k+ hsize] != c_opp){
                if(board[k+ 1] == c){
                    if(dist<0)
                        dist =0;
                    cSet[k+1] = true; c_cnt++; mincost[k+1] = 0;
                }

            }
        }
    }
        // oSet
    for(auto k: one_side){
        if(board[k] == color::EMPTY){
   
            oSet[k] = true; mincost[k] = 1;
        }
    }
    if(str =="X"){
        for(int k=0; k<hsize -1; k++){
            if(board[k] == color::EMPTY && board[k+1] == color::EMPTY){
                if(board[k+ hsize] == color::EMPTY){
            
                    oSet[k+hsize] = true; mincost[k+hsize] = 1;
                }

            }
        }
    }
    if(str =="O"){
        for(int k=0; k<size -hsize; k += hsize){
            if(board[k] == color::EMPTY && board[k+ hsize] == color::EMPTY){
                if(board[k+ 1] == color::EMPTY){
                    oSet[k+1] = true; mincost[k+1] = 1;
                }

            }
        }
    }


    for(int k=0;k<size; k++){
        if(cSet[k]){
            for(auto w: virtual_cluster(k)){
                if(!cSet[w]){
                    cSet[w] = true; c_cnt++; mincost[w] = mincost[k];
                }
            }
        }
    }


    // if initial update (with dist 0) includes arr el then skip the following while loop
    int temp=1;
    vector<int> arr;
    for(auto k: arr){
        if(cSet[k]){
            temp=-1; break; // temp = -1 means c_cnt>0 and  already reached arr so it skips while loop
        }
    }
    // if initial cset with dist 0 is empty include cSet width dist 1
   
    if(c_cnt ==0){
        for(auto k: one_side){
            if(board[k] == color::EMPTY){
                cSet[k]= true; c_cnt++; dist=1; mincost[k]=1; 
            }
        }
        if(str =="X"){
            for(int k=0; k<hsize -1; k++){
                if(board[k] == color::EMPTY && board[k+1] == color::EMPTY){
                    if(board[k+ hsize] == color::EMPTY){   
                        cSet[k + hsize]  = true; c_cnt++; mincost[k + hsize] = 1; dist =1;
                    }
                }
            }
        }
        if(str =="O"){
            for(int k=0; k<size -hsize; k += hsize){
                if(board[k] == color::EMPTY && board[k+ hsize] == color::EMPTY){
                    if(board[k+ 1] == color::EMPTY){
                        cSet[k+1] = true; c_cnt++; mincost[k+1] = 1; dist=1;
                    }

                }
            }
        }
    
        // if reached arr then skip while loop
        for(auto k: arr){
            if(cSet[k]){
                temp =-1; break;
            }
        }
   
    }
    // cSet is empty 

   
   
    if(c_cnt ==0)
        return -1;
    // cSet is not empty and dist is either 0 or 1


   // when c_cnt does not update, while loop runs until it reaches arr
 
    while(c_cnt< size && temp>0){
        temp = c_cnt;
        // update oSet which can be reached from cSet by one unit
        for(int k=0;k<size; k++){
            for(auto w: edgelist[k]){
                if(cSet[k] && !oSet[w] && !cSet[w] && board[w] == color::EMPTY){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k] +1;
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k] +1;
                    }
                    
                }
            }
            for(auto w: edgelist[k]){
                if(cSet[k] && !oSet[w] && !cSet[w] && board[w] == c){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k];
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k];
                    }
                    
                }
            }
        }

        for(int k=0;k<size;k++){
            for(auto w: virtual_edgelist[k]){        
                // case: w has  color c
                if(cSet[k]&& !oSet[w]&& !cSet[w] && bridge_neighbor(k,w)==2 && board[w] == c){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k];
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k];
                    } 
                }
            }
            for(auto w: virtual_edgelist[k]){        
                // case: w has empty color
                if(cSet[k]&& !oSet[w]&& !cSet[w] && bridge_neighbor(k,w)==2 && board[w] == color::EMPTY){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k] +1;
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k] +1;
                    } 
                }
            }
        }
        // check if oSet is empty (break)
        // include oSet to cSet which has zero unit away
        int temposet =-1; // -1 means oSet empty 
        for(int k=0;k<size;k++){
            if(oSet[k] && mincost[k] == dist && temposet <0){
                temposet =1; 
            }
            if(oSet[k] &&  mincost[k] == dist){
                oSet[k] = false; cSet[k] = true; c_cnt++;
            }
        }
        if(temposet <0)
            break;



        for(int k=0;k<size;k++){
            if(cSet[k]){
                for(auto w: cluster(k)){
                    if(oSet[w]){
                        cSet[w] = true; oSet[w] = false; c_cnt++;
                    }
                }
            }

        }

 

        // check if arr includes cSet element if it does then break from while loop
        int temp_arr=-1;
        for(auto w: arr){
            if(cSet[w]){
                temp_arr=1;
                break; // 
            }
        }
        if(temp_arr == 1){
            break; 
        }
        
        // update dist 
        dist++;
        

    }

   
   // reaching here means either one side cannot reaches arr or reaches arr
   temp= -1;
    for(auto w: arr){
        if(cSet[w]){
            temp=1;
        }
    }
    if(temp==-1)
        return -1; 
    // now path reached arr 
    for(auto w: arr){
        cSet[w]= true; oSet[w] = false; mincost[w] = dist;
    }



    for(int k=0;k<size;k++){
        if(cSet[k] && board[k] == c){
            for(auto w: virtual_cluster(k)){
                if(!cSet[w]){
                    cSet[w] = true; c_cnt++; mincost[w] = mincost[k];
                }
            }
        }
    }




    // check if opposite side include cSet
    for(auto w: opposite_side){
        if(cSet[w]){
            mincost[w] = dist; 
            return dist;
        }
    }
    if(str =="X"){
        for(int k=size -hsize; k<size-1; k++){
            if(board[k] != c_opp && board[k+1] != c_opp){
                if(board[k- hsize+1] == c){
                    if(cSet[k-hsize+1]){
                        mincost[k-hsize+1] = dist; 
                        return dist;
                    }
                }

            }
        }
    }
    if(str =="O"){
        for(int k=hsize-1; k<size -hsize; k += hsize){
            if(board[k] != c_opp && board[k+ hsize] != c_opp){
                if(board[k+ hsize-1] == c){
                    if(cSet[k+hsize-1]){
                        mincost[k+hsize-1] = dist;
                        return dist;
                    }
                }

            }
        }
    }


    // cSet includes arr and update cSet until it reaches the opposite side
    while(c_cnt< size){
        temp = c_cnt;
        // update oSet which can be reached from cSet by one unit
        for(int k=0;k<size; k++){
            for(auto w: edgelist[k]){
                if(cSet[k] && !oSet[w] && !cSet[w] && board[w] == color::EMPTY){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k] +1;
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k] +1;
                    }
                    
                }
            }
        }
        for(int k=0;k<size; k++){
            for(auto w: edgelist[k]){
                if(cSet[k] && !oSet[w] && !cSet[w] && board[w] == c){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k];
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k];
                    }
                    
                }
            }
        }

        for(int k=0;k<size;k++){
            for(auto w: virtual_edgelist[k]){        
                // case: w has empty color
                if(cSet[k]&& !oSet[w] && !cSet[w] && bridge_neighbor(k,w)==2 && board[w] == color::EMPTY){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k] +1;
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k] +1;
                    } 
                }
            }

        }

  
        for(int k=0;k<size;k++){
            for(auto w: virtual_edgelist[k]){        
                // case: w has empty color
                if(cSet[k]&& !oSet[w] && !cSet[w] && bridge_neighbor(k,w)==2 && board[w] == c){
                    oSet[w] = true; 
                    if(mincost[w] == -1){
                        mincost[w] = mincost[k];
                    }
                    if(mincost[w] > mincost[k]){
                        mincost[w] = mincost[k];
                    } 
                }
            }
        
        }
        
         // break if oSet is empty
        // include oSet to cSet and increment dist by 1
        int temposet =-1; // -1 means oSet empty 
         for(int k=0;k<size;k++){
            if(oSet[k] &&  temposet <0){
                temposet =1;
            }
         
        }
        if(temposet == -1){
            cout <<"haha! break";
            break;
        }
  



        for(int k=0;k<size;k++){
            if(oSet[k] && mincost[k] == dist){
               oSet[k] = false; cSet[k] = true; c_cnt++;
            }
        }

        for(int k=0;k<size;k++){
            if(cSet[k]){
                for(auto w: virtual_cluster(k)){
                    if(!cSet[w]){
                        cSet[w] = true;
                        mincost[w] = mincost[k];
                        c_cnt++;
                    }
                    
                }
            }
        }

        // check if opposite side include cSet
        for(auto w: opposite_side){
            if(cSet[w]){
                return dist;
            }
        }
        if(str =="X"){
            for(int k=size -hsize; k<size-1; k++){
                if(board[k] != c_opp && board[k+1] !=c_opp){
                    if(board[k- hsize+1] == c && cSet[k-hsize+1]){
                        return dist;
                    }
                }
            }
        }
        if(str =="O"){
            for(int k=hsize-1; k<size -hsize; k += hsize){
                if(board[k] != c_opp && board[k+ hsize] != c_opp){
                    if(board[k+ hsize-1] == c && cSet[k+hsize-1]){
                        return dist;
                        
                    }
                }
            }
        }
        // update dist
        dist++;

        if(str =="X"){
            for(int k=size -hsize; k<size-1; k++){
                if(board[k] != c_opp && board[k+1] !=c_opp){
                    if(board[k- hsize+1] == color::EMPTY && cSet[k-hsize+1]){
                        return dist+1;
                    }
                }
            }
        }
        if(str =="O"){
            for(int k=hsize-1; k<size -hsize; k += hsize){
                if(board[k] != c_opp && board[k+ hsize] != c_opp){
                    if(board[k+ hsize-1] == color::EMPTY && cSet[k+hsize-1]){
                        return dist+1;
                        
                    }
                }
            }
        }

        // check oSet is empty if not include to cSet
        if(temp == c_cnt){ // oSet was empty 
            return -1; // cannot be reach to opposite side
        }
    }


        for(auto w: opposite_side){
            if(cSet[w]){
                return dist;
            }
        }

        if(str =="X"){
            for(int k=size -hsize; k<size-1; k++){
                if(board[k] == color::EMPTY && board[k+1] == color::EMPTY){
                    if(board[k- hsize+1] == c && cSet[k-hsize+1]){
                        return dist;
                    }
                }
            }
        }
        if(str =="O"){
            for(int k=hsize-1; k<size -hsize; k += hsize){
                if(board[k] == color::EMPTY && board[k+ hsize] == color::EMPTY){
                    if(board[k+ hsize-1] == c && cSet[k+hsize-1]){
                        return dist;
                        
                    }
                }
            }
        }
        if(str =="X"){
            for(int k=size -hsize; k<size-1; k++){
                if(board[k] == color::EMPTY && board[k+1] == color::EMPTY){
                    if(board[k- hsize+1] == color::EMPTY && cSet[k-hsize+1]){
                        return dist+ 1;
                    }
                }
            }
        }
        if(str =="O"){
            for(int k=hsize-1; k<size -hsize; k += hsize){
                if(board[k] == color::EMPTY && board[k+ hsize] == color::EMPTY){
                    if(board[k+ hsize-1] == color::EMPTY && cSet[k+hsize-1]){
                        return dist+ 1;
                        
                    }
                }
            }
        }

    cout << "aaa \n";
    return -1;
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

    cout << "best o position: (" << best_pos /hsize << ", " << best_pos % hsize<<")" << endl;
    cout <<"\n";
    cout << temp << "  " << best_pos;
    return best_pos;
    
};












// to be deleted
// bool hex_graph::dijkstra(string str){
//     // str is either X or O
//     // return true if there is a path for str from one side to the opposite side
//     // X: red path from top to botom, O: blue path from east to west

//     // setup cSet and oSet initially set false, mincost: -1, c_cnt: 0
//     bool cSet[size], oSet[size];
//     vector<int> mincost(size);
//     vector<int>path(size);
//     int c_cost =-1; // used to update cSet. all elements of cSet has cost less than or equal to c_cost
//     int c_cnt = 0; // size of cSet.  
//     for(int k=0;k<size;k++){
//         cSet[k]=false; oSet[k] = false; 
//          mincost[k] =-1; path[k]= -1;
     
        
//     }

//     // setuo one side and opposite side
//     vector<int> one_side, opposite_side;
//     if(str == "X"){
//         for(int k=0; k<hsize; k++)
//             one_side.push_back(k); // top side 0th row
//         for(int k=size - hsize; k<size; k++)
//             opposite_side.push_back(k); // bottom side,last row
//     }
//     if(str == "O"){
//         for(int k=0; k<size; k += hsize)
//             one_side.push_back(k); // left side 0th column
//         for(int k=hsize -1; k<size; k += hsize)
//             opposite_side.push_back(k); // right side,last column
//     }

//     // initial update of cSet at oneside
//     color c = str == "X" ? color::RED : color::BLUE;
//     color c_opp;  // color of opponent
//     if(c == color::RED)
//         c_opp = color::BLUE;
//     if(c == color::BLUE)
//         c_opp = color::RED;
//     for(auto k: one_side){
//         if(board[k] == c){
//             cSet[k] = true; 
//            c_cnt++; c_cost =0;
            
//         mincost[k] =0; path[k]= k;
        
//         }
//         if(board[k] == color::EMPTY){
//             cSet[k] = true; mincost[k] =1; path[k]=k; c_cnt++;
//             cout <<"***"<< mincost[k] << "&&&&";
//         }
//     }
//     int temp = c_cnt; // when c_cnt does not update, while loop exits
//     while(c_cnt< size && temp>0){
//         temp = c_cnt;
//         // update oSet which can be reached from cSet at most one unit
//         for(int k=0;k<size; k++){
//             for(auto w: edgelist[k]){
//                 // case: w has color c
//                 if(cSet[k] && !cSet[w] && board[w] == c){
//                     oSet[w] = true; 
//                     if(mincost[w] == -1) {
//                         mincost[w] = mincost[k]; path[w] =k;
//                     }
//                     if(mincost[w]> mincost[k]){
//                         mincost[w] = mincost[k]; path[w] =k;
//                     }
//                 }
//                 // case: w has empty color
//                 if(cSet[k] && !cSet[w] && board[w] == color::EMPTY){
//                     oSet[w] = true; 
//                     if(mincost[w] == -1) {
//                         mincost[w] = mincost[k]+ 1; path[w] =k;
//                     }
//                     if(mincost[w]> mincost[k]){
//                         mincost[w] = mincost[k]+1; path[w] =k;
//                     }
//                 }

//             }
//         }
//         // move elements of oSet which has cost c_cost to cSet
//         for(int k=0;k<size;k++){
//             if(oSet[k] && mincost[k] == c_cost){
//                 cSet[k] = true; oSet[k] = false; c_cnt++;
//             }
//         }
        
//         if(temp == c_cnt){
//             c_cost +=1;
//             // increment c_cost by 1
//             // move elements of oSet which has cost c_cost to cSet
//             for(int k=0;k<size;k++){
//                 if(oSet[k] && mincost[k] == c_cost){
//                     cSet[k] = true; oSet[k] = false; c_cnt++;
//                 }
//             }

//         }
//         // exit the loop if c_cost becomes size;
//         if(c_cost >= size)
//             break;
            
//     }
//     // print_cost();
//     // print_path();
//     for(int k=0;k<size;k++){
//         if(str == "X"){
//             xmincost[k] =mincost[k]; xpath[k] =path[k];
//         }
//         else{
//             omincost[k] =mincost[k]; opath[k] =path[k];
//         }
//     }
    
//     for(auto k: opposite_side){
//         if(cSet[k] && mincost[k] == 0)
//             return true;
//     }

//     return false;

// }





void hex_graph::edge_test(){

    cout <<"hsize ";
    cout << hsize;
    cout<< endl; 

    cout <<"size ";
    cout << size;
    cout<< endl; 

    cout << "top side ";
    for(auto w: top_side){
        cout << w << " ";
    }
    cout<< endl; 

    cout << "bottom side ";
    for(auto w: bottom_side){
        cout << w << " ";
    }
    cout<< endl; 


    cout << "left side ";
    for(auto w: left_side){
        cout << w << " ";
    }
    cout<< endl; 


    cout << "right side ";
    for(auto w: right_side){
        cout << w << " ";
    }
    cout<< endl; 

    color rr = color::RED, bb = color::BLUE, ee = color::EMPTY;
    cout << "top, bottom, left and right bridge test: \n";
    while(true){
        draw_board();
        cout << " enter one string and one integer to get bridge or 0 0 to exit\n";
        cout << "eg l 4, r 2, t 3, b 2): \n";
        string pos; int i;
        cin >> pos;
        cin >> i;
        int first, second, third;
        if(pos == "t"){
            first = i; second = i+1; third = first + hsize;
        }
        else if(pos =="b"){
            first = size - hsize +i; second = first + 1; third = second - hsize;
        }
        else if(pos =="l"){
            first = i*hsize; second = first + hsize; third = first +1;
        }
        else if(pos =="r"){
            first = i*hsize + hsize -1; second = first + hsize; third = second - 1;
        }else{
            break;
        }
        move(first, rr);
        move(second, rr);
        move(third, bb);
        draw_board();
        string ch;
        cout << "enter y to try again or any character to exit\n";
        cin >> ch;
        move_back(first);
        move_back(second);
        move_back(third);
        if(ch !="y")
            break;
    }




  


  


    draw_board();
    cout << "\n*****edge_list test ***** ";
    cout<< endl; 
    int temp=1;
    while(temp>0){
        cout << "enter position i and j  to exit press -1 -1 ";
        int i, j, edge;
        cin >> i >> j;
        if(i<0 || i>=hsize && j<0 ||  j >=hsize)
            break;
        edge = i*hsize + j;
        move(edge, rr);
        for(auto w: edgelist[edge]){
            move(w, bb);
        }
        draw_board();
        cout <<"\n num_moves: " << num_moves << "\n";
        move_back(edge);
        for(auto w: edgelist[edge]){
            move_back(w);
        }
        reset_to_original();
    }

    cout << "\n******virtual_edge_list test *****";
    cout<< endl; 
    temp=1;
    while(temp>0){
        cout << "enter position i and j  to exit press -1 -1 ";
        int i, j, edge;
        cin >> i >> j;
        if(i<0 || i>=hsize && j<0 ||  j >=hsize)
            break;
        edge = i*hsize + j;
        move(edge, rr);
        for(auto w: virtual_edgelist[edge]){
            move(w, bb);
        }
        draw_board();
        cout <<"\n num_moves: " << num_moves << "\n";
        move_back(edge);
        for(auto w: virtual_edgelist[edge]){
            move_back(w);
        }
        reset_to_original();
    }


};
void hex_graph::color_move_test(){
    color rr = color::RED, bb = color::BLUE, ee =color::EMPTY;
    int pos1=2, pos2=5;
    string red = mark(rr), blue = mark(bb);
    move(pos1, rr); move(pos2,bb);
    cout << "testing color and string mark \n";
    
    draw_board();
    cout << pos1 << " is " << red << endl;
    cout << pos2 << " is " << blue << endl;
    if(get_color("X") == color::RED && get_color("O") == color::BLUE 
        && get_color(".") == color::EMPTY){
        cout << "color and string mark test passed\n";
    }
    else{
        cout << "color and str test failed\n"; exit;
    }

    cout << "\nlegal move test***\n";

    cout << "position " << pos1 << " put " << mark(bb);
    if(move_is_legal(pos1)){
        move(pos1, bb);
        cout <<" ---illegal move. test failed \n";
    }
    else{
        cout << " ---illegal move: test passed \n";
    }
    draw_board();
    move_back(pos1);
    cout << "move back then at position " << pos1 << " put " << mark(bb);
        if(move_is_legal(pos1)){
        move(pos1, bb);
        cout <<" ---valid move passed \n";
    }
    else{
        cout << " ---valid move: failed \n";
    }
    draw_board();

};
void hex_graph::connection_test(){
    cout <<"***testing connection of sites****\n";
    vector<int> vec_red={0, 6, 10,  12, 16 };
    vector<int> vec_blue={3,4, 7,21};

    for(auto w: vec_red){
        board[w] = color::RED;
    }
        
    for(auto w: vec_blue){
        board[w] = color::BLUE;
    }

    draw_board();
    
    cout << "a pair of connected sites: \n";
    for(int k1=0;k1<size;k1++)
        for(int k2=k1;k2<size;k2++){
            if(k1!=k2 && connected(k1,k2)){
                cout << k1 << " " << k2 << ",  ";
            }
        }
    cout <<endl;


    cout<<" connected clusters: \n";
    for(int k=0;k<size;k++){
        if(!cluster(k).empty()){
           for(auto w: cluster(k)){
                cout << w << " ";
            }
            cout << endl;
        }
 
    }
   
    cout << " max cluster: \n";
    cout << "red X: ";
    for(auto w: max_cluster("X"))
        cout << w << " ";
    cout << endl;
    cout << "blue O: ";
    for(auto w: max_cluster("O"))
        cout << w << " ";
    cout << endl;

    cout<<" connected virtual clusters: \n";
    for(int k=0;k<size;k++){
        if(!virtual_cluster(k).empty()){
           for(auto w: virtual_cluster(k)){
                cout << w << " ";
            }
            cout << endl;
        }
 
    }

    cout << " max virtual cluster: \n";
    cout << "red X: ";
    for(auto w: max_virtual_cluster("X"))
        cout << w << " ";
    cout << endl;
    cout << "blue O: ";
    for(auto w: max_virtual_cluster("O"))
        cout << w << " ";
    cout << endl;

    cout << "connected: ";
    for(int k1=0;k1<size;k1++){
        for(int k2=k1;k2<size;k2++){
            if(k1!=k2){
                if(connected(k1,k2))
                    cout << k1 << " " << k2 << ",   ";
            }
        }
    }
    cout << endl;
    
    cout << "virtual connected: ";
    for(int k1=0;k1<size;k1++){
        for(int k2=k1;k2<size;k2++){
            if(k1!=k2){
                if(virtual_connected(k1,k2))
                    cout << k1 << " " << k2 << ",   ";
            }
        }
    }
    cout << endl;
    cout <<"*************************************\n";
    draw_board();
    cout <<"*** testing whether str is connected from one side to the other side***\n";
    cout << " choose str X or O: ";
    string str;
    cin >> str;
    if(str == "x")
        str ="X";
    else 
        str ="O";
    color cc = str =="X" ? color::RED : color::BLUE;
   
    vector<int> vec_pos; // empty now
    while(true){

        draw_board();
        int pos;
        cout << "enter position for str: ";
        cin >> pos;
        if(move_is_legal(pos)){
            move(pos, cc);
            vec_pos.push_back(pos);
        }

        if(connected(str)){
            cout << "connected now \n";
            draw_board();
            break;
        }else{
            cout << "not connected yet \n";
        }
        
    }
    for(auto w: vec_pos)
        move_back(w);
    draw_board();
    cout <<"*** testing whether str is virtually connected from one side to the other side***\n";
    cout << " choose str X or O: ";

    cin >> str;
    if(str=="x")
        str ="X";
    else
        str ="O";
    cc = str =="X" ? color::RED : color::BLUE;

    while(true){
        int pos;
        draw_board();
        cout << "enter position for str: ";
        cin >> pos;
        if(move_is_legal(pos)){
            move(pos, cc);
        }
        
        if(virtual_connected(str)){
            cout << "virtually connected now \n";
            draw_board();
            break;
        }else{
            cout << "not virtually connected yet \n";
        }
        


    }
   
}

void hex_graph::neighbor_test(){
    
    vector <int> vec_red = {2, 4, 10};
    vector<int>  vec_blue ={3,5, 7};
    string str ="X";
    color rr = color::RED, bb = color::BLUE, ee= color::EMPTY;
    for(auto w: vec_red){
        move(w, rr);
    }
    for(auto w: vec_blue){
        move(w, bb);
    }
    draw_board();

    cout << "**** x empty neighbor test\n";
    for(auto w: vec_red){
        for(int k=0;k<size;k++){
            if(k%5 ==0 && empty_neighbor(w, k)){
                cout << endl;
            }
            if(empty_neighbor(w, k)){
                cout << w << " " << k<< ",  ";
            }
        }
    }
    cout << endl;
    
       cout << "**** x empty bridge neighbor test\n";
    for(auto w: vec_red){
        for(int k=0;k<size;k++){
            if(k%5 ==0 && empty_bridge(w, k)){
                cout << endl;
            }
            if(empty_bridge(w, k)){
                cout << w << " " << k<< ",  ";
            }
        }
    }
    cout << endl;

    cout << "**** x empty line neighbor test\n";
    for(auto w: vec_red){
        for(int k=0;k<size;k++){
            if(k%5 ==0 && empty_line_neighbor(w, k)){
                cout << endl;
            }
            if(empty_line_neighbor(w, k)){
                cout << w << " " << k<< ",  ";
            }
        }
    }
    cout << endl;
         
}

void hex_graph::dijkstra_test(){
    // vector<int> vec_red = {24};
    // int k =size/2;
   
    color rr= color::RED, bb = color::BLUE, ee = color::EMPTY;
    // for(auto w: vec_red){
    //     move(w, rr);   
    // }
   
      
    int i,j, k;
    string c, str;
    while(true){
        cout << "enter i and j and letter r or b or back \n";
        cin >> i >> j;
        cin >> c;
        k = i*hsize +j;
        if(c=="r"){
            move(k, rr);
            str = "X";
        }else if(c=="b"){
            move(k, bb);
            str = "O";
        }else{
            move_back(k);
        }
        draw_board();
        cout << str << " dijkstra dist " << dijkstra(str)<< endl;
        cout << str << " virtual dijkstra dist " << virtual_dijkstra(str)<< endl;
    
    }

    
    



   
}

void hex_graph::strat_test(){
    color rr = color::RED, bb = color::BLUE;
    int pos=size/2;
    vector<int> vec_red = {24};
    move(pos, rr);
    draw_board();
    int dist, vdist;
    dist = dijkstra("X");
    vdist = virtual_dijkstra("X");
    cout << "x dijkstra dist: " << dist << endl;
    cout << "x virtual dijkstra dist: " << vdist << endl;

    for(int k=0; k<size; k++){
        move(k, rr);
        int dijk = dijkstra("X");
        if(dijk< dist)
            cout <<k << " x dijkstra dist: " << dijk << endl;
        move_back(k);
    }
    cout<<"\n****************\n";

    for(int k=0; k<size; k++){
        
        move(k, rr);
        int dijk = virtual_dijkstra("X");
        if(dijk< vdist)
            cout <<k << " x virtual_dijkstra dist: " << dijk << endl;
        move_back(k);
    }

};

void hex_graph::monte_carlo_test(){
    vector<int> vec_red = {12, 15, 18};
    vector<int> vec_blue = {2, 4};

    color rr= color::RED, bb = color::BLUE, ee = color::EMPTY;

    for(auto w: vec_red){
        move(w, rr);
    }
    for(auto w: vec_blue){
        move(w, bb);
    }

    draw_board();
    monte_carlo();
};